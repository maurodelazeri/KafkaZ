#pragma once

namespace KafkaZ {
struct ProducerMessage {
  virtual ~ProducerMessage() = default;
  unsigned char *data{};
  uint32_t size{};
};
} // namespace KafkaZ
