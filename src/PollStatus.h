#pragma once

namespace KafkaZ {
enum class PollStatus { Message, Error, EndOfPartition, Empty, TimedOut };
}
