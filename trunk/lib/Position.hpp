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
#include <boost/shared_ptr.hpp>

// Hudson
#include "ExecutionSet.hpp"
#include "EODDB.hpp"

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


class Position;
typedef boost::shared_ptr<Position> PositionPtr;

/*!
  \brief Abstract class defining Position services.
  \see LongPosition.
  \see ShortPosition.
  \see StrategyPosition.
*/
class Position
{
public:
  typedef unsigned long ID;

  enum Type {
	  LONG,
	  SHORT,
	  STRATEGY
  };

public:  
  //! Default comparison by Position ID.
  bool operator<(const Position& p) const { return _id < p._id; }
  bool operator==(const Position& p) const { return _id == p._id; }

  //! Unique Position identifier.
  ID id(void) const { return _id; }
  //! Returns Position security ticker.
  std::string symbol(void) const { return _symbol; }
  //! Returns current open size.
  int size(void) const { return _size; }

  //! Is Position open.
  virtual bool open(void) const { return _size != 0; }
  //! Is Position closed.
  virtual bool closed(void) const { return !_sExecutions.empty() && !open(); }
  //! Print position data.
  virtual void print(void) const;

  //! Return Position type.
  virtual Type type(void) const = 0;
  //! Return Position type as string.
  virtual std::string type_str(void) const = 0;

  //! First Execution by time.
  virtual const Execution& first_exec(void) const { return _sExecutions.first_by_date(); }
  //! Last Execution by time.
  virtual const Execution& last_exec(void) const { return _sExecutions.last_by_date(); }
  //! Return all Execution.
  const ExecutionSet& executions(void) const { return _sExecutions; }
  
  //! Return Position holding period, from first opening execution to last closing position execution,
  //! or the last date in the database for the position symbol.
  boost::gregorian::date_period hold_period(void) const throw(PositionException);

  //! Return Position average entry price.
  virtual double avgEntryPrice(void) const throw(PositionException) = 0;
  //! Return Position average exit price.
  virtual double avgExitPrice(void) const throw(PositionException) = 0;
  
  //! Add an existing Position to this Position. Only valid for composite positions.
  //! \return True if the Position was successfully added, false otherwise.
  virtual bool add(const PositionPtr pPos) throw(PositionException) = 0;

  //! Return current return factor.
  virtual double factor(void) const = 0;
  //! Return current return factor calculated from avgEntryPrice to price parameter.
  virtual double factor(const Price& price) const throw(PositionException) = 0;
  //! Return return factor from prev_price to curr_price. Factor calculation will change according to Position type.
  virtual double factor(const Price& prev_price, const Price& curr_price) const throw(PositionException) = 0;
  //! Return monthly factor for month/year period.
  virtual double factor(const boost::gregorian::date::month_type& month, const boost::gregorian::date::year_type& year) const throw(PositionException) = 0;

  //! Add BuyExecution at specific price.
  virtual void buy(const boost::gregorian::date& dt, const Price& price, unsigned size) throw(PositionException) = 0;
  //! Add BuyExecution at database PriceType.
  virtual void buy(const boost::gregorian::date& dt, Series::EODDB::PriceType pt, unsigned size) throw(PositionException) = 0;
  
  //! Add SellExecution at specific price.
  virtual void sell(const boost::gregorian::date& dt, const Price& price, unsigned size) throw(PositionException) = 0;
  //! Add SellExecution at database PriceType.
  virtual void sell(const boost::gregorian::date& dt, Series::EODDB::PriceType pt, unsigned size) throw(PositionException) = 0;
    
  //! Add SellShortExecution at specific price.
  virtual void sell_short(const boost::gregorian::date& dt, const Price& price, unsigned size) throw(PositionException) = 0;
  //! Add SellShortExecution database PriceType.
  virtual void sell_short(const boost::gregorian::date& dt, Series::EODDB::PriceType pt, unsigned size) throw(PositionException) = 0;
    
  //! Add CoverExecution at specific price.
  virtual void cover(const boost::gregorian::date& dt, const Price& price, unsigned size) throw(PositionException) = 0;
  //! Add CoverExecution at database PriceType.
  virtual void cover(const boost::gregorian::date& dt, Series::EODDB::PriceType pt, unsigned size) throw(PositionException) = 0;
    
  //! Close open Position size on dt at a specific price.
  virtual void close(const boost::gregorian::date& dt, const Price& price) throw(PositionException) = 0;
  //! Close open Position size on dt at database PriceType.
  virtual void close(const boost::gregorian::date& dt, Series::EODDB::PriceType pt) throw(PositionException) = 0;
 
protected:
  Position(ID id, const std::string& symbol);

  ID _id;
  std::string _symbol;
  unsigned _size;

  ExecutionSet _sExecutions;
};

#endif // _POSITION_HPP_
