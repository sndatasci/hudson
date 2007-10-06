/*
 * VIXTrader.hpp
 */

#ifndef _VIXTRADER_HPP_
#define _VIXTRADER_HPP_

// STL
#include <vector>

// Boost
#include <boost/date_time/gregorian/gregorian.hpp>

// Hudson
#include "EODSeries.hpp"
#include "Trader.hpp"


class VIXTrader: public Trader
{
  typedef Series::EODSeries DB;

public:
  VIXTrader(const DB& spxdb, const DB& vixdb);

  void run(void) throw(TraderException);

  boost::gregorian::days invested_days(void) { return _invested_days; }

private:
  const DB& _db;
  const DB& _vixdb;

  boost::gregorian::date _first_entry;
  boost::gregorian::date _last_exit;
  boost::gregorian::days _invested_days;
};

#endif // _VIXTRADER_HPP_
