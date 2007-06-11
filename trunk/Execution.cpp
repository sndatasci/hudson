/*
 * Execution.cpp
 */

// IO
#include <iostream>

#include "Execution.hpp"

using namespace std;
using namespace boost::gregorian;


Execution::Execution(ID id, const date& dt, const Price& price, unsigned size):
  _id(id),
  _dt(dt),
  _price(price),
  _size(size)
{
}


void Execution::print(void) const
{
  cout << action()
	   << ' ' << _size
	   << ' ' << _price.value()
	   << ' ' << to_simple_string(_dt) 
	   << " (" << _dt.day_of_week() << ")";
}


BuyExecution::BuyExecution(ID id, const date& dt, const Price& price, unsigned size):
  Execution(id, dt, price, size)
{
}


SellExecution::SellExecution(ID id, const date& dt, const Price& price, unsigned size):
  Execution(id, dt, price, size)
{
}


SellShortExecution::SellShortExecution(ID id, const date& dt, const Price& price, unsigned size):
  Execution(id, dt, price, size)
{
}


CoverExecution::CoverExecution(ID id, const date& dt, const Price& price, unsigned size):
  Execution(id, dt, price, size)
{
}
