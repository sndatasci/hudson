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
#include "ExecutionSet.hpp"

using namespace std;


ExecutionSet::ExecutionSet(void):
  _eid(0)
{
}


void ExecutionSet::print(void) const
{
  for( by_date::iterator iter = get<date_key>().begin(); iter != get<date_key>().end(); ++iter ) {
	  cout << " ";
	  (*iter)->print();
  }
}


bool ExecutionSet::buy(boost::gregorian::date dt, const Price& price, unsigned size)
{
  ExecutionPtr pExe(new BuyExecution(++_eid, dt, price, size));
  return insert(pExe).second;
}


bool ExecutionSet::sell(boost::gregorian::date dt, const Price& price, unsigned size)
{
  ExecutionPtr pExe(new SellExecution(++_eid, dt, price, size));
  return insert(pExe).second;
}


bool ExecutionSet::sell_short(boost::gregorian::date dt, const Price& price, unsigned size)
{
  ExecutionPtr pExe(new SellShortExecution(++_eid, dt, price, size));
  return insert(pExe).second;
}


bool ExecutionSet::cover(boost::gregorian::date dt, const Price& price, unsigned size)
{
  ExecutionPtr pExe(new CoverExecution(++_eid, dt, price, size));
  return insert(pExe).second;
}


void ExecutionSet::add( const ExecutionSet& other )
{
  for( ExecutionSet::const_iterator citer = other.begin(); citer != other.end(); ++citer )
    if( insert(*citer).second )
      cout << "Execution " << (*citer)->id() << " inserted" << endl;
}

