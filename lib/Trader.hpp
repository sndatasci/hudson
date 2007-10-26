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

#ifndef _TRADER_HPP_
#define _TRADER_HPP_

#ifdef WIN32
#pragma warning (disable:4290)
#endif

// STL
#include <string>

// Boost
#include <boost/date_time/gregorian/gregorian.hpp>

// Hudson
#include "PositionSet.hpp"

class Price;


class TraderException: public std::exception
{
public:
  TraderException(const std::string& msg):
	  _Str("TraderException: ")
  {
    _Str += msg;
  }

  virtual ~TraderException(void) throw() { }
  virtual const char *what(void) const throw() { return _Str.c_str(); }

protected:
  std::string _Str;
};


class Trader
{
public:
  Trader(void);
  virtual ~Trader(void) { }

  // Buy/Sell
  Position::ID buy(const std::string& symbol, const boost::gregorian::date& dt, const Price& price, unsigned size = 1) throw(TraderException);  
  void buy(Position::ID, const boost::gregorian::date& dt, const Price& price, unsigned size = 1) throw(TraderException);
  void sell(Position::ID, const boost::gregorian::date& dt, const Price& price, unsigned size = 1) throw(TraderException);

  // Short Sell/Cover
  Position::ID sell_short(const std::string& symbol, const boost::gregorian::date& dt, const Price& price, unsigned size = 1) throw(TraderException);
  void sell_short(Position::ID, const boost::gregorian::date& dt, const Price& price, unsigned size = 1) throw(TraderException);
  void cover(Position::ID, const boost::gregorian::date& dt, const Price& price, unsigned size = 1) throw(TraderException);

  // Close
  void close(Position::ID, const boost::gregorian::date& dt, const Price& price) throw(TraderException);

  const PositionSet& positions(void) const { return _miPositions; }
  PositionSet positions(const std::string& symbol);

protected:
  Position::ID _pid;
  PositionSet _miPositions;
};

#endif // _TRADER_HPP_
