"""ProjectNestor stub server — research task intake and NATS dispatch."""

import asyncio
import json
import logging
import os
import uuid
from contextlib import asynccontextmanager

import nats
from fastapi import FastAPI
from pydantic import BaseModel

logging.basicConfig(level=logging.INFO, format="%(asctime)s [%(levelname)s] %(message)s")
log = logging.getLogger("nestor-stub")

NATS_URL = os.environ.get("NATS_URL", "nats://localhost:4222")

# In-memory state
active = 0
completed = 0
pending = 0
research_tasks: dict[str, dict] = {}

_nc = None
_js = None


async def handle_research_message(msg):
    """Process incoming research messages: wait 2s, publish completion, update counters."""
    global active, completed, pending
    try:
        data = json.loads(msg.data.decode())
        research_id = data.get("research_id", "unknown")
        log.info(f"Received research request {research_id}, processing...")

        await asyncio.sleep(2)

        if _js is not None:
            completion_subject = f"hi.research.{research_id}.completed"
            payload = json.dumps({
                "research_id": research_id,
                "status": "completed",
                "idea": data.get("idea", ""),
                "context": data.get("context", ""),
            }).encode()
            await _js.publish(completion_subject, payload)
            log.info(f"Published completion to {completion_subject}")

        pending = max(0, pending - 1)
        completed += 1

        if research_id in research_tasks:
            research_tasks[research_id]["status"] = "completed"

        await msg.ack()
    except Exception as e:
        log.error(f"Error processing research message: {e}")
        try:
            await msg.nak()
        except Exception:
            pass


@asynccontextmanager
async def lifespan(app: FastAPI):
    global _nc, _js
    try:
        _nc = await nats.connect(NATS_URL)
        _js = _nc.jetstream()

        # Ensure stream exists
        try:
            await _js.add_stream(name="homeric-research", subjects=["hi.research.>"])
            log.info("Created stream homeric-research")
        except Exception as e:
            if "already in use" in str(e).lower() or "exists" in str(e).lower():
                log.info("Stream homeric-research already exists")
            else:
                log.warning(f"Stream creation warning: {e}")

        # Subscribe to process research requests
        await _js.subscribe("hi.research.>", cb=handle_research_message, durable="nestor-stub")
        log.info(f"Connected to NATS at {NATS_URL}, subscribed to hi.research.>")
    except Exception as e:
        log.warning(f"NATS unavailable ({e}), continuing without NATS")

    yield

    if _nc is not None:
        await _nc.drain()


app = FastAPI(title="ProjectNestor Stub", lifespan=lifespan)


class ResearchRequest(BaseModel):
    idea: str
    context: str = ""


@app.get("/v1/health")
async def health():
    return {"status": "ok"}


@app.get("/v1/research/stats")
async def research_stats():
    return {"active": active, "completed": completed, "pending": pending}


@app.post("/v1/research", status_code=202)
async def create_research(req: ResearchRequest):
    global pending
    research_id = str(uuid.uuid4())
    pending += 1
    research_tasks[research_id] = {
        "id": research_id,
        "idea": req.idea,
        "context": req.context,
        "status": "pending",
    }

    if _js is not None:
        try:
            subject = f"hi.research.{research_id}"
            payload = json.dumps({
                "research_id": research_id,
                "idea": req.idea,
                "context": req.context,
            }).encode()
            await _js.publish(subject, payload)
            log.info(f"Published research request {research_id} to {subject}")
        except Exception as e:
            log.error(f"Failed to publish research request: {e}")

    return {"id": research_id, "status": "pending"}
