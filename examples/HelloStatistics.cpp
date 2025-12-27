// HelloStatistics is a simple statistics application.
// It produces the following output.
//
// 3 Values
//  Minimum  = 1
//  Maximum  = 3
//  Mean     = 2
//  Abs.Mean = 2
//  Rms      = 2.16025
//  Std.Devn = 0.816497
//  Skewness = 0
//  Kurtosis = -1.5

#include <iostream>
#include <stats/StatisticsCalculator.hpp>
#include <stats/StatisticsReport.hpp>

int main(int /*unused*/, char** /*unused*/)
{
    stats::StatisticsCalculator calculator;

    calculator.add(1.0);
    calculator.add(2.0);
    calculator.add(3.0);

    std::cout << stats::description(calculator) << std::endl;
}
