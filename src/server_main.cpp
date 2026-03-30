// ProjectNestor HTTP Server — C++20 skeleton
// Real business logic is TODO; this binary compiles and serves stub responses.
// The Python stub in stub/server.py is used for E2E testing until this is complete.

#include <csignal>
#include <cstdlib>
#include <iostream>
#include <string>

#include "httplib.h"
#include "projectnestor/routes.hpp"
#include "projectnestor/version.hpp"

namespace {
httplib::Server* g_server = nullptr;

void signal_handler(int /*signal*/) {
  std::cout << "\nShutting down ProjectNestor...\n";
  if (g_server != nullptr) {
    g_server->stop();
  }
}
}  // namespace

int main() {
  const std::string host = "0.0.0.0";
  const int port = []() -> int {
    const char* env = std::getenv("NESTOR_PORT");
    return env != nullptr ? std::stoi(env) : 8081;
  }();

  std::cout << projectnestor::kProjectName << " v" << projectnestor::kVersion << "\n";
  std::cout << "Starting HTTP server on " << host << ":" << port << "\n";

  httplib::Server server;
  g_server = &server;

  std::signal(SIGINT, signal_handler);
  std::signal(SIGTERM, signal_handler);

  projectnestor::register_routes(server);

  std::cout << "Routes registered. Listening...\n";
  if (!server.listen(host, port)) {
    std::cerr << "Failed to start server on port " << port << "\n";
    return 1;
  }

  return 0;
}
