/*
 * BnHTrader.hpp
 */

#ifndef _BNHTRADER_HPP_
#define _BNHTRADER_HPP_

// STL
#include <vector>

#include "DayPrice.hpp"
#include "DaySeries.hpp"
#include "Trader.hpp"


class BnHTrader: public Trader
{
  typedef Series::DaySeries<Series::DayPrice> DB;

public:
  BnHTrader(const DB& db);

  void run(void);

  std::vector<double> factors(void) const { return _vf; }

private:
  const DB& _db;

  std::vector<double> _vf;
};

#endif // _BNHTRADER_HPP_
