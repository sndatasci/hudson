/*
 * JanTrader.hpp
 */

#ifndef _JANTRADER_HPP_
#define _JANTRADER_HPP_

// STL
#include <vector>

// Boost
#include <boost/date_time/gregorian/gregorian.hpp>

#include "EODSeries.hpp"
#include "Trader.hpp"


class JanTrader: public Trader
{
  typedef Series::EODSeries DB;

public:
  JanTrader(const DB& longdb,  const DB& hedgedb);

  void run(int entry_offset, int exit_offset) throw(TraderException);
  std::vector<double> factors(void) const { return _vf; }
  boost::gregorian::days invested_days(void) { return _vested_days; }

private:
  const DB& _longdb;
  const DB& _hedgedb;

  std::vector<double> _vf;

  boost::gregorian::days _vested_days;
};


#endif // _JANTRADER_HPP_
