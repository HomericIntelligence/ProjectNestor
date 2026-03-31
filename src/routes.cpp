// ProjectNestor route handlers — C++20 with real Store and NATS integration.

#include "projectnestor/routes.hpp"

#include <string>

#include "httplib.h"
#include "nlohmann/json.hpp"

namespace projectnestor {

using json = nlohmann::json;

void register_routes(httplib::Server& server, Store& store, NatsClient& nats) {
  Store* sp = &store;
  NatsClient* np = &nats;

  // ── Health ────────────────────────────────────────────────────────────────

  server.Get("/v1/health", [](const httplib::Request& /*req*/, httplib::Response& res) {
    res.set_content(json{{"status", "ok"}}.dump(), "application/json");
  });

  // ── Research ─────────────────────────────────────────────────────────────

  server.Get("/v1/research/stats", [sp](const httplib::Request& /*req*/, httplib::Response& res) {
    res.set_content(sp->get_stats().dump(), "application/json");
  });

  server.Post("/v1/research", [sp, np](const httplib::Request& req, httplib::Response& res) {
    const auto body = json::parse(req.body, nullptr, false);
    if (body.is_discarded()) {
      res.status = 400;
      res.set_content(json{{"detail", "Invalid JSON"}}.dump(), "application/json");
      return;
    }

    const json result = sp->submit_research(body);
    const std::string id = result["id"].get<std::string>();

    // Publish to hi.research.<id> — graceful degradation if NATS unavailable.
    const std::string subject = "hi.research." + id;
    json payload = body;
    payload["id"] = id;
    payload["status"] = "pending";
    np->publish(subject, payload.dump());

    res.status = 202;
    res.set_content(result.dump(), "application/json");
  });
}

}  // namespace projectnestor
