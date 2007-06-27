/*
 * ShortPosition.cpp
 */

#include "StdAfx.h"

// Hudson
#include "ShortPosition.hpp"

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
  if( ! closed() )
    throw PositionException("Position not closed");

  if( _avgShortPrice <= 0 )
    throw PositionException("Invalid average short price");

  if( _avgCoverPrice <= 0 )
    throw PositionException("Invalid average cover price");

  return _avgShortPrice / _avgCoverPrice;
}


double ShortPosition::factor( const Price& price ) const throw(PositionException)
{
  if( closed() )
    throw PositionException("Closed position");

  if( _avgShortPrice <= 0 )
    throw PositionException("Invalid short price");

  return _avgShortPrice / price;
}
