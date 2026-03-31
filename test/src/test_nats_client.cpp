#include "projectnestor/nats_client.hpp"

#include <memory>

#include <gtest/gtest.h>

namespace projectnestor::test {

TEST(NatsClientTest, ConstructorSetsDisconnected) {
  NatsClient client("nats://localhost:4222");
  EXPECT_FALSE(client.is_connected());
}

TEST(NatsClientTest, CloseWhenNotConnected) {
  NatsClient client("nats://localhost:4222");
  client.close();
  EXPECT_FALSE(client.is_connected());
}

TEST(NatsClientTest, PublishWhenNotConnectedReturnsFalse) {
  NatsClient client("nats://localhost:4222");
  EXPECT_FALSE(client.publish("hi.research.test", R"({"test":true})"));
}

TEST(NatsClientTest, EnsureStreamsWhenNotConnectedReturnsEarly) {
  NatsClient client("nats://localhost:4222");
  client.ensure_streams();
  EXPECT_FALSE(client.is_connected());
}

TEST(NatsClientTest, DestructorWhenNotConnected) {
  auto ptr = std::make_unique<NatsClient>("nats://localhost:4222");
  EXPECT_FALSE(ptr->is_connected());
  ptr.reset();
  SUCCEED();
}

TEST(NatsClientTest, ConnectToInvalidUrlFails) {
  NatsClient client("nats://127.0.0.1:1");
  EXPECT_FALSE(client.connect());
  EXPECT_FALSE(client.is_connected());
}

TEST(NatsClientTest, DoubleCloseIsSafe) {
  NatsClient client("nats://localhost:4222");
  client.close();
  client.close();
  EXPECT_FALSE(client.is_connected());
}

}  // namespace projectnestor::test
