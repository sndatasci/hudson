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
#include "PositionSet.hpp"

class Price;

//! Composite Position class.
/*!
  StrategyPosition is a composite pattern class that aggregates executions for one or more Position objects. The factor() method is redefined
  to accumulate factors for all Position included in the StrategyPosition. Any type of Position can be added to the StrategyPosition,
  including another StrategyPosition. This class can be used to build multi-leg strategies such as spread trades or pair trades, and calculate
  simulation statistics based on multiple Position composite returns.
  \see ReturnFactors.
  \see EOMReturnFactors.
 */
class StrategyPosition: public Position
{
public:
  //! Initialize a new StrategyPosition.
  /*!
    \param id A unique Position identifier.
    \param symbol A virtual symbol name used to name this composite position.
    \param pPos The first Position to be added to the strategy.
  */
  StrategyPosition(Position::ID id, const std::string& symbol, const PositionPtr pPos);

  //! Add a new Position.
  /*!
    \param p A pointer to the Position that will be added to this StrategyPosition.
    \return True if the Position was successfully added, false otherwise.
  */
  virtual bool add(const PositionPtr p) throw(PositionException);

  //! Get a specific Position by ID.
  /*!
    \param id The unique identifier of the wanted Position.
    \return An iterator to the PositionPtr in PositionSet matching id, or PositionSet::end() if not found.
  */
  PositionSet::const_iterator get(Position::ID id) const { return _sPositions.find(id, pos_comp_id()); }
  //! Get all Position objects included in this StrategyPosition.
  //! \return A reference to the complete set of Positions for this StrategyPosition.
  const PositionSet& get(void) const { return _sPositions; }

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
  virtual double avgEntryPrice(void) const throw(PositionException);
  //! Always throw an exception. A StrategyPosition does not have a unique entry price.
  //! See get() to extract underlying Position objects and read average entry/exit prices.
  virtual double avgExitPrice(void) const throw(PositionException);

  //! Return strategy factor. This is the product factor for all Position included in the strategy.
  virtual double factor(void) const;
  //! Always throw an exception. A StrategyPosition does not a have a single entry/exit price.
  virtual double factor(const Price& price) const throw(PositionException);
  //! Always throw an exception. A StrategyPosition does not a have a single entry/exit price.
  virtual double factor(const Price& prev_price, const Price& curr_price) const throw(PositionException);
  //! Return monthly factor for month/year period.
  virtual double factor(const boost::gregorian::date::month_type& month, const boost::gregorian::date::year_type& year) const throw(PositionException);

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

protected:
  PositionSet _sPositions;
};

#endif // _STRATEGYPOSITION_HPP_
