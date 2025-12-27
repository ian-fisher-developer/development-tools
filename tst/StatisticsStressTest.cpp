#include <gtest/gtest.h>

#include "stats/StatisticsCalculator.hpp"
#include "stats/StatisticsReport.hpp"
#include "test_data/DocumentedTestSet.hpp"
#include "test_data/StressData.hpp"

// Note that each of these tests takes many minutes to run. They are gathered
// in their own test program to avoid slowing the regular unit test program.

TEST(Statistics, Stress_CountsPast32BitLimits)
{
    stats::StatisticsCalculator calculator;
    uint64_t large_number_counter(0);
    while (large_number_counter < kFiveBillion)
    {
        calculator.add(1.5F);
        ++large_number_counter;
    }
    EXPECT_EQ(large_number_counter, calculator.count());
    EXPECT_EQ("5000000000 Values\n Minimum  = 1.5\n Maximum  = 1.5\n Mean     = 1.5\n Abs.Mean = "
              "1.5\n Rms      = 1.5\n Std.Devn = 0",
              stats::description(calculator));
}

TEST(Statistics, Stress_CountsPast32BitLimitsWithTwoCalculators)
{
    const float value(1.5F);
    uint64_t large_number_counter(0);

    stats::StatisticsCalculator calculator1;
    while (large_number_counter < kTwoBillion)
    {
        calculator1.add(value);
        ++large_number_counter;
    }

    stats::StatisticsCalculator calculator2;
    while (large_number_counter < kFiveBillion)
    {
        calculator2.add(value);
        ++large_number_counter;
    }

    EXPECT_EQ(kTwoBillion, calculator1.count());
    EXPECT_EQ(kThreeBillion, calculator2.count());

    stats::StatisticsCalculator combined = calculator1 + calculator2;
    EXPECT_EQ(kFiveBillion, combined.count());
    EXPECT_EQ("5000000000 Values\n Minimum  = 1.5\n Maximum  = 1.5\n Mean     = 1.5\n Abs.Mean = "
              "1.5\n Rms      = 1.5\n Std.Devn = 0",
              stats::description(combined));
}
