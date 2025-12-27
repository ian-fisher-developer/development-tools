#include "stats/StatisticsReport.hpp"

#include <sstream>

#include "StatisticsReportsHelpers.hpp"
#include "stats/StatisticsCalculator.hpp"
#include "stats/StatisticsUtilities.hpp"

namespace stats
{

std::string description(const stats::StatisticsCalculator& calculator)
{
    using namespace stats::detail;

    std::ostringstream oss;

    oss << count_description(calculator.count());

    if (calculator.count() == 0)
    {
        return oss.str();
    }

    if (calculator.count() == 1)
    {
        oss << std::endl << label_and_value(kValueLabel, calculator.minimum());
    }
    else
    {
        oss << std::endl << label_and_value(kMinimumLabel, calculator.minimum());
        oss << std::endl << label_and_value(kMaximumLabel, calculator.maximum());
        oss << std::endl << label_and_value(kMeanLabel, calculator.mean());
        oss << std::endl << label_and_value(kAbsMeanLabel, calculator.absolute_mean());
        oss << std::endl << label_and_value(kRmsLabel, calculator.quadratic_mean());
        oss << std::endl << label_and_value(kStdDevnLabel, calculator.standard_deviation());
        if (!stats::undefined(calculator.skewness()))
        {
            oss << std::endl << label_and_value(kSkewnessLabel, calculator.skewness());
        }
        if (!stats::undefined(calculator.kurtosis()))
        {
            oss << std::endl << label_and_value(kKurtosisLabel, calculator.kurtosis());
        }
    }

    return oss.str();
}

} // namespace stats
