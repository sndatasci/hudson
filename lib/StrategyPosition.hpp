/*
* Copyright (C) 2007,2008 Alberto Giannetti
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
#include <set>
#include <map>

// Hudson
#include "Price.hpp"
#include "SeriesFactor.hpp"
#include "Position.hpp"
#include "ExecutionObserver.hpp"


//! Composite Position class.
/*!
  StrategyPosition is a composite Position that contains one or more LongPosition/ShortPosition objects.
  Each underlying Position has a weighting factor that is used to calculate the StrategyPosition factors.
  This class can be used to build multi-leg positions such as spread trades or pair trades, and calculate
  one virtual Position simulation statistics from multiple Position returns.
  \see ReturnFactors.
  \see EOMReturnFactors.
 */
class StrategyPosition: public Position, public ExecutionObserver
{
public:
  //! Initialize a new StrategyPosition.
  /*!
    \param id A unique Position identifier.
    \param symbol A virtual symbol name used to name this composite position.
    \param pPos The first Position added to this strategy.
  */
  StrategyPosition(Position::ID id, const std::string& symbol, const PositionPtr pPos, double weight = 1) throw(PositionException);
  virtual ~StrategyPosition(void) { }

  virtual void attach(ExecutionObserver* pObserver) throw(PositionException);
  virtual void detach(ExecutionObserver* pObserver) throw(PositionException);

  //! Add a new Position.
  /*!
    \param pPos A pointer to the Position that will be added to this StrategyPosition.
    \param weight The weight of the Position
  */
  virtual void add(const PositionPtr pPos, double weight = 1) throw(PositionException);

  //! Is this StrategyPosition open.
  /*!
    \return True if any of the underlying Position objects is open, false otherwise.
  */
  virtual bool open(void) const;
  //! Is StrategyPosition closed.
  /*!
    \return True if all the underlying Position are closed, false otherwise.
  */
  virtual bool closed(void) const;
  //! Print data for all underlying Position.
  virtual void print(void) const;

  //! Return position type.
  virtual Type type(void) const { return STRATEGY; }
  //! Return position type as string.
  virtual std::string type_str(void) const { return "Strategy"; }

  //! Always throw an exception. A StrategyPosition does not have a single entry price.
  //! See get() to extract underlying Position objects and read average entry/exit prices.
  virtual Price avgEntryPrice(void) const throw(PositionException);
  //! Always throw an exception. A StrategyPosition does not have a unique entry price.
  //! See get() to extract underlying Position objects and read average entry/exit prices.
  virtual Price avgExitPrice(void) const throw(PositionException);

  //! First Execution by time.
  virtual const ExecutionPtr first_exec(void) const { return executions().first_by_date(); }
  //! Last Execution by time.
  virtual const ExecutionPtr last_exec(void) const { return executions().last_by_date(); }

  //! Return strategy factor. This is the product factor for all Positions included in the strategy.
  virtual double factor(Series::EODDB::PriceType pt = Series::EODDB::ADJCLOSE) const throw(PositionException);
  //! Return factor until dt using PriceType pt. Throw an exception if the input date precedes the Position opening Execution.
  virtual double factor(const boost::gregorian::date& dt, Series::EODDB::PriceType pt = Series::EODDB::ADJCLOSE) const throw(PositionException);
  //! Return factor for a given period using PriceType from_pt and to_pt
  virtual double factor(const boost::gregorian::date_period& dp, Series::EODDB::PriceType pt = Series::EODDB::ADJCLOSE) const throw(PositionException);
  //! Return monthly factor for month/year period.
  virtual double factor(const boost::gregorian::date::month_type& month, const boost::gregorian::date::year_type& year, Series::EODDB::PriceType pt = Series::EODDB::ADJCLOSE) const throw(PositionException);

  //! Return daily factors. If the position is closed, return daily factors from the opening execution to the closing execution. If the Position is open,
  //! return daily factors from the first execution to the last available entry in the database.

  virtual SeriesFactorSet factors(Series::EODDB::PriceType pt = Series::EODDB::ADJCLOSE) const throw(PositionException);
  //! Return series factors until dt using PriceType pt.
  virtual SeriesFactorSet factors(const boost::gregorian::date& dt, Series::EODDB::PriceType pt = Series::EODDB::ADJCLOSE) const throw(PositionException);
  //! Return all factors for the period dp using PriceType pt.
  virtual SeriesFactorSet factors(const boost::gregorian::date_period& dp, Series::EODDB::PriceType pt = Series::EODDB::ADJCLOSE) const throw(PositionException);

  //! Always throw an exception. A StrategyPosition can not be bought directly. See get() to buy a specific LongPosition.
  virtual void buy(const boost::gregorian::date& dt, const Price& price, unsigned size) throw(PositionException);
  //! Always throw an exception. A StrategyPosition can not be bought directly. See get() to buy a specific LongPosition.
  virtual void buy(const boost::gregorian::date& dt, Series::EODDB::PriceType pt, unsigned size) throw(PositionException);
  
  //! Always throw an exception. A StrategyPosition can not be sold directly. See get() to sell a specific LongPosition.
  virtual void sell(const boost::gregorian::date& dt, const Price& price, unsigned size) throw(PositionException);
  //! Always throw an exception. A StrategyPosition can not be sold directly. See get() to sell a specific LongPosition.
  virtual void sell(const boost::gregorian::date& dt, Series::EODDB::PriceType pt, unsigned size) throw(PositionException);
  
  //! Always throw an exception. A StrategyPosition can not be shorted directly. See get() to short a specific ShortPosition.
  virtual void sell_short(const boost::gregorian::date& dt, const Price& price, unsigned size) throw(PositionException);
  //! Always throw an exception. A StrategyPosition can not be shorted directly. See get() to short a specific ShortPosition.
  virtual void sell_short(const boost::gregorian::date& dt, Series::EODDB::PriceType pt, unsigned size) throw(PositionException);
 
  //! Always throw an exception. A StrategyPosition can not be covered directly. See get() to cover a specific ShortPosition.
  virtual void cover(const boost::gregorian::date& dt, const Price& price, unsigned size) throw(PositionException);
  //! Always throw an exception. A StrategyPosition can not be covered directly. See get() to cover a specific ShortPosition.
  virtual void cover(const boost::gregorian::date& dt, Series::EODDB::PriceType pt, unsigned size) throw(PositionException);

  //! Always throw an exception. StrategyPosition can only be closed at price type or by closing each underlying Position.
  virtual void close(const boost::gregorian::date& dt, const Price& price) throw(PositionException);
  //! Close any open size on dt at price type pt.
  /*!
    All the Position objects included in the strategy will be closed on the date specified at PriceType.
    \param dt The series date that will be used to retrieve the database price.
    \param pt The type of price that will be used to close the Position.
  */
  virtual void close(const boost::gregorian::date& dt, Series::EODDB::PriceType pt) throw(PositionException);

  //! Execution notification for underlying NaturalPosition instances
  virtual void update(const ExecutionPtr pExe);

protected:
  struct PositionWeight
  {
    PositionPtr pPos;
    double weight;
  };
  typedef std::map<Position::ID, PositionWeight> POSW_MAP;
  //! Position/weight mapping
  POSW_MAP _mPositions;

private:
  struct SeriesFactorFromCmp: public std::binary_function<SeriesFactor, SeriesFactor, bool>
  {
    bool operator()(const SeriesFactor& sf1, const SeriesFactor& sf2) const { return sf1.from_tm() < sf2.from_tm(); }
  };

  //! Set of SeriesFactor indexed by 'from' date
  typedef std::multiset<SeriesFactor, SeriesFactorFromCmp> SeriesFactorMultiSetFrom;

  SeriesFactorSet _matchFactors(const SeriesFactorMultiSetFrom& sfsAll) const;
};

#endif // _STRATEGYPOSITION_HPP_
