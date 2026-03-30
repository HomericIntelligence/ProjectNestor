#pragma once

#include "httplib.h"
#include "projectnestor/nats_client.hpp"
#include "projectnestor/store.hpp"

namespace projectnestor {

/// Register all HTTP route handlers onto the server.
void register_routes(httplib::Server& server, Store& store, NatsClient& nats);

}  // namespace projectnestor
