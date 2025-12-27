#include "stats/StatisticsReport.hpp"

#include <gtest/gtest.h>

#include "stats/StatisticsCalculator.hpp"
#include "test_data/DocumentedTestSet.hpp"

TEST(StatisticsReport, BehavesWellWithNoValues)
{
    stats::StatisticsCalculator calculator;

    EXPECT_EQ("No Values", stats::description(calculator));
}

TEST(StatisticsReport, BehavesWellWithOneValue)
{
    stats::StatisticsCalculator calculator;

    float value(123.4F);
    calculator.add(value);

    EXPECT_EQ("1 Value\n Value = 123.4", stats::description(calculator));
}

TEST(StatisticsReport, BehavesWellWithConstantValues)
{
    stats::StatisticsCalculator calculator;

    for (size_t i = 0; i < 10; ++i)
    {
        calculator.add(2);
    }

    EXPECT_EQ("10 Values\n Minimum  = 2\n Maximum  = 2\n Mean     = 2\n Abs.Mean = 2\n Rms      = "
              "2\n Std.Devn = 0",
              stats::description(calculator));
}

TEST(StatisticsReport, AgreesWithDocumentedExample)
{
    stats::StatisticsCalculator calculator;

    for (size_t i = 0; i < documented_test_set::count(); ++i)
    {
        calculator.add(documented_test_set::values()[i]);
    }

    EXPECT_EQ(documented_test_set::statistics_description(), stats::description(calculator));
}
