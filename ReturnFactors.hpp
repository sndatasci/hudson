/*
 * ReturnFactors.hpp
 */

#ifndef _RETURNFACTORS_HPP_
#define _RETURNFACTORS_HPP_

#ifdef WIN32
#pragma warning (disable:4290)
#endif

// STD
#include <cmath>

// STL
#include <vector>
#include <numeric>
#include <functional>

// Boost
#include <boost/date_time/gregorian/gregorian.hpp>

// Hudson
#include "PositionSet.hpp"
#include "EODSeries.hpp"


class ReturnFactorsException: public std::exception
{
public:
  ReturnFactorsException(const std::string& msg):
    _Str("ReturnFactorsException: ")
  {
    _Str += msg;
  }

  virtual ~ReturnFactorsException(void) throw() { }
  virtual const char *what(void) const throw() { return _Str.c_str(); }

protected:
  std::string _Str;
};


class ReturnFactors
{
public:
  ReturnFactors(const PositionSet& sPositions, const Series::EODSeries& db);

  const Series::EODSeries& db(void) const { return _db; }

  unsigned days(void) const { return _days; }
  unsigned yperiods(void) const { return _yperiods; }

  int num(void) const;

  double roi(void) const;
  double avg(void) const;
  double stddev(void) const;
  double skew(void) const;
  double cagr(void) const;

  PositionSet pos(void) const;
  PositionSet neg(void) const;

  const Position& best(void) const throw(ReturnFactorsException);
  const Position& worst(void) const throw(ReturnFactorsException);

  PositionSet max_cons_pos(void) const throw(ReturnFactorsException);
  PositionSet max_cons_neg(void) const throw(ReturnFactorsException);

  PositionSet dd(void) const throw(ReturnFactorsException);

protected:
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

  struct PositionGt: public std::binary_function<PositionPtr, double, bool> {

    PositionGt(double curr_price): _curr_price(curr_price) { }

    bool operator()(PositionPtr pPos, double x) const { return pfactor(*pPos, _curr_price) > x; }

  private:
    double _curr_price;
  };

  struct PositionLt: public std::binary_function<PositionPtr, double, bool> {

    PositionLt(double curr_price): _curr_price(curr_price) { }

    bool operator()(PositionPtr pPos, double x) const { return pfactor(*pPos, _curr_price) < x; }

  private:
    double _curr_price;
  };

  struct PositionLtCmp: public std::binary_function<PositionPtr, PositionPtr, bool> {

    PositionLtCmp(double curr_price): _curr_price(curr_price) { }

    bool operator()(const PositionPtr pos1, const PositionPtr pos2) const { return pfactor(*pos1, _curr_price) < pfactor(*pos2, _curr_price); }

  private:
    double _curr_price;
  };

  struct PositionSetSizeCmp: public std::binary_function<PositionSet, PositionSet, bool> {

    bool operator()(const PositionSet& pset1, const PositionSet& pset2) const { return pset1.size() < pset2.size(); }
  };

  struct PositionSetRealizedCmp: public std::binary_function<PositionSet, PositionSet, bool> {

    bool operator()(const PositionSet& pset1, const PositionSet& pset2) const { return pset1.realized() < pset2.realized(); }
  };

  PositionSet _dd(PositionSet::by_last_exec::iterator& start) const;

protected:
  PositionSet _sPositions;
  const Series::EODSeries& _db;

  PositionSet _sClosedPositions;
  PositionSet _sOpenPositions;

  double _last_close;
  double _last_open;

  typedef std::vector<double> doubleVector;
  doubleVector _vFactors; // time-ordered position factors for fast array calculations

  unsigned _days;			// time in days
  double   _years;		// time in years + fraction
  const unsigned _yperiods; // yearly factors

  double _fvalue;			// future value
  double _mean;				// factors average
  double _stddev;			// factors standard deviation
};

#endif // _RETURNFACTORS_HPP_
