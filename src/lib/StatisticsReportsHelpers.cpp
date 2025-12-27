#include "StatisticsReportsHelpers.hpp"

#include <sstream>

namespace stats
{
namespace detail
{

std::string count_description(const std::size_t& count)
{
    std::ostringstream oss;

    if (count == 0)
    {
        oss << "No Values";
    }
    else if (count == 1)
    {
        oss << "1 Value";
    }
    else
    {
        oss << count << " Values";
    }

    return oss.str();
}

std::string label_and_value(const std::string& label, const float& value)
{
    std::ostringstream oss;
    oss.precision(6);

    oss << label << value;

    return oss.str();
}

} // namespace detail
} // namespace stats
