/*
 * ReturnFactors.hpp
 */

#ifndef _RETURNFACTORS_HPP_
#define _RETURNFACTORS_HPP_

#ifdef WIN32
#pragma warning (disable:4290)
#endif

// STDLIB
#include <cmath>

// STL
#include <vector>
#include <numeric>
#include <functional>

// Hudson
#include "PositionSet.hpp"


class ReturnFactors
{
public:
  ReturnFactors(const PositionSet& sPositions, unsigned days, unsigned yperiods);

  unsigned days(void) const { return _days; }
  unsigned yperiods(void) const { return _yperiods; }

  int num(void) const;

  double roi(void) const;
  double avg(void) const;
  double stddev(void) const;
  double skew(void) const;
  double cagr(void) const;
  double gsd(void) const;

  const Position& best(void) const;
  const Position& worst(void) const;

  PositionSet max_cons_pos(void) const; // max consecutive positive
  PositionSet max_cons_neg(void) const; // max consecutive negative

  PositionSet pos(void) const;
  PositionSet neg(void) const;

  PositionSet dd(void) const;

private:
  struct variance_bf : public std::binary_function<double, double, double> {
    variance_bf(double mean): _mean(mean) { }

    // accumulate() doesn't accumulate when using a custom binary_function...
    double operator()(double x, double y) { return x + ::pow(y - _mean, 2); }

  private:
    double _mean;
  };

  struct skew_bf: public std::binary_function<double, double, double> {
    skew_bf(double mean): _mean(mean) { }

    double operator()(double x, double y) { return x + ::pow(y - _mean, 3); }

  private:
    double _mean;
  };

  struct log10_uf: public std::unary_function<double, double> {

    double operator()(double x) { return ::log10(x); }
  };

  struct PositionGt: public std::binary_function<PositionPtr, double, bool> {

    bool operator()(PositionPtr pPos, double x) const { return pPos->factor() > x; }
  };

  struct PositionLt: public std::binary_function<PositionPtr, double, bool> {

    bool operator()(PositionPtr pPos, double x) const { return pPos->factor() < x; }
  };

  struct PositionLtCmp: public std::binary_function<PositionPtr, PositionPtr, bool> {

    bool operator()(const PositionPtr pos1, const PositionPtr pos2) const { return pos1->factor() < pos2->factor(); }
  };

  struct PositionSetSizeCmp: public std::binary_function<PositionSet, PositionSet, bool> {

    bool operator()(const PositionSet& pset1, const PositionSet& pset2) const { return pset1.size() < pset2.size(); }
  };

  struct PositionSetRealizedCmp: public std::binary_function<PositionSet, PositionSet, bool> {

    bool operator()(const PositionSet& pset1, const PositionSet& pset2) const { return pset1.realized() < pset2.realized(); }
  };

  PositionSet _dd(position_by_last_exec::iterator& start) const;

private:
  PositionSet _sPositions;

  typedef std::vector<double> doubleVector;
  doubleVector _vFactors; // time-ordered position factors for fast array calculations
  doubleVector _vLogFactors; // time-ordered position log factors

  unsigned _days;			// time in days
  unsigned _yperiods; // yearly factors (12 for monthly)
  double _years;			// total time in years

  double _fvalue;			// future value
  double _mean;				// factors average
  double _stddev;			// factors standard deviation
};

#endif // _RETURNFACTORS_HPP_

