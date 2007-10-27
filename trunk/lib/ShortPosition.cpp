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

#include "StdAfx.h"

// Hudson
#include "ShortPosition.hpp"
#include "EODDB.hpp"

using namespace std;
using namespace boost::gregorian;


ShortPosition::ShortPosition(ID id, const string& symbol, const date& dt, const Price& price, unsigned size) throw(PositionException):
  Position(id, symbol),
  _shorts(0),
  _covers(0),
  _avgShortPrice(0),
  _avgCoverPrice(0)
{
  if( size == 0 )
	  throw PositionException("Invalid size");

  if( dt.is_not_a_date() )
	  throw PositionException("Invalid date");

  if( _sExecutions.sell_short(dt, price, size) == false )
	  throw PositionException("Invalid execution");

  _avgShortPrice = ((_avgShortPrice * _shorts) + (price * size)) / (double)(_shorts + size);

  _size += size;
  _shorts += size;
}


void ShortPosition::sell_short(const date& dt, const Price& price, unsigned size) throw(PositionException)
{
  if( closed() )
	  throw PositionException("Position is closed");

  if( size == 0 )
	  throw PositionException("Invalid size");

  if( dt.is_not_a_date() )
	  throw PositionException("Invalid date");

  if( _sExecutions.sell_short(dt, price, size) == false )
	  throw PositionException("Invalid execution");

  _avgShortPrice = ((_avgShortPrice * _shorts) + (price * size)) / (double)(_shorts + size);

  _size += size;
  _shorts += size;
}


void ShortPosition::cover(const date& dt, const Price& price, unsigned size) throw(PositionException)
{
  if( closed() )
	  throw PositionException("Position is closed");

  if( size == 0 || size > _size )
	  throw PositionException("Invalid size");

  if( dt.is_not_a_date() )
	  throw PositionException("Invalid date");

  if( _sExecutions.cover(dt, price, size) == false )
	  throw PositionException("Invalid execution");

  _avgCoverPrice = ((_avgCoverPrice * _covers) + (price * size)) / (_covers + size);

  _size -= size;
  _covers += size;
}


void ShortPosition::close(const date& dt, const Price& price) throw(PositionException)
{
  if( closed() )
	  throw PositionException("Position is closed");

  cover(dt, price, _size);
}


void ShortPosition::buy(const boost::gregorian::date& dt, const Price& price, unsigned size) throw(PositionException)
{
  throw PositionException("Short position");
}


void ShortPosition::sell(const boost::gregorian::date& dt, const Price& price, unsigned size) throw(PositionException)
{
  throw PositionException("Short position");
}


double ShortPosition::factor( void ) const throw(PositionException)
{
  if( _avgShortPrice <= 0 )
    throw PositionException("Invalid average short price");

  if( closed() && _avgCoverPrice <= 0 )
    throw PositionException("Invalid average cover price");

  return closed() ? _avgShortPrice / _avgCoverPrice : _avgShortPrice / Series::EODDB::instance().get(_symbol).last().adjclose;
}


double ShortPosition::factor( const Price& price ) const throw(PositionException)
{
  if( closed() )
    throw PositionException("Closed position");

  if( _avgShortPrice <= 0 )
    throw PositionException("Invalid short price");

  return _avgShortPrice / price;
}


double ShortPosition::factor( const Price& prev_price, const Price& curr_price ) const
{
  return prev_price / curr_price;
}
