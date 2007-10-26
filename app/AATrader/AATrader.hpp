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

//! Asset Allocator Trader.
/*!
  AATrader enters trades in 5 different asset classes to optimize risk-adjusted returns. SP500, US Bonds,
  Commodities, EAFE and REIT are the databases currently supported.
*/
class AATrader: public Trader
{
  typedef Series::EODSeries DB;

public:
  /*!
    The constructor gets historical data series for different asset classes.
    \param spx_db SP500 historical data
    \param tnx_db US Bonds historical data
    \param djc_db Commodities historical data
    \param eafe_db EAFE historical data
    \param reit_db REIT historical data
  */
  AATrader(const DB& spx_db, const DB& tnx_db, const DB& djc_db, const DB& eafe_db, const DB& reit_db);

  //! Run the trader.
  void run(void) throw(TraderException);

private:
  /*!
    Run trading loop over select calendar period. Called for each asset class.
    \param db historical data
    \param iter begin of trading period
    \param sma simple moving average series
  */
  void trade(const DB& db, DB::const_iterator& iter, const TA::SMARes& sma);
  /*!
    Execute the buy strategy
    \param db historical data
    \param iter begin of trading period
    \param sma Simple moving average series
    \param i begin of SMA series matching iter
  */
  void check_buy(const DB& db, DB::const_iterator& iter, const TA::SMARes& sma, int i);
  /*!
    Execute the sell strategy
    \param db historical data
    \param iter begin of trading period
    \param sma Simple moving average series
    \param i begin of SMA series matching iter   
  */  
  void check_sell(const DB& db, DB::const_iterator& iter, const TA::SMARes& sma, int i);

private:
  const DB& _spx_db;
  const DB& _tnx_db;
  const DB& _djc_db;
  const DB& _eafe_db;
  const DB& _reit_db;
};

#endif // _AATRADER_HPP_
