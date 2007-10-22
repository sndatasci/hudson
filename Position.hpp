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

#ifndef _POSITION_HPP_
#define _POSITION_HPP_

#ifdef WIN32
#pragma warning (disable:4290)
#endif

// STL
#include <stdexcept>
#include <string>

// Boost
#include <boost/date_time/gregorian/gregorian.hpp>

// Hudson
#include "ExecutionSet.hpp"

class Price;


class PositionException: public std::exception
{
public:
  PositionException(const std::string& msg):
	  _Str("PositionException: ")
  {
    _Str += msg;
  }

  virtual ~PositionException(void) throw() { }
  virtual const char *what() const throw() { return _Str.c_str(); }

protected:
  std::string _Str;
};


class Position
{
public:
  typedef unsigned long ID;

  enum Type {
	  LONG,
	  SHORT
  };

public:
  bool operator<(const Position& p) const { return _id < p._id; }
  bool operator==(const Position& p) const { return _id == p._id; }

  ID id(void) const { return _id; }
  std::string symbol(void) const { return _symbol; }
  int size(void) const { return _size; }

  bool open(void) const { return _size != 0; }
  bool closed(void) const { return !_sExecutions.empty() && !open(); }
  void print(Price curr_price) const;

  virtual Type type(void) const = 0;
  virtual std::string type_str(void) const = 0;

  const Execution& first_exec(void) const { return _sExecutions.first_by_date(); }
  const Execution& last_exec(void) const { return _sExecutions.last_by_date(); }

  virtual double avgEntryPrice(void) const = 0;
  virtual double avgExitPrice(void) const = 0;

  virtual double factor(void) const = 0;
  virtual double factor(const Price& price) const = 0;
  virtual double factor(const Price& prev_price, const Price& curr_price) const = 0;

  virtual void buy(const boost::gregorian::date& dt, const Price& price, unsigned size) throw(PositionException) = 0;
  virtual void sell(const boost::gregorian::date& dt, const Price& price, unsigned size) throw(PositionException) = 0;
  virtual void sell_short(const boost::gregorian::date& dt, const Price& price, unsigned size) throw(PositionException) = 0;
  virtual void cover(const boost::gregorian::date& dt, const Price& price, unsigned size) throw(PositionException) = 0;
  virtual void close(const boost::gregorian::date& dt, const Price& price) throw(PositionException) = 0;
 
protected:
  Position(ID id, const std::string& symbol);

  ID _id;
  std::string _symbol;
  unsigned _size;

  ExecutionSet _sExecutions;
};


inline double pfactor(const Position& pos, double curr_price) { return pos.closed() ? pos.factor() : pos.factor(Price(curr_price)); }


#endif // _POSITION_HPP_
