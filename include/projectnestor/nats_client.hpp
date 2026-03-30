#pragma once
#include <string>

namespace projectnestor {

class NatsClient {
 public:
  explicit NatsClient(const std::string& url);
  ~NatsClient();

  // Returns true if connection succeeded.
  bool connect();
  void close();
  bool is_connected() const;

  // Ensure the homeric-research JetStream stream exists.
  void ensure_streams();

  // Publish payload to subject. Returns false if not connected or publish fails.
  bool publish(const std::string& subject, const std::string& payload);

 private:
  std::string url_;
  void* conn_ = nullptr;
  void* js_ = nullptr;
  bool connected_ = false;
};

}  // namespace projectnestor
