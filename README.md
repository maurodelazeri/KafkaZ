# KafkaZ
High level C++ wrapper for librdkafka - https://github.com/edenhill/librdkafka


# kafkatesting

### Consumer example

```c++

#include <iostream>
#include "src/KafkaZ/ConsumerFactory.h"

using namespace std;

int main(int argc, char **argv) {
    std::string broker = "x.x.x.x:9092";
    KafkaZ::BrokerSettings settings;
    settings.Address = broker;
    std::string topic = "test";

    std::unique_ptr<KafkaZ::ConsumerInterface> consumer_ = KafkaZ::createConsumer(settings, broker);
        consumer_->addTopic(topic);
        //consumer_->addTopicAtTimestamp(topic,1582155192000ms); you can specify based on timestamp also
        while (true) {
            std::unique_ptr<std::pair<KafkaZ::PollStatus, Msg>> KafkaMessage =
                    consumer_->poll();
            if (KafkaMessage->first == KafkaZ::PollStatus::Message) {
                cout << KafkaMessage->second.data() << endl;
            }
    }
    return 0;
}
```

```c++
#include "src/KafkaZ/Producer.h"
#include "src/KafkaZ/ProducerTopic.h"

using namespace std;

int main(int argc, char **argv) {
    std::string broker = "x.x.x.x:9092";
    KafkaZ::BrokerSettings settings;
    settings.Address = broker;
    std::string topic = "test";

    auto producer = std::make_shared<KafkaZ::Producer>(settings);
    KafkaZ::ProducerTopic pt(producer, topic);
    while (true) {
        pt.produce("got it " + to_string(rand()));
        sleep(1);
    }
    return 0;
}
```
