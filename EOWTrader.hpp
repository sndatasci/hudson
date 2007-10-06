/*
 * EOWTrader.hpp
 */

#ifndef _EOWTRADER_HPP_
#define _EOWTRADER_HPP_

// STL
#include <vector>

// Boost
#include <boost/date_time/gregorian/gregorian.hpp>

// Hudson
#include "EODSeries.hpp"
#include "Trader.hpp"


class EOWTrader: public Trader
{
  typedef Series::EODSeries DB;

public:
  EOWTrader(const DB& db);

  void run(unsigned entry_offset, char entry_oc, unsigned exit_offset, char exit_oc) throw(TraderException);

  boost::gregorian::date first_entry(void) { return _first_entry; }
  boost::gregorian::date last_exit(void) { return _last_exit; }
  boost::gregorian::days invested_days(void) { return _invested_days; }

private:
  const DB& _db;

  boost::gregorian::date _first_entry;
  boost::gregorian::date _last_exit;
  boost::gregorian::days _invested_days;
};

#endif // _EOWTRADER_HPP_
