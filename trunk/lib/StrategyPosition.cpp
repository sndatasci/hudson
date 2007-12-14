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

// Hudson
#include "StrategyPosition.hpp"

using namespace std;
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


double StrategyPosition::factor( EODDB::PriceType pt ) const
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


SeriesFactorSet StrategyPosition::factors( const boost::gregorian::date& dt, EODDB::PriceType pt ) const throw(PositionException)
{
  // We need to accumulate all factors with the same begin/end day until dt for all positions in this strategy.
  // First add all positions indexed by first execution date in a multi_set
  SeriesFactorMultiSetFrom sfsAll;
  SeriesFactorSet sfsStrategy;

  for( PositionSet::const_iterator citer = _sPositions.begin(); citer != _sPositions.end(); ++citer ) {
    SeriesFactorSet sfs = (*citer)->factors(dt, pt);
    for( SeriesFactorSet::const_iterator sfs_citer = sfs.begin(); sfs_citer != sfs.end(); ++sfs_citer )
      sfsAll.insert(*sfs_citer);
  }

  for( SeriesFactorMultiSetFrom::const_iterator citer = sfsAll.begin(); citer != sfsAll.end(); ++citer ) {
    SeriesFactor currentFactor = *citer;
    double acc = currentFactor.factor();

    SeriesFactorMultiSetFrom::const_iterator citer_next = citer;
    while( ++citer_next != sfsAll.end() ) {
      SeriesFactor nextFactor = *citer_next;
      if( currentFactor.from_tm() == nextFactor.from_tm() && currentFactor.to_tm() == nextFactor.to_tm() ) {
        // Cumulate factors
        acc *= nextFactor.factor();
        ++citer;
      }
    }

    // Add cumulated factor
    sfsStrategy.insert(SeriesFactor(currentFactor.from_tm(), currentFactor.to_tm(), acc));
  }

  return sfsStrategy;
}


SeriesFactorSet StrategyPosition::factors( const boost::gregorian::date_period& dp, EODDB::PriceType pt ) const throw(PositionException)
{
  double f = 1;
  for( PositionSet::const_iterator citer = _sPositions.begin(); citer != _sPositions.end(); ++citer )
    f *= (*citer)->factor(dp, pt);

  return SeriesFactorSet();
}


bool StrategyPosition::add( const PositionPtr p ) throw(PositionException)
{
  // Add position pointers
  if( ! _sPositions.insert(p).second )
    return false;
  
  // Add new Position executions to this Position set
  _sExecutions.add(p->executions());
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


