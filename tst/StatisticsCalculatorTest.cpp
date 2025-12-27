#include "stats/StatisticsCalculator.hpp"

#include <gtest/gtest.h>
#include <limits>

#include "stats/StatisticsUtilities.hpp"
#include "test_data/DocumentedTestSet.hpp"

namespace
{ // unnamed namespace

void test_equivalence(const stats::StatisticsCalculator& expected,
                      const stats::StatisticsCalculator& actual)
{
    EXPECT_EQ(expected.count(), actual.count());
    EXPECT_EQ(expected.minimum(), actual.minimum());
    EXPECT_EQ(expected.maximum(), actual.maximum());
    EXPECT_EQ(expected.mean(), actual.mean());
    EXPECT_EQ(expected.absolute_mean(), actual.absolute_mean());
    EXPECT_EQ(expected.quadratic_mean(), actual.quadratic_mean());
    EXPECT_EQ(expected.standard_deviation(), actual.standard_deviation());
    EXPECT_EQ(expected.skewness(), actual.skewness());
    EXPECT_EQ(expected.kurtosis(), actual.kurtosis());
}

} // unnamed namespace

TEST(StatisticsCalculator, BehavesWellWithNoValues)
{
    stats::StatisticsCalculator calculator;

    EXPECT_EQ(0U, calculator.count());
    EXPECT_TRUE(stats::undefined(calculator.minimum()));
    EXPECT_TRUE(stats::undefined(calculator.maximum()));
    EXPECT_TRUE(stats::undefined(calculator.mean()));
    EXPECT_TRUE(stats::undefined(calculator.absolute_mean()));
    EXPECT_TRUE(stats::undefined(calculator.quadratic_mean()));
    EXPECT_TRUE(stats::undefined(calculator.standard_deviation()));
    EXPECT_TRUE(stats::undefined(calculator.skewness()));
    EXPECT_TRUE(stats::undefined(calculator.kurtosis()));
}

TEST(StatisticsCalculator, BehavesWellWithOneValue)
{
    stats::StatisticsCalculator calculator;

    float value(123.4F);
    calculator.add(value);

    EXPECT_EQ(1U, calculator.count());
    EXPECT_EQ(value, calculator.minimum());
    EXPECT_EQ(value, calculator.maximum());
    EXPECT_EQ(value, calculator.mean());
    EXPECT_EQ(value, calculator.absolute_mean());
    EXPECT_EQ(value, calculator.quadratic_mean());
    EXPECT_EQ(0.F, calculator.standard_deviation());
    EXPECT_TRUE(stats::undefined(calculator.skewness()));
    EXPECT_TRUE(stats::undefined(calculator.kurtosis()));
}

TEST(StatisticsCalculator, BehavesWellWithConstantValues)
{
    stats::StatisticsCalculator calculator;

    float value(234.5F);
    std::vector<float> test_set(50, value);
    for (const float& value : test_set)
    {
        calculator.add(value);
    }

    EXPECT_EQ(test_set.size(), calculator.count());
    EXPECT_EQ(value, calculator.minimum());
    EXPECT_EQ(value, calculator.maximum());
    EXPECT_EQ(value, calculator.mean());
    EXPECT_EQ(value, calculator.absolute_mean());
    EXPECT_EQ(value, calculator.quadratic_mean());
    EXPECT_EQ(0.F, calculator.standard_deviation());
    EXPECT_TRUE(stats::undefined(calculator.skewness()));
    EXPECT_TRUE(stats::undefined(calculator.kurtosis()));
}

TEST(StatisticsCalculator, WorksWithNegativeValues)
{
    stats::StatisticsCalculator calculator;

    calculator.add(-6.F);
    calculator.add(-2.F);
    calculator.add(-1.F);

    EXPECT_EQ(3, calculator.count());
    EXPECT_EQ(-6.F, calculator.minimum());
    EXPECT_EQ(-1.F, calculator.maximum());
    EXPECT_EQ(-3.F, calculator.mean());
    EXPECT_EQ(+3.F, calculator.absolute_mean());

    double mean_square      = (-6.0 * -6.0 + -2.0 * -2.0 + -1.0 * -1.0) / 3;
    double root_mean_square = sqrt(mean_square);
    EXPECT_FLOAT_EQ(root_mean_square, calculator.quadratic_mean());

    double variance = (pow(-6.0 - -3.0, 2) + pow(-2.0 - -3.0, 2) + pow(-1.0 - -3.0, 2)) / 3;
    double standard_deviation = sqrt(variance);
    EXPECT_FLOAT_EQ(standard_deviation, calculator.standard_deviation());
}

TEST(StatisticsCalculator, WorksWithPositiveValues)
{
    stats::StatisticsCalculator calculator;

    calculator.add(6.F);
    calculator.add(2.F);
    calculator.add(1.F);

    EXPECT_EQ(3, calculator.count());
    EXPECT_EQ(1.F, calculator.minimum());
    EXPECT_EQ(6.F, calculator.maximum());
    EXPECT_EQ(3.F, calculator.mean());
    EXPECT_EQ(3.F, calculator.absolute_mean());

    double mean_square      = (6.0 * 6.0 + 2.0 * 2.0 + 1.0 * 1.0) / 3;
    double root_mean_square = sqrt(mean_square);
    EXPECT_FLOAT_EQ(root_mean_square, calculator.quadratic_mean());

    double variance           = (pow(6.0 - 3.0, 2) + pow(2.0 - 3.0, 2) + pow(1.0 - 3.0, 2)) / 3;
    double standard_deviation = sqrt(variance);
    EXPECT_FLOAT_EQ(standard_deviation, calculator.standard_deviation());
}

TEST(StatisticsCalculator, WorksWithZeroCenteredValues)
{
    stats::StatisticsCalculator calculator;

    calculator.add(-1);
    calculator.add(0);
    calculator.add(+1);

    EXPECT_EQ(3, calculator.count());
    EXPECT_EQ(-1.F, calculator.minimum());
    EXPECT_EQ(+1.F, calculator.maximum());
    EXPECT_EQ(0.F, calculator.mean());
    EXPECT_EQ(2.F / 3, calculator.absolute_mean());

    double mean_square      = (-1.0 * -1.0 + 0.0 * 0.0 + 1.0 * 1.0) / 3;
    double root_mean_square = sqrt(mean_square);
    EXPECT_FLOAT_EQ(root_mean_square, calculator.quadratic_mean());

    double variance           = (pow(-1.0 - 0.0, 2) + pow(0.0 - 0.0, 2) + pow(1.0 - 0.0, 2)) / 3;
    double standard_deviation = sqrt(variance);
    EXPECT_FLOAT_EQ(standard_deviation, calculator.standard_deviation());
}

TEST(StatisticsCalculator, WorksAtMinimumBoundary)
{
    stats::StatisticsCalculator calculator;

    calculator.add(-std::numeric_limits<float>::max());

    EXPECT_EQ(-std::numeric_limits<float>::max(), calculator.minimum());
    EXPECT_EQ(-std::numeric_limits<float>::max(), calculator.maximum());
}

TEST(StatisticsCalculator, WorksAtMaximumBoundary)
{
    stats::StatisticsCalculator calculator;

    calculator.add(std::numeric_limits<float>::max());

    EXPECT_EQ(std::numeric_limits<float>::max(), calculator.minimum());
    EXPECT_EQ(std::numeric_limits<float>::max(), calculator.maximum());
}

TEST(StatisticsCalculator, AgreesWithDocumentedExample)
{
    stats::StatisticsCalculator calculator;

    for (const float& value : documented_test_set::values())
    {
        calculator.add(value);
    }

    EXPECT_EQ(documented_test_set::count(), calculator.count());
    EXPECT_EQ(documented_test_set::minimum(), calculator.minimum());
    EXPECT_EQ(documented_test_set::maximum(), calculator.maximum());
    EXPECT_EQ(documented_test_set::mean(), calculator.mean());
    EXPECT_EQ(documented_test_set::absolute_mean(), calculator.absolute_mean());
    EXPECT_EQ(documented_test_set::quadratic_mean(), calculator.quadratic_mean());
    EXPECT_EQ(documented_test_set::standard_deviation(), calculator.standard_deviation());
    EXPECT_EQ(documented_test_set::skewness(), calculator.skewness());
    EXPECT_EQ(documented_test_set::kurtosis(), calculator.kurtosis());
}

TEST(StatisticsCalculator, HandlesSelfAssignment)
{
    stats::StatisticsCalculator calculator;

    const stats::StatisticsCalculator& calculator2(calculator);

    calculator = calculator2;

    EXPECT_EQ(&calculator, &calculator2);
}

TEST(StatisticsCalculator, CombinesResultsFromEmptyCalculators)
{
    stats::StatisticsCalculator fullset;
    stats::StatisticsCalculator subset1, subset2, subset3;

    stats::StatisticsCalculator combined = subset1 + subset2 + subset3;

    test_equivalence(fullset, combined);
}

TEST(StatisticsCalculator, CombinesResultsFromSomeEmptyCalculators)
{
    stats::StatisticsCalculator fullset;

    stats::StatisticsCalculator subset1;

    stats::StatisticsCalculator subset2;
    subset2.add(1.F);
    fullset.add(1.F);
    subset2.add(1.5F);
    fullset.add(1.5F);
    subset2.add(2.F);
    fullset.add(2.F);

    stats::StatisticsCalculator subset3;

    stats::StatisticsCalculator subset4;
    subset2.add(3.F);
    fullset.add(3.F);
    subset2.add(4.F);
    fullset.add(4.F);

    stats::StatisticsCalculator combined = subset1 + subset2 + subset3 + subset4;

    test_equivalence(fullset, combined);
}

TEST(StatisticsCalculator, CombinesResultsFromSingleValueCalculators)
{
    stats::StatisticsCalculator fullset;

    stats::StatisticsCalculator calculator1;
    calculator1.add(1.F);
    fullset.add(1.F);

    stats::StatisticsCalculator calculator2;
    calculator2.add(2.F);
    fullset.add(2.F);

    stats::StatisticsCalculator combined = calculator1 + calculator2;

    test_equivalence(fullset, combined);
}

TEST(StatisticsCalculator, CombinesResultsFromMultipleCalculators)
{
    stats::StatisticsCalculator fullset;
    stats::StatisticsCalculator subset1;
    stats::StatisticsCalculator subset2;

    int count = 0;
    for (const float& value : documented_test_set::values())
    {
        fullset.add(value);
        if ((++count % 3) == 0)
        {
            subset1.add(value);
        }
        else
        {
            subset2.add(value);
        }
    }

    stats::StatisticsCalculator combined1 = subset1 + subset2;
    test_equivalence(fullset, combined1);

    stats::StatisticsCalculator combined2 = subset2 + subset1;
    test_equivalence(fullset, combined2);
}

TEST(StatisticsCalculator, IncrementsResultsFromAnotherCalculator)
{
    stats::StatisticsCalculator fullset;
    stats::StatisticsCalculator subset1;
    stats::StatisticsCalculator subset2;

    int count = 0;
    for (const float& value : documented_test_set::values())
    {
        fullset.add(value);
        if ((++count % 3) == 0)
        {
            subset1.add(value);
        }
        else
        {
            subset2.add(value);
        }
    }

    stats::StatisticsCalculator combined;
    combined += subset1;
    combined += subset2;

    test_equivalence(fullset, combined);
}
