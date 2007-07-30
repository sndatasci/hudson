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
#include "SeriesFactor.hpp"
#include "SeriesFactorSet.hpp"


class PositionFactors
{
  public:
    PositionFactors(const Position& pos, const Series::DaySeries<Series::DayPrice>& db);

    double avg(void) const;
    double stddev(void) const;

    const SeriesFactor& best(void) const;
    const SeriesFactor& worst(void) const;

    SeriesFactorSet max_cons_pos(void) const;
    SeriesFactorSet max_cons_neg(void) const;

    unsigned num_pos(void) const;
    unsigned num_neg(void) const;

    SeriesFactorSet pk(void) const;
    SeriesFactorSet dd(void) const;

  private:
    struct variance_bf: public std::binary_function<double, double, double>
    {
      variance_bf(double mean): _mean(mean) { }

      // accumulate() doesn't accumulate when using a custom binary_function...
      double operator()(double x, double y) { return x + ::pow(y - _mean, 2); }

    private:
      double _mean;
    };

    struct SeriesFactorLtCmp: public std::binary_function<SeriesFactor, SeriesFactor, bool>
    {
      bool operator()(const SeriesFactor& sf1, const SeriesFactor& sf2) const { return sf1.factor() < sf2.factor(); }
    };  

    struct SeriesFactorSetSizeCmp: public std::binary_function<SeriesFactorSet, SeriesFactorSet, bool>
    {
      bool operator()(const SeriesFactorSet& set1, const SeriesFactorSet& set2) const { return set1.size() < set2.size(); }
    };

    struct SeriesFactorSetFactorCmp: public std::binary_function<SeriesFactorSet, SeriesFactorSet, bool>
    {
      bool operator()(const SeriesFactorSet& set1, const SeriesFactorSet& set2) const { return set1.factor() < set2.factor(); }
    };

    SeriesFactorSet _dd(series_factor_by_end_mark::const_iterator& start) const;
    SeriesFactorSet _pk(series_factor_by_end_mark::const_iterator& start) const;

  private:
    const Position& _pos;
    const Series::DaySeries<Series::DayPrice>& _db;
    SeriesFactorSet _sfs;

    typedef std::vector<double> doubleVector;
    doubleVector _vFactors; // time-ordered position factors for fast array calculations

    double _mean;
    double _stddev;
};


#endif // _POSITIONFACTORS_HPP_
