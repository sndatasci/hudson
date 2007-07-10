/*
* PositionFactors.hpp
*/

#ifndef _POSITIONFACTORS_HPP_
#define _POSITIONFACTORS_HPP_

#ifdef WIN32
#pragma warning (disable:4290)
#endif

// CSTD
#include <cmath>
// STL
#include <stdexcept>

// Hudson
#include "DaySeries.hpp"
#include "DayPrice.hpp"
#include "Position.hpp"


class PositionFactors
{
  public:
    PositionFactors(const Position& pos, const Series::DaySeries<Series::DayPrice>& db);

    double avg(void) const;
    double stddev(void) const;

    double best(void) const;
    double worst(void) const;

    int max_cons_pos(void) const;
    int max_cons_neg(void) const;

    unsigned num_pos(void) const;
    unsigned num_neg(void) const;

    double pk(void) const;
    double dd(void) const;

  private:
    struct variance_bf : public std::binary_function<double, double, double> {
      variance_bf(double mean): _mean(mean) { }

      // accumulate() doesn't accumulate when using a custom binary_function...
      double operator()(double x, double y) { return x + ::pow(y - _mean, 2); }

    private:
      double _mean;
    };

    double _dd(unsigned start) const;
    double _pk(unsigned start) const;

  private:
    const Position& _pos;
    const Series::DaySeries<Series::DayPrice>& _db;

    typedef std::vector<double> doubleVector;
    doubleVector _vFactors; // time-ordered position factors for fast array calculations

    double _mean;
    double _stddev;
};


#endif // _POSITIONFACTORS_HPP_
