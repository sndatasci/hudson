/*
 * LongPosition.cpp
 */

#include "LongPosition.hpp"

using namespace std;
using namespace boost::gregorian;


LongPosition::LongPosition(ID id, const string& symbol, const date& dt, double price, unsigned size) throw(PositionException):
  Position(id, symbol),
  _buys(0),
  _sells(0),
  _avgBuyPrice(0),
  _avgSellPrice(0)
{
  if( size == 0 )
	throw PositionException("Invalid size");

  if( price <= 0 )
	throw PositionException("Invalid price");

  if( dt.is_not_a_date() )
	throw PositionException("Invalid date");

  if( _sExecutions.buy(dt, price, size) == false )
	throw PositionException("Invalid execution");

  _avgBuyPrice = ((_avgBuyPrice * _buys) + (price * size)) / (double)(_buys + size);

  _size += size;
  _buys += size;
}


void LongPosition::buy(const date& dt, double price, unsigned size) throw(PositionException)
{
  if( closed() )
	throw PositionException("Position is closed");

  if( size == 0 )
	throw PositionException("Invalid size");

  if( price <= 0 )
	throw PositionException("Invalid price");

  if( dt.is_not_a_date() )
	throw PositionException("Invalid date");

  if( _sExecutions.buy(dt, price, size) == false )
	throw PositionException("Invalid execution");

  _avgBuyPrice = ((_avgBuyPrice * _buys) + (price * size)) / (double)(_buys + size);

  _size += size;
  _buys += size;
}


void LongPosition::sell(const date& dt, double price, unsigned size) throw(PositionException)
{
  if( closed() )
	throw PositionException("Position is closed");

  if( size == 0 || size > _size )
	throw PositionException("Invalid size");

  if( price <= 0 )
	throw PositionException("Invalid price");

  if( dt.is_not_a_date() )
	throw PositionException("Invalid date");

  if( _sExecutions.sell(dt, price, size) == false )
	throw PositionException("Invalid execution");

  _avgSellPrice = ((_avgSellPrice * _sells) + (price * size)) / (_sells + size);

  _size -= size;
  _sells += size;
}


void LongPosition::close(const date& dt, double price) throw(PositionException)
{
  if( closed() )
	throw PositionException("Position is closed");

  sell(dt, price, _size);
}


double LongPosition::factor(void) const
{
  if( _avgBuyPrice <= 0 || _avgSellPrice <= 0 )
	return 1;

  return _avgSellPrice / _avgBuyPrice;
}


void LongPosition::sell_short(const date& dt, double price, unsigned size) throw(PositionException)
{
  throw PositionException("Invalid side");
}


void LongPosition::cover(const date& dt, double price, unsigned size) throw(PositionException)
{
  throw PositionException("Invalid side");
}
