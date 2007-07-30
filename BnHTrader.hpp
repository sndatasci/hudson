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
  typedef Series::DaySeries DB;

public:
  BnHTrader(const DB& db);

  void run(void);

private:
  const DB& _db;
};

#endif // _BNHTRADER_HPP_
