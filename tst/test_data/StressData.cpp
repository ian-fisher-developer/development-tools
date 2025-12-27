#include "StressData.hpp"

#include <limits>

const uint64_t kThousand = 1000U;
const uint64_t kMillion  = kThousand * kThousand;
const uint64_t kBillion  = kThousand * kMillion;

const uint64_t kTwoBillion   = 2U * kBillion;
const uint64_t kThreeBillion = 3U * kBillion;
const uint64_t kFiveBillion  = 5U * kBillion;

const uint64_t kTwoPointSixBillion       = 2U * kBillion + 600U * kMillion;
const uint64_t kTwentyEightBillion       = 28U * kBillion;
const uint64_t kHundredThirtyFourBillion = 134U * kBillion;

const uint64_t kQuarterTrillion = 250U * kBillion;

const float kLargeValue = std::numeric_limits<float>::max();
