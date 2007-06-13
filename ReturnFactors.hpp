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


class ReturnFactorsException: public std::exception
{
public:
  ReturnFactorsException(const std::string& msg):
    _Str("ReturnFactorsException: ")
  {
    _Str += msg;
  }

  virtual ~ReturnFactorsException(void) { }
  virtual const char *what() const { return _Str.c_str(); }

protected:
  std::string _Str;
};


class ReturnFactors
{
public:
  ReturnFactors(const PositionSet& sPositions, unsigned days, unsigned yperiods) throw(ReturnFactorsException);
  ~ReturnFactors(void);

  unsigned days(void) const { return _days; }
  unsigned yperiods(void) const { return _yperiods; }

  double roi(void) const;
  double avg(void) const;
  double dd(void) const;
  double stddev(void) const;
  double skew(void) const;
  double cagr(void) const;
  double gsd(void) const;
  const Position& best(void) const;
  const Position& worst(void) const;
  size_t num(void) const;
  PositionSet max_cons_pos(void) const; // max consecutive positive
  PositionSet max_cons_neg(void) const; // max consecutive negative

  PositionSet pos(void) const;
  PositionSet neg(void) const;

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

  struct PositionGt: public std::binary_function<Position*, double, bool> {

    bool operator()(Position* pPos, double x) const { return pPos->factor() > x; }
  };

  struct PositionLt: public std::binary_function<Position*, double, bool> {

    bool operator()(Position* pPos, double x) const { return pPos->factor() < x; }
  };

  struct PositionLtCmp: public std::binary_function<Position*, Position*, bool> {

    bool operator()(const Position* pos1, const Position* pos2) const { return pos1->factor() < pos2->factor(); }
  };

  struct PositionSetSizeCmp: public std::binary_function<PositionSet, PositionSet, bool> {

    bool operator()(const PositionSet& pset1, const PositionSet& pset2) const { return pset1.size() < pset2.size(); }
  };

  double _dd(int i) const;

private:
  PositionSet _sPositions;

  typedef std::vector<double> doubleVector;
  doubleVector _vFactors; // time-ordered position factors
  doubleVector _vLogFactors; // time-ordered position log factors

  unsigned _days;			// time in days
  unsigned _yperiods; // yearly factors (12 for monthly)
  double _years;			// total time in years
  double _fvalue;			// future value
  double _mean;				// factors average
  double _stddev;			// factors standard deviation
};

#endif // _RETURNFACTORS_HPP_

