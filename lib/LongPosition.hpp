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

#ifndef _LONGPOSITION_HPP_
#define _LONGPOSITION_HPP_

#ifdef WIN32
#pragma warning (disable:4290)
#endif

// Hudson
#include "Position.hpp"

class Price;

//! A new long position.
/*!
  LongPosition automatically generates a new buy Execution for a specific symbol/price/size. It runs a number of consistency checks to
  validate the price, date, and the specified execution ID. Only buy() and sell() transactions can be executed, other transaction types
  will generate an exception.
*/
class LongPosition: public Position
{
public:
  /*!
    All Execution parameters passed to the constructor are relative to the opening Execution.
      
    \param id A unique Execution ID.
    \param symbol Security for this position.
    \param dt Execution date.
    \param price Execution price.
    \param size Execution size.
    \see Execution.
  */
  LongPosition(ID id, const std::string& symbol, const boost::gregorian::date& dt, const Price& price, unsigned size) throw(PositionException);

  virtual Type type(void) const { return LONG; }
  virtual std::string type_str(void) const { return "Long"; }

  //! The number of buy executions run on this position.
  unsigned buys(void) const { return _buys; }
  //! The number of sell executions run on this position.
  unsigned sells(void) const { return _sells; }

  //! Average buy price. 
  virtual double avgEntryPrice(void) const { return _avgBuyPrice; }
  //! Average sell price.
  virtual double avgExitPrice(void) const { return _avgSellPrice; }

  //! Current return factor: average sell price divided by average buy price.
  virtual double factor(void) const throw(PositionException);
  //! Return factor calculated dividing the price parameter into the avgEntryPrice().
  virtual double factor(const Price& price) const throw(PositionException);
  //! XXX should be a static: curr_price / prev_price.
  virtual double factor(const Price& prev_price, const Price& curr_price) const;

  //! Add buy Execution.
  virtual void buy(const boost::gregorian::date& dt, const Price& price, unsigned size) throw(PositionException);
  //! Add sell Execution.
  virtual void sell(const boost::gregorian::date& dt, const Price& price, unsigned size) throw(PositionException);
  //! Virtual implementation. Will throw an exception.
  virtual void sell_short(const boost::gregorian::date& dt, const Price& price, unsigned size) throw(PositionException);
  //! Virtual implementation. Will throw an exception.
  virtual void cover(const boost::gregorian::date& dt, const Price& price, unsigned size) throw(PositionException);
  //! Close any open size by adding a sell Execution.
  virtual void close(const boost::gregorian::date& dt, const Price& price) throw(PositionException);

private:
  unsigned _buys;
  unsigned _sells;

  double _avgBuyPrice;
  double _avgSellPrice;
};

#endif // _LONGPOSITION_HPP_
