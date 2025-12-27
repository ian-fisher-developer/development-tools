#include <gtest/gtest.h>

#include "stats/StatisticsCalculator.hpp"
#include "test_data/DocumentedTestSet.hpp"
#include "test_data/StressData.hpp"

// Note that this test takes many hours to run. It is gathered
// in its own test program to avoid slowing the regular unit test program.

TEST(Statistics, ExtremeStress_AgreesWithDocumentedExample)
{
    // The test can fail when rounding errors in the internal double precision
    // calculations accumulate all the way up to the precision level of the
    // returned 32-bit results.
    //
    // For kurtosis, rounding errors become visible at the 32-bit precision
    // level after about 2.6 billion values. For skewness, at about 28 billion
    // values. Standard deviation, at about 134 billion.

    const std::vector<float>& test_set = documented_test_set::values();

    stats::StatisticsCalculator calculator;

    uint64_t large_number_counter(0);
    while (large_number_counter < kQuarterTrillion)
    {
        for (const float& value : test_set)
        {
            calculator.add(value);
            ++large_number_counter;
        }

        ASSERT_EQ(large_number_counter, calculator.count());
        ASSERT_EQ(documented_test_set::minimum(), calculator.minimum());
        ASSERT_EQ(documented_test_set::maximum(), calculator.maximum());
        ASSERT_EQ(documented_test_set::mean(), calculator.mean());
        ASSERT_EQ(documented_test_set::absolute_mean(), calculator.absolute_mean());

        ASSERT_FLOAT_EQ(documented_test_set::quadratic_mean(), calculator.quadratic_mean())
            << "calculator.count(): " << calculator.count();

        if (large_number_counter < kHundredThirtyFourBillion)
        {
            ASSERT_FLOAT_EQ(documented_test_set::standard_deviation(),
                            calculator.standard_deviation())
                << "calculator.count(): " << calculator.count();
        }

        if (large_number_counter < kTwentyEightBillion)
        {
            ASSERT_FLOAT_EQ(documented_test_set::skewness(), calculator.skewness())
                << "calculator.count(): " << calculator.count();
        }

        if (large_number_counter < kTwoPointSixBillion)
        {
            ASSERT_FLOAT_EQ(documented_test_set::kurtosis(), calculator.kurtosis())
                << "calculator.count(): " << calculator.count();
        }
    }
}
