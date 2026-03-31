#pragma once

#include "projectnestor/nats_client.hpp"
#include "projectnestor/store.hpp"

#include "httplib.h"

namespace projectnestor {

/// Register all HTTP route handlers onto the server.
void register_routes(httplib::Server& server, Store& store, NatsClient& nats);

}  // namespace projectnestor
