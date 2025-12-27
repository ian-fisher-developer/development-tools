#pragma once

#include <cmath>
#include <string>
#include <vector>

// Test example from http://brownmath.com/stat/shape.htm

namespace
{
inline void append(std::vector<float>& values, size_t n, float value)
{
    std::vector<float> more_values(n, value);
    values.insert(values.end(), more_values.begin(), more_values.end());
}
} // namespace

namespace documented_test_set
{

inline const std::vector<float>& values()
{
    static std::vector<float> values;
    if (values.empty())
    {
        append(values, 18, 64.F);
        append(values, 42, 67.F);
        append(values, 5, 61.F);
        append(values, 8, 73.F);
        append(values, 27, 70.F);
    }
    return values;
}

inline size_t count()
{
    return 100;
}

inline float minimum()
{
    return 61.F;
}

inline float maximum()
{
    return 73.F;
}

inline float mean()
{
    return 67.45F;
}

inline float absolute_mean()
{
    return 67.45F;
}

inline float quadratic_mean()
{
    double rms = sqrt(4558.03);
    return (float)rms;
}

inline float median()
{
    return 67.F;
}

inline float standard_deviation()
{
    double std_dev = sqrt(8.5275);
    return (float)std_dev;
}

inline float skewness()
{
    double skew = -0.10815437112299035;
    return (float)skew;
}

inline float kurtosis()
{
    double kurt = -0.25824103146037514;
    return (float)kurt;
}

inline std::string range_description()
{
    return "100 Values\n Minimum  = 61\n Maximum  = 73";
}

inline std::string rms_description()
{
    return "100 Values\n Rms      = 67.5132";
}

inline std::string variance_description()
{
    return "100 Values\n Mean     = 67.45\n Abs.Mean = 67.45\n Rms      = 67.5132\n Std.Devn = "
           "2.92019";
}

inline std::string statistics_description()
{
    return "100 Values\n Minimum  = 61\n Maximum  = 73\n Mean     = 67.45\n Abs.Mean = 67.45\n Rms "
           "     = 67.5132\n Std.Devn = 2.92019\n Skewness = -0.108154\n Kurtosis = -0.258241";
}

inline std::string median_description()
{
    return " Median   = 67";
}

} // namespace documented_test_set
