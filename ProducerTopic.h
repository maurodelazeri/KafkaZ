#pragma once

#include "Producer.h"
#include <memory>
#include <string>

namespace KafkaZ {

struct ProducerMessage;

class TopicCreationError : public std::runtime_error {
public:
  TopicCreationError() : std::runtime_error("Can not create Kafka topic") {}
};

class ProducerTopic {
public:
  ProducerTopic(std::shared_ptr<Producer> ProducerPtr, std::string TopicName);
  virtual ~ProducerTopic() = default;

  /// \brief Send a message to Kafka for publishing on this topic.
  ///
  /// Note: this copies the provided data, so use only for low volume
  /// publishing.
  ///
  /// \param MsgData The message to publish
  /// \return 0 if message is successfully passed to RdKafka to be published, 1
  /// otherwise
  virtual int produce(const std::string &MsgData);
  [[nodiscard]] std::string name() const;

private:
  std::unique_ptr<RdKafka::Conf> ConfigPtr{
      RdKafka::Conf::create(RdKafka::Conf::CONF_TOPIC)};
  std::shared_ptr<Producer> KafkaProducer;
  std::unique_ptr<RdKafka::Topic> RdKafkaTopic;
  std::string Name;
  int produce(std::unique_ptr<KafkaZ::ProducerMessage> &Msg);
};
} // namespace KafkaZ
