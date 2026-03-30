// ProjectNestor route handlers — C++20 skeleton
// Each handler returns the correct JSON shape with hardcoded/stub data.
// TODO markers indicate where real business logic should be implemented.

#include "projectnestor/routes.hpp"

#include <string>
#include "httplib.h"
#include "nlohmann/json.hpp"

namespace projectnestor {

using json = nlohmann::json;

void register_routes(httplib::Server& server) {
  // ── Health ────────────────────────────────────────────────────────────────

  server.Get("/v1/health", [](const httplib::Request& /*req*/, httplib::Response& res) {
    // TODO: check internal service health
    res.set_content(json{{"status", "ok"}}.dump(), "application/json");
  });

  // ── Research ─────────────────────────────────────────────────────────────

  server.Get("/v1/research/stats", [](const httplib::Request& /*req*/, httplib::Response& res) {
    // TODO: return real research task counts from backing store
    res.set_content(
        json{{"active", 0}, {"completed", 0}, {"pending", 0}}.dump(), "application/json");
  });

  server.Post("/v1/research", [](const httplib::Request& req, httplib::Response& res) {
    // TODO: accept research brief, enqueue to hi.research.* NATS subject
    const auto body = json::parse(req.body, nullptr, false);
    if (body.is_discarded()) {
      res.status = 400;
      res.set_content(json{{"detail", "Invalid JSON"}}.dump(), "application/json");
      return;
    }
    res.status = 501;
    res.set_content(json{{"detail", "Research submission not yet implemented"}}.dump(),
                    "application/json");
  });
}

}  // namespace projectnestor
