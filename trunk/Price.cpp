/*
* Price.cpp
*/

// Hudson
#include "Price.hpp"


Price::Price(double value) throw(PriceException):
  _value(value)
{
  if( _value <= 0 )
    throw PriceException("Invalid price");
}


Price::Price(const Price& src)
{
  _value = src.value();
}


Price& Price::operator=(const Price& src)
{
  if( this != &src )
    _value = src.value();

  return *this;
}

