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

#ifndef _STRATEGYPOSITION_HPP_
#define _STRATEGYPOSITION_HPP_

#ifdef WIN32
#pragma warning (disable:4290)
#endif

// STL
#include <string>

// Hudson
#include "Position.hpp"

class Price;

//! Composite Position class.
/*!
  StrategyPosition is a composite pattern class that aggregates executions for one or more Position objects. The factor() method is redefined
  to accumulate the factors of all the Position objects included in the strategy. Any type of Position can be added to the StrategyPosition,
  including another StrategyPosition. This class can be used to build multi-leg strategies such as spread trades or pair trades and calculate
  simulation statistics through the ReturnFactors class using the common Position interface.
 */
class StrategyPosition: public Position
{
public:
  StrategyPosition(Position::ID id, const std::string& symbol);

  //! Add a new Position.
  void add(const Position& p);

  //! Get a specific Position by ID.
  const Position& get(Position::ID id) const;
  //! Get all Position objects included in this StrategyPosition.
  const PositionSet& get(void) const;

  //! Is strategy open.
  virtual bool open(void) const;
  //! Is strategy closed.
  virtual bool closed(void) const;
  //! Print strategy data.
  virtual void print(void) const;

  //! Return position type.
  virtual Type type(void) const { return STRATEGY; }
  //! Return position type as string.
  virtual std::string type_str(void) const { return "Strategy"; }

  //! First Execution by date/time
  virtual const Execution& first_exec(void) const;
  //! First Execution by date/time
  virtual const Execution& last_exec(void) const;

  //! Always throw an exception. A StrategyPosition does not have a single entry price.
  //! See get() to extract basic Position objects and read average entry/exit prices.
  virtual double avgEntryPrice(void) const throw(PositionException);
  //! Always throw an exception. A StrategyPosition does not have a unique entry price.
  //! See get() to extract basic Position objects and read average entry/exit prices.
  virtual double avgExitPrice(void) const throw(PositionException);

  //! Return strategy factor. This is the product factor for all Position included in the strategy.
  virtual double factor(void) const;
  //! Always throw an exception. A StrategyPosition does not a have a single entry/exit price.
  virtual double factor(const Price& price) const throw(PositionException);
  //! Always throw an exception. A StrategyPosition does not a have a single entry/exit price.
  virtual double factor(const Price& prev_price, const Price& curr_price) const throw(PositionException);

  //! Always throw an exception. A StrategyPosition can not be bought directly. See get() to buy a specific LongPosition.
  virtual void buy(const boost::gregorian::date& dt, const Price& price, unsigned size) throw(PositionException);
  //! Add SellExecution. A StrategyPosition can not be sold directly. See get() to sell a specific LongPosition.
  virtual void sell(const boost::gregorian::date& dt, const Price& price, unsigned size) throw(PositionException);
  //! Add SellShortExecution. A StrategyPosition can not be shorted directly. See get() to short a specific ShortPosition.
  virtual void sell_short(const boost::gregorian::date& dt, const Price& price, unsigned size) throw(PositionException);
  //! Add CoverExecution. A StrategyPosition can not be covered directly. See get() to cover a specific ShortPosition.
  virtual void cover(const boost::gregorian::date& dt, const Price& price, unsigned size) throw(PositionException);

  //! Close open Position size.
  virtual void close(const boost::gregorian::date& dt, const Price& price) throw(PositionException);

protected:
  double _f;
};

#endif // _STRATEGYPOSITION_HPP_
