#pragma once

#include <cstddef>
#include <string>

namespace stats
{
namespace detail
{

std::string count_description(const std::size_t& count);

std::string label_and_value(const std::string& label, const float& value);

const std::string kValueLabel(" Value = ");

const std::string kMinimumLabel(" Minimum  = ");
const std::string kMaximumLabel(" Maximum  = ");
const std::string kMedianLabel(" Median   = ");
const std::string kMeanLabel(" Mean     = ");
const std::string kAbsMeanLabel(" Abs.Mean = ");
const std::string kRmsLabel(" Rms      = ");
const std::string kStdDevnLabel(" Std.Devn = ");
const std::string kSkewnessLabel(" Skewness = ");
const std::string kKurtosisLabel(" Kurtosis = ");

} // namespace detail
} // namespace stats
