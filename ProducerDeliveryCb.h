#pragma once

#include "Producer.h"
#include "ProducerMessage.h"
#include "ProducerStats.h"
#include <librdkafka/rdkafkacpp.h>
#include "../logger/logger.h"

namespace KafkaZ {

class ProducerDeliveryCb : public RdKafka::DeliveryReportCb {
public:
  explicit ProducerDeliveryCb(ProducerStats &Stats) : Stats(Stats){};

  void dr_cb(RdKafka::Message &Message) override {
    if (Message.err() != RdKafka::ERR_NO_ERROR) {
        ZINNION_LOG(error, "ERROR on delivery, topic {}, {} [{}]", Message.topic_name(), Message.err(),
                    Message.errstr());
      ++Stats.produce_cb_fail;
    } else {
      ++Stats.produce_cb;
    }
    // When produce was called, we gave RdKafka a pointer to our message object
    // This is returned to us here via Message.msg_opaque() so that we can now
    // clean it up
    delete reinterpret_cast<ProducerMessage *>(Message.msg_opaque());
  }

private:
  ProducerStats &Stats;
};
} // namespace KafkaZ
