#include "stats/StatisticsUtilities.hpp"

#include <gtest/gtest.h>

TEST(Statistics, ProvidesComparableUndefinedMarker)
{
    float value = stats::undefined();
    EXPECT_EQ(value, stats::undefined()); // Fails if the "undefined" marker is a NaN.
}

TEST(Statistics, ChecksValueIsUndefined)
{
    float value = stats::undefined();
    EXPECT_TRUE(stats::undefined(value));
}

TEST(Statistics, UndefinedMarkerShowsClearlyInHexDump)
{
    std::uint32_t bits;
    memcpy(&bits, &stats::undefined(), sizeof(float));
    EXPECT_EQ(0xFEFEFEFE, bits);
}
