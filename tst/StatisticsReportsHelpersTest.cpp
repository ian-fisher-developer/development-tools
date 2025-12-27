#include "StatisticsReportsHelpers.hpp"

#include <gtest/gtest.h>

TEST(StatisticsReportsHelpers, CountDescriptionForNoValues)
{
    EXPECT_EQ("No Values", stats::detail::count_description(0));
}

TEST(StatisticsReportsHelpers, CountDescriptionForOneValue)
{
    EXPECT_EQ("1 Value", stats::detail::count_description(1));
}

TEST(StatisticsReportsHelpers, CountDescriptionForTwoValues)
{
    EXPECT_EQ("2 Values", stats::detail::count_description(2));
}

TEST(StatisticsReportsHelpers, LabelAndValueHasSixSignificantDigits)
{
    EXPECT_EQ(" Value = 6.54321", stats::detail::label_and_value(" Value = ", 6.5432123456));
}
