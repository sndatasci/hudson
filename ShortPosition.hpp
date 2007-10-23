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

#ifndef _SHORTPOSITION_HPP_
#define _SHORTPOSITION_HPP_

#ifdef WIN32
#pragma warning (disable:4290)
#endif

// Hudson
#include "Position.hpp"

//! A new short position.
/*!
  ShortPosition automatically generates a new short Execution for a specific symbol/price/size. It runs a number of consistency checks to
  validate the price, date, and the specified execution ID. Only shorts() and cover() transactions can be executed, other transaction types
  will generate an exception.
*/
class ShortPosition: public Position
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
  ShortPosition(ID id, const std::string& symbol, const boost::gregorian::date& dt, const Price& price, unsigned size) throw(PositionException);

  virtual Type type(void) const { return SHORT; }
  virtual std::string type_str(void) const { return "Short"; }

  //! The number of short executions run on this position.
  unsigned shorts(void) const { return _shorts; }
  //! The number of cover executions run on this position.
  unsigned covers(void) const { return _covers; }

  //! Average short price.
  virtual double avgEntryPrice(void) const { return _avgShortPrice; }
  //! Average cover price.
  virtual double avgExitPrice(void) const { return _avgCoverPrice; }

  //! Current return factor: average short price divided by the average cover price.
  virtual double factor(void) const throw(PositionException);
  //! Return factor calculated dividing avgEntrPrice() into the price parameter.
  virtual double factor(const Price& price) const throw(PositionException);
  //! XXX: Should be a static.
  virtual double factor(const Price& prev_price, const Price& curr_price) const;
  
  //! Virtual implementation. Will throw an exception.
  virtual void buy(const boost::gregorian::date& dt, const Price& price, unsigned size) throw(PositionException);
  //! Virtual implementation. Will throw an exception.
  virtual void sell(const boost::gregorian::date& dt, const Price& price, unsigned size) throw(PositionException);
  //! Add a short Execution.
  virtual void sell_short(const boost::gregorian::date& dt, const Price& price, unsigned size) throw(PositionException);
  //! Add a cover Execution.
  virtual void cover(const boost::gregorian::date& dt, const Price& price, unsigned size) throw(PositionException);
  //! Close any open short size by adding a cover Execution.
  virtual void close(const boost::gregorian::date& dt, const Price& price) throw(PositionException);

private:
  unsigned _shorts;
  unsigned _covers;

  double _avgShortPrice;
  double _avgCoverPrice;
};

#endif // _SHORTPOSITION_HPP_
