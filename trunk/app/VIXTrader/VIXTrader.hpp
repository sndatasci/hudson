/*
* Copyright (C) 2007, Alberto Giannetti
*
* This file is part of Hudson.
*
* Hudson is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* Hudson is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with Hudson.  If not, see <http://www.gnu.org/licenses/>.
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
