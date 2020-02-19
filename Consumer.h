#pragma once

#include "BrokerSettings.h"
#include "ConsumerRebalanceCb.h"
#include "KafkaEventCb.h"
#include "PollStatus.h"
#include <chrono>
#include <librdkafka/rdkafkacpp.h>
#include <memory>
#include "Msg.h"

struct Msg;

namespace KafkaZ {

class ConsumerInterface {
public:
  ConsumerInterface() = default;
  virtual ~ConsumerInterface() = default;
  virtual void addTopic(std::string const &Topic) = 0;
  virtual void addTopicAtTimestamp(std::string const &Topic,
                                   std::chrono::milliseconds StartTime) = 0;
  virtual std::unique_ptr<std::pair<PollStatus, Msg>> poll() = 0;
  virtual bool topicPresent(const std::string &Topic) = 0;
  virtual std::vector<int32_t>
  queryTopicPartitions(const std::string &TopicName) = 0;
  virtual std::vector<int64_t>
  offsetsForTimesAllPartitions(std::string const &Topic,
                               std::chrono::milliseconds Time) = 0;
  virtual int64_t getHighWatermarkOffset(std::string const &Topic,
                                         int32_t Partition) = 0;
  virtual std::vector<int64_t> getCurrentOffsets(std::string const &Topic) = 0;
};

class Consumer : public ConsumerInterface {
public:
  /// The constructor
  ///
  /// \param RdConsumer The RdKafka Consumer to wrap.
  /// \param RdConf The Configuration for the RdKafka Consumer.
  /// \param EventCb Callback for each time the consumer is polled.
  Consumer(std::unique_ptr<RdKafka::KafkaConsumer> RdConsumer,
           std::unique_ptr<RdKafka::Conf> RdConf,
           std::unique_ptr<KafkaEventCb> EventCb);
  Consumer(Consumer &&) = delete;
  Consumer(Consumer const &) = delete;
  ~Consumer() override;
  /// Adds topic to consumer at latest offset.
  ///
  /// \param Topic The name of the topic to add.
  void addTopic(std::string const &Topic) override;
  /// Adds topic to consumer, consuming from a specified timestamp
  ///
  /// \param Topic The name of the topic to add.
  /// \param StartTime Start timestamp to consume from.
  void addTopicAtTimestamp(std::string const &Topic,
                           std::chrono::milliseconds StartTime) override;
  /// Checks if a topic is present on the broker.
  /// This is used to prevent passing around a pointer to an iterator of an
  /// RdKafka::Topic outside of KafkaZ and instead returns a bool.
  /// \param TopicName Name of the topic to check.
  /// \return True or false depending on whether the topic exists or not.
  bool topicPresent(const std::string &TopicName) override;
  /// Get a list of partition numbers in a topic.
  ///
  /// \param Topic The name of the topic to query.
  /// \return List of partition numbers on topic.
  std::vector<int32_t> queryTopicPartitions(const std::string &Topic) override;
  /// Polls for any new messages.
  ///
  /// \return Any new messages consumed.
  std::unique_ptr<std::pair<PollStatus, Msg>> poll() override;

  /// Returns first available offset after given time, for each partition in
  /// specified topic
  /// \param Topic The name of the topic
  /// \param Time The time to get offsets corresponding to
  /// \return Offset for each partition in topic
  std::vector<int64_t>
  offsetsForTimesAllPartitions(std::string const &Topic,
                               std::chrono::milliseconds Time) override;

  /// Get the current known high watermark offset from the consumer
  /// Does not query the broker
  /// \param Topic The name of the topic
  /// \param Partition The parition number to get the offset for
  /// \return high watermark offset
  int64_t getHighWatermarkOffset(std::string const &Topic,
                                 int32_t Partition) override;

  /// Get the current position of the consumer for each partition in the given
  /// topic
  /// \param Topic The name of the topic
  /// \return Vector of current offset in each partition
  std::vector<int64_t> getCurrentOffsets(std::string const &Topic) override;

protected:
  std::unique_ptr<RdKafka::KafkaConsumer> KafkaConsumer;

private:
  std::unique_ptr<RdKafka::Conf> Conf;
  BrokerSettings ConsumerBrokerSettings;
  std::shared_ptr<RdKafka::Metadata> getMetadata();
  int id = 0;
  std::unique_ptr<KafkaEventCb> EventCallback;
  void assignToPartitions(
      const std::string &Topic,
      const std::vector<RdKafka::TopicPartition *> &TopicPartitionsWithOffsets);
  std::vector<RdKafka::TopicPartition *>
  queryWatermarkOffsets(const std::string &Topic);
  std::shared_ptr<RdKafka::Metadata> metadataCall();
  //SharedLogger Logger = spdlog::get("filewriterlogger");
  std::vector<RdKafka::TopicPartition *>
  offsetsForTimesForTopic(std::string const &Topic,
                          std::chrono::milliseconds Time);
  bool queryOffsetsForTimes(
      std::vector<RdKafka::TopicPartition *> &TopicPartitionsWithTimestamp);
  size_t CurrentNumberOfPartitions = 0;
  std::string CurrentTopic;

  size_t getNumberOfPartitionsInTopic(const std::string &Topic);
  static std::vector<int64_t>
  getOffsets(std::vector<RdKafka::TopicPartition *> const &TopicPartitions);
  static void deletePartitions(
      std::vector<RdKafka::TopicPartition *> const &TopicPartitions);
};
} // namespace KafkaZ
