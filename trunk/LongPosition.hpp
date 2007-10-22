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


class LongPosition: public Position
{
public:
  LongPosition(ID id, const std::string& symbol, const boost::gregorian::date& dt, const Price& price, unsigned size) throw(PositionException);

  virtual Type type(void) const { return LONG; }
  virtual std::string type_str(void) const { return "Long"; }

  unsigned buys(void) const { return _buys; }
  unsigned sells(void) const { return _sells; }

  virtual double avgEntryPrice(void) const { return _avgBuyPrice; }
  virtual double avgExitPrice(void) const { return _avgSellPrice; }

  virtual double factor(void) const throw(PositionException);
  virtual double factor(const Price& price) const throw(PositionException);
  virtual double factor(const Price& prev_price, const Price& curr_price) const;

  virtual void buy(const boost::gregorian::date& dt, const Price& price, unsigned size) throw(PositionException);
  virtual void sell(const boost::gregorian::date& dt, const Price& price, unsigned size) throw(PositionException);
  virtual void sell_short(const boost::gregorian::date& dt, const Price& price, unsigned size) throw(PositionException);
  virtual void cover(const boost::gregorian::date& dt, const Price& price, unsigned size) throw(PositionException);
  virtual void close(const boost::gregorian::date& dt, const Price& price) throw(PositionException);

private:
  unsigned _buys;
  unsigned _sells;

  double _avgBuyPrice;
  double _avgSellPrice;
};

#endif // _LONGPOSITION_HPP_
