#pragma once
#include <atomic>
#include <mutex>
#include <string>
#include <unordered_map>

#include "nlohmann/json.hpp"

namespace projectnestor {
using json = nlohmann::json;

std::string generate_uuid();
std::string now_iso8601();

class Store {
 public:
  json get_stats() const;
  json submit_research(const json& body);

 private:
  mutable std::mutex mutex_;
  std::atomic<int> active_{0};
  std::atomic<int> completed_{0};
  std::atomic<int> pending_{0};
  std::unordered_map<std::string, json> research_items_;
};
}  // namespace projectnestor
