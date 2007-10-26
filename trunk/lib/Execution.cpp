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

// STL
#include <iostream>

// Hudson
#include "Execution.hpp"

using namespace std;
using namespace boost::gregorian;


Execution::Execution(Execution::ID id, const date& dt, const Price& price, unsigned size):
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


BuyExecution::BuyExecution(Execution::ID id, const date& dt, const Price& price, unsigned size):
  Execution(id, dt, price, size)
{
}


SellExecution::SellExecution(Execution::ID id, const date& dt, const Price& price, unsigned size):
  Execution(id, dt, price, size)
{
}


SellShortExecution::SellShortExecution(Execution::ID id, const date& dt, const Price& price, unsigned size):
  Execution(id, dt, price, size)
{
}


CoverExecution::CoverExecution(Execution::ID id, const date& dt, const Price& price, unsigned size):
  Execution(id, dt, price, size)
{
}
