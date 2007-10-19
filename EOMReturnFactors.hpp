/*
* EOMReturnFactors.hpp
*/

#ifndef _EOMRETURNFACTORS_HPP_
#define _EOMRETURNFACTORS_HPP_

#ifdef WIN32
#pragma warning (disable:4290)
#endif

// STD
#include <cmath>

// STL
#include <vector>
#include <functional>

// Boost
#include <boost/date_time/gregorian/gregorian.hpp>

// Hudson
#include "ReturnFactors.hpp"
#include "PositionSet.hpp"
#include "EODSeries.hpp"


class EOMReturnFactors: public ReturnFactors
{
public:
  EOMReturnFactors(const PositionSet& sPositions, const Series::EODSeries& db, double rf_rate = 3.0);
  
  double cagr(void) const;
  double gsd(void) const;
  double sharpe(void) const;

protected:
  void _calculateM2M(void);
  double _monthlyFactor(Series::EODSeries::const_iterator prev_em_mark, Series::EODSeries::const_iterator em_mark, const PositionPtr pos);

protected:
  const Series::EODSeries& _db;
  const Series::EODSeries _monthly_db;
  const double _rf_rate;

  std::vector<double> _vMFactors; // monthly factors
  std::vector<double> _vLogMFactors; // monthly log factors

  typedef std::map<boost::gregorian::date, double> DATEMFACTOR;
  DATEMFACTOR _mDateMFactors;

  double _mmean;
  double _mstddev;
  double _msharpe;
  
  unsigned _days;
  double   _years;

protected:
  struct log10_uf: public std::unary_function<double, double> {
    double operator()(double x) { return ::log10(x); }
  };
};


#endif // _EOMRETURNFACTORS_HPP_
