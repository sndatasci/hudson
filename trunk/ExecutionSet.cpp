/*
 * ExecutionSet.cpp
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
