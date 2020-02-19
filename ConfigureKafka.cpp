#include "ConfigureKafka.h"

namespace KafkaZ {
void configureKafka(RdKafka::Conf *RdKafkaConfiguration, const KafkaZ::BrokerSettings &Settings) {
    std::string ErrorString;
    for (const auto &ConfigurationItem : Settings.KafkaConfiguration) {
        ZINNION_LOG(info, "set config: {} = {}", ConfigurationItem.first, ConfigurationItem.second);
        if (RdKafka::Conf::ConfResult::CONF_OK !=
            RdKafkaConfiguration->set(ConfigurationItem.first,
                                      ConfigurationItem.second, ErrorString)) {
            ZINNION_LOG(warn, "Failure setting config: {} = {}", ConfigurationItem.first, ConfigurationItem.second);
        }
    }
}
} // namespace KafkaZ
