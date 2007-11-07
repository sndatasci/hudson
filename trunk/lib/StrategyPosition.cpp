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

// IOSTREAM
#include <iostream>

// Hudson
#include "StrategyPosition.hpp"

using namespace std;


StrategyPosition::StrategyPosition( Position::ID id, const std::string& symbol ):
  Position(id, symbol)
{
}


double StrategyPosition::avgEntryPrice( void ) const throw(PositionException)
{
  throw PositionException("StrategyPosition does not have single average entry price");
}


double StrategyPosition::avgExitPrice( void ) const throw(PositionException)
{
  throw PositionException("StrategyPosition does not have single average exit price");
}


void StrategyPosition::buy( const boost::gregorian::date& dt, const Price& price, unsigned size ) throw(PositionException)
{
  throw PositionException("StrategyPosition can not be bought");
}


void StrategyPosition::sell( const boost::gregorian::date& dt, const Price& price, unsigned size ) throw(PositionException)
{
  throw PositionException("StrategyPosition can not be sold");
}


void StrategyPosition::sell_short( const boost::gregorian::date& dt, const Price& price, unsigned size ) throw(PositionException)
{
  throw PositionException("StrategyPosition can not be sold short");
}


void StrategyPosition::cover( const boost::gregorian::date& dt, const Price& price, unsigned size ) throw(PositionException)
{
  throw PositionException("StrategyPosition can not be covered");
}


void StrategyPosition::close( const boost::gregorian::date& dt, const Price& price ) throw(PositionException)
{
  throw PositionException("StrategyPosition can not be closed at unique price");
}


void StrategyPosition::close( const boost::gregorian::date& dt, Series::EODDB::PriceType pt ) throw(PositionException)
{
  for( PositionSet::const_iterator citer = _sPositions.begin(); citer != _sPositions.end(); ++citer )
    (*citer)->close(dt, pt);
}


double StrategyPosition::factor( void ) const
{
  double f = 1;
  for( PositionSet::const_iterator citer = _sPositions.begin(); citer != _sPositions.end(); ++citer )
    f *= (*citer)->factor();
    
  return f;
}


double StrategyPosition::factor( const Price& price ) const throw(PositionException)
{
  throw PositionException("StrategyPosition can not calculate factor on unique price");
}


double StrategyPosition::factor( const Price& prev_price, const Price& curr_price ) const throw(PositionException)
{
  throw PositionException("StrategyPosition can not calculate factor on unique price");
}


bool StrategyPosition::add( const PositionPtr p )
{
  // Add position pointers
  if( _sPositions.insert(p).second ) {
    // Add execution pointers
    _sExecutions.add(p->executions());
    return true;
  }
  
  return false;
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
  for( PositionSet::const_iterator citer = _sPositions.begin(); citer != _sPositions.end(); ++citer )
    (*citer)->print();
    
  cout << endl << "Strategy factor " << factor() << " (" << (factor()-1)*100 << "%)";
}

