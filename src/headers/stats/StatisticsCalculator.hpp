#pragma once

#include <cstddef>

namespace stats
{

/**
 * Takes one value at a time, providing accumulated descriptive statistics.
 *
 * StatisticsCalculator accepts 32-bit floating point values with add().
 * It provides measures of count, minimum, maximum, mean, absolute mean,
 * quadratic mean (rms), standard deviation, skewness, and kurtosis.
 *
 * Use the calculator with code like the following.

 \code
 #include <stats/StatisticsCalculator.h>


 // create a statistics calculator

 StatisticsCalculator calculator;


 // add some values

 calculator.add( -2.0 );
 calculator.add(  0.0 );
 calculator.add( +2.0 );
 calculator.add( +4.0 );


 // access the measures

 size_t n = calculator.count(); // sets n to 4
 float  u = calculator.mean();  // sets u to 1.0;
 \endcode

 * Be sure to watch for undefined measures.

 \code
 if( stats::undefined( calculator.mean() ) ){
     // no values -- ignore the undefined mean
 }
 if( stats::undefined( calculator.skewness() ) ){
     // all values are the same -- ignore the undefined skewness
 }
 \endcode

 * The calculator does not store the values. It allows statistics of
 * larga data.
 *
 * The statistics algorithm is stable. Rounding errors accumulate slowly.
 * Stress tests find 32-bit floating point deviations for large data:
 *
 * - standard deviation: approximately 134 billion values
 * - skewness: approximately 29 billion values
 * - kurtosis: approximately 2.6 billion values
 *
 * \sa
 * <a href="http://www.johndcook.com/blog/skewness_kurtosis/">
 * Computing skewness and kurtosis in one pass.
 * </a>
 * This class includes John D. Cook's skewness/kurtosis extension of the
 * method of Knuth and Welford for computing standard deviation in one pass
 * through the data.
 */

class StatisticsCalculator
{
  private:
    std::size_t count_;
    float minimum_, maximum_;
    double moment1_, abs_moment1_, moment2_, moment3_, moment4_;

  public:
    StatisticsCalculator();

    /**
     * Updates the calculator with the value.
     */
    void add(const float& value);

    /**
     * Returns the total number of values provided with add().
     */
    std::size_t count() const;

    /**
     * Returns the minimum of the values provided with add().
     */
    float minimum() const;

    /**
     * Returns the maximum of the values provided with add().
     */
    float maximum() const;

    /**
     * Returns the arithmetic mean of the values provided with add().
     */
    float mean() const;

    /**
     * Returns the mean of the absolute values provided with add().
     */
    float absolute_mean() const;

    /**
     * Returns the quadratic mean (rms) of the values provided with add().
     */
    float quadratic_mean() const;

    /**
     * Returns the standard deviation of the values provided with add().
     */
    float standard_deviation() const;

    /**
     * Returns the skewness of the values provided with add().
     *
     * The normal distribution's skewness is zero.
     */
    float skewness() const;

    /**
     * Returns the kurtosis of the values provided with add().
     *
     * The measure is technically "excess kurtosis", for which the normal
     * distribution is zero.
     */
    float kurtosis() const;

    /**
     * "Adds" calculators, aggregating the results.
     *
     * Use code like the following to combine calculators.

     \code
     #include <stats/StatisticsCalculator.h>


     // make three calculators

     StatisticsCalculator calculator1, calculator2, calculator3;


     // add values to each calculator

     ...


     // and combine the results

     StatisticsCalculator combined = calculator1 + calculator2 + calculator3;
     \endcode

     */
    StatisticsCalculator operator+(const StatisticsCalculator& that) const;

    /**
     * "Adds" the specified calculator to this one, aggregating the results.
     */
    StatisticsCalculator& operator+=(const StatisticsCalculator& rhs);
};

} // namespace stats
