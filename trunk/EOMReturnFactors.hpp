/*
* EOMReturnFactors.hpp
*/

#ifndef _EOMRETURNFACTORS_HPP_
#define _EOMRETURNFACTORS_HPP_

#ifdef WIN32
#pragma warning (disable:4290)
#endif

// STL
#include <vector>

// Hudson
#include "ReturnFactors.hpp"
#include "PositionSet.hpp"


class EOMReturnFactors: public ReturnFactors
{
public:
  EOMReturnFactors(const PositionSet& sPositions, const Series::EODSeries& db);

  double gsd(void) const;
  double sharpe(double rf_rate = 5.0) const;

private:
  void _calculateM2M(void);
  double _monthlyFactor(Series::EODSeries::const_iterator prev_em_mark, Series::EODSeries::const_iterator em_mark, const PositionPtr pos);

private:
  const Series::EODSeries _monthly_db;
  std::vector<double> _vMFactors; // monthly factors
  std::vector<double> _vLogMFactors; // monthly log factors
};


#endif // _EOMRETURNFACTORS_HPP_
