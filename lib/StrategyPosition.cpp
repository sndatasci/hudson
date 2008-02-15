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

#include <StdAfx.h>

// STDLIB
#include <iostream>

// Boost
#include <boost/date_time/gregorian/gregorian.hpp>

// Hudson
#include "StrategyPosition.hpp"

using namespace std;
using namespace boost::gregorian;
using namespace Series;

// XXX Strategy assumes equal weighting of all underlying positions


StrategyPosition::StrategyPosition( Position::ID id, const std::string& symbol, const PositionPtr pPos ):
  Position(id, symbol)
{
  add(pPos);
}


Price StrategyPosition::avgEntryPrice( void ) const throw(PositionException)
{
  throw PositionException("StrategyPosition does not have single average entry price");
}


Price StrategyPosition::avgExitPrice( void ) const throw(PositionException)
{
  throw PositionException("StrategyPosition does not have single average exit price");
}


void StrategyPosition::buy( const boost::gregorian::date& dt, const Price& price, unsigned size ) throw(PositionException)
{
  throw PositionException("StrategyPosition can not be bought");
}


void StrategyPosition::buy( const boost::gregorian::date& dt, Series::EODDB::PriceType pt, unsigned size ) throw(PositionException)
{
  throw PositionException("StrategyPosition can not be bought");
}


void StrategyPosition::sell( const boost::gregorian::date& dt, const Price& price, unsigned size ) throw(PositionException)
{
  throw PositionException("StrategyPosition can not be sold");
}


void StrategyPosition::sell( const boost::gregorian::date& dt, Series::EODDB::PriceType pt, unsigned size ) throw(PositionException)
{
  throw PositionException("StrategyPosition can not be sold");
}


void StrategyPosition::sell_short( const boost::gregorian::date& dt, const Price& price, unsigned size ) throw(PositionException)
{
  throw PositionException("StrategyPosition can not be sold short");
}


void StrategyPosition::sell_short( const boost::gregorian::date& dt, Series::EODDB::PriceType pt, unsigned size ) throw(PositionException)
{
  throw PositionException("StrategyPosition con not be sold short");
}


void StrategyPosition::cover( const boost::gregorian::date& dt, const Price& price, unsigned size ) throw(PositionException)
{
  throw PositionException("StrategyPosition can not be covered");
}


void StrategyPosition::cover( const boost::gregorian::date& dt, Series::EODDB::PriceType pt, unsigned size ) throw(PositionException)
{
  throw PositionException("StrategyPosition can not be covered");
}


void StrategyPosition::close( const boost::gregorian::date& dt, const Price& price ) throw(PositionException)
{
  throw PositionException("StrategyPosition can not be closed at single price");
}


void StrategyPosition::close( const boost::gregorian::date& dt, Series::EODDB::PriceType pt ) throw(PositionException)
{
  for( PositionSet::const_iterator citer = _sPositions.begin(); citer != _sPositions.end(); ++citer )
    (*citer)->close(dt, pt);
}


ExecutionSet StrategyPosition::executions( void )
{
  for( PositionSet::const_iterator citer = _sPositions.begin(); citer != _sPositions.end(); ++citer )
    _sExecutions.add((*citer)->executions());

  return _sExecutions;
}


double StrategyPosition::factor( EODDB::PriceType pt ) const throw(PositionException)
{
  double f = 1;
  for( PositionSet::const_iterator citer = _sPositions.begin(); citer != _sPositions.end(); ++citer )
    f *= (*citer)->factor(pt);
    
  return f;
}


double StrategyPosition::factor( const boost::gregorian::date& dt, EODDB::PriceType pt ) const throw(PositionException)
{
  double f = 1;
  for( PositionSet::const_iterator citer = _sPositions.begin(); citer != _sPositions.end(); ++citer )
    f *= (*citer)->factor(dt, pt);

  return f;
}


double StrategyPosition::factor( const boost::gregorian::date_period& dp, EODDB::PriceType pt ) const throw(PositionException)
{
  double f = 1;
  for( PositionSet::const_iterator citer = _sPositions.begin(); citer != _sPositions.end(); ++citer )
    f *= (*citer)->factor(dp, pt);

  return f;
}


double StrategyPosition::factor( const boost::gregorian::date::month_type& month, const boost::gregorian::date::year_type& year, EODDB::PriceType pt ) const throw(PositionException)
{
  double f = 1;
  for( PositionSet::const_iterator citer = _sPositions.begin(); citer != _sPositions.end(); ++citer )
    f *= (*citer)->factor(month, year);

  return f;
}


SeriesFactorSet StrategyPosition::factors(Series::EODDB::PriceType pt) const throw(PositionException)
{
  cout << "Strategy Position " << _id << " first exec " << first_exec().dt() << ", last exec " << last_exec().dt() << endl;
  cout << "Number of legs: " << _sPositions.size() << endl;
  cout << "Symbol " << _symbol << " end of database: " << EODDB::instance().get(_symbol).rbegin()->first << endl;
  cout << "Is Position closed: " << closed() << endl;
  date last_dt = (closed() ? last_exec().dt() : EODDB::instance().get(_symbol).rbegin()->first);

  date_period dp(first_exec().dt(), last_dt);
  return factors(dp, pt);
}


SeriesFactorSet StrategyPosition::factors( const boost::gregorian::date& dt, Series::EODDB::PriceType pt ) const throw(PositionException)
{
  date_period dp(first_exec().dt(), dt);
  return factors(dp, pt);
}


SeriesFactorSet StrategyPosition::factors( const boost::gregorian::date_period& dp, EODDB::PriceType pt ) const throw(PositionException)
{
  // Accumulate all factors with the same begin and end date until dt for all positions in this strategy.
  SeriesFactorMultiSetFrom sfsAll;

  // Add all SeriesFactor for all positions indexed by first execution date
  for( PositionSet::const_iterator citer = _sPositions.begin(); citer != _sPositions.end(); ++citer ) {
    SeriesFactorSet sfs = (*citer)->factors(dp, pt);
    for( SeriesFactorSet::const_iterator sfs_citer = sfs.begin(); sfs_citer != sfs.end(); ++sfs_citer )
      sfsAll.insert(*sfs_citer);
  }

  return _matchFactors(sfsAll);
}


SeriesFactorSet StrategyPosition::_matchFactors(const SeriesFactorMultiSetFrom& sfsAll) const
{
  SeriesFactorSet sfsStrategy;

  // Extract current factor
  for( SeriesFactorMultiSetFrom::const_iterator citer = sfsAll.begin(); citer != sfsAll.end(); ) {

    // Get current factor
    SeriesFactor currentFactor = *citer;
    double acc = currentFactor.factor();

    cout << "Analyzing Position daily factor from " << currentFactor.from_tm() << " to " << currentFactor.to_tm()
	 << " factor " << currentFactor.factor() << endl;

    // Accumulate all the following factors with the same from/to date
    SeriesFactorMultiSetFrom::const_iterator citer_next = citer;
    while( ++citer_next != sfsAll.end() && citer_next->from_tm() == currentFactor.from_tm() && citer_next->to_tm() == currentFactor.to_tm() ) {
      cout << "Found daily factor with same from/to dates, factor " << citer_next->factor() << endl;
      acc *= citer_next->factor();
    }

    // Add cumulated factor
    sfsStrategy.insert(SeriesFactor(currentFactor.from_tm(), currentFactor.to_tm(), acc));

    // Start from first SeriesFactor with different from/to date
    citer = citer_next;
  }

  return sfsStrategy;
}


bool StrategyPosition::add( const PositionPtr p ) throw(PositionException)
{
  // Add position pointers
  if( ! _sPositions.insert(p).second )
    return false;

  return true;
}


bool StrategyPosition::open( void ) const
{
  for( PositionSet::const_iterator citer = _sPositions.begin(); citer != _sPositions.end(); ++citer )
    if( (*citer)->open() )
      return true;
      
  return false;
}


bool StrategyPosition::closed( void ) const
{
  return !open();
}


void StrategyPosition::print( void ) const
{
  for( PositionSet::const_iterator citer = _sPositions.begin(); citer != _sPositions.end(); ++citer ) {
    (*citer)->print();
    cout << endl;
  }
    
  cout << "Strategy factor " << factor() << " (" << (factor()-1)*100 << "%)" << endl;
}
