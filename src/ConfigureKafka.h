#pragma once

#include "BrokerSettings.h"
#include <librdkafka/rdkafkacpp.h>
#include "../logger/logger.h"

namespace KafkaZ {
void configureKafka(RdKafka::Conf *RdKafkaConfiguration,
                    const KafkaZ::BrokerSettings& Settings);
}
