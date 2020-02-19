#pragma once
#include "ConfigureKafka.h"
#include "Consumer.h"
#include <memory>

namespace KafkaZ {
std::unique_ptr<Consumer> createConsumer(const BrokerSettings &Settings,
                                         const std::string &Broker);
} // namespace KafkaZ
