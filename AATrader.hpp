/*
 * AATrader.hpp
 */

#ifndef _AATRADER_HPP_
#define _AATRADER_HPP_

// STL
#include <vector>

// Boost
#include <boost/date_time/gregorian/gregorian.hpp>

// Hudson
#include "EODSeries.hpp"
#include "Trader.hpp"
#include "TA.hpp"


class AATrader: public Trader
{
  typedef Series::EODSeries DB;

public:
  AATrader(const DB& spx_db, const DB& tnx_db, const DB& djc_db, const DB& eafe_db, const DB& reit_db);

  void run(void) throw(TraderException);

private:
  void check_buy(const DB& db, DB::const_iterator& iter, const TA::MACDRes& macd, int i);
  void check_sell(const DB& db, DB::const_iterator& iter, const TA::MACDRes& macd, int i);

private:
  const DB& _spx_db;
  const DB& _tnx_db;
  const DB& _djc_db;
  const DB& _eafe_db;
  const DB& _reit_db;
};

#endif // _AATRADER_HPP_
