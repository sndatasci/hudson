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
#include "LongPosition.hpp"

using namespace std;
using namespace boost::gregorian;


LongPosition::LongPosition(ID id, const string& symbol, const date& dt, const Price& price, unsigned size) throw(PositionException):
  Position(id, symbol),
  _buys(0),
  _sells(0),
  _avgBuyPrice(0),
  _avgSellPrice(0)
{
  if( size == 0 )
	  throw PositionException("Invalid size");

  if( dt.is_not_a_date() )
	  throw PositionException("Invalid date");

  if( _sExecutions.buy(dt, price, size) == false )
	  throw PositionException("Invalid execution");

  _avgBuyPrice = ((_avgBuyPrice * _buys) + (price * size)) / (double)(_buys + size);

  _size += size;
  _buys += size;
}


void LongPosition::buy(const date& dt, const Price& price, unsigned size) throw(PositionException)
{
  if( closed() )
	  throw PositionException("Position is closed");

  if( size == 0 )
	  throw PositionException("Invalid size");

  if( dt.is_not_a_date() )
	  throw PositionException("Invalid date");

  if( _sExecutions.buy(dt, price, size) == false )
	  throw PositionException("Invalid execution");

  _avgBuyPrice = ((_avgBuyPrice * _buys) + (price * size)) / (double)(_buys + size);

  _size += size;
  _buys += size;
}


void LongPosition::sell(const date& dt, const Price& price, unsigned size) throw(PositionException)
{
  if( closed() )
	  throw PositionException("Position is closed");

  if( size == 0 || size > _size )
	  throw PositionException("Invalid size");

  if( dt.is_not_a_date() )
	  throw PositionException("Invalid date");

  if( _sExecutions.sell(dt, price, size) == false )
	  throw PositionException("Invalid execution");

  _avgSellPrice = ((_avgSellPrice * _sells) + (price * size)) / (_sells + size);

  _size -= size;
  _sells += size;
}


void LongPosition::close(const date& dt, const Price& price) throw(PositionException)
{
  if( closed() )
	  throw PositionException("Position is closed");

  sell(dt, price, _size);
}


void LongPosition::sell_short(const date& dt, const Price& price, unsigned size) throw(PositionException)
{
  throw PositionException("Invalid side");
}


void LongPosition::cover(const date& dt, const Price& price, unsigned size) throw(PositionException)
{
  throw PositionException("Invalid side");
}


double LongPosition::factor( void ) const throw(PositionException)
{
  if( ! closed() )
    throw PositionException("Position not closed");

  if( _avgBuyPrice <= 0 )
    throw PositionException("Invalid average buy price");

  if( _avgSellPrice <= 0 )
    throw PositionException("Invalid average sell price");

  return _avgSellPrice / _avgBuyPrice;
}


double LongPosition::factor( const Price& price ) const throw(PositionException)
{
  if( _avgBuyPrice <= 0 )
    throw PositionException("Invalid average buy price");

  return price / _avgBuyPrice;
}


double LongPosition::factor( const Price& prev_price, const Price& curr_price ) const
{
  return curr_price / prev_price;
}
