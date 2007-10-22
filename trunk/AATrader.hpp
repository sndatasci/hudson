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
  void check_buy(const DB& db, DB::const_iterator& iter, const TA::SMARes& sma, int i);
  void check_sell(const DB& db, DB::const_iterator& iter, const TA::SMARes& sma, int i);
  void trade(const DB& db, DB::const_iterator& iter, const TA::SMARes& sma);

private:
  const DB& _spx_db;
  const DB& _tnx_db;
  const DB& _djc_db;
  const DB& _eafe_db;
  const DB& _reit_db;
};

#endif // _AATRADER_HPP_
