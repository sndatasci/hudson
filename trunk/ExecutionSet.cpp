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


ExecutionSet::~ExecutionSet(void)
{
  for( __ExecutionSet::const_iterator iter = _es.begin(); iter != _es.end(); ++iter )
	  delete *iter;
}


void ExecutionSet::print(void) const
{
  for( executions_by_date::iterator iter = _es.get<date_key>().begin(); iter != _es.get<date_key>().end(); ++iter ) {
	  (*iter)->print();
	  cout << ", ";
  }
}


bool ExecutionSet::buy(boost::gregorian::date dt, const Price& price, unsigned size)
{
  Execution* pExe = new BuyExecution(++_eid, dt, price, size);
  return _es.insert(pExe).second;
}


bool ExecutionSet::sell(boost::gregorian::date dt, const Price& price, unsigned size)
{
  Execution* pExe = new SellExecution(++_eid, dt, price, size);
  return _es.insert(pExe).second;
}


bool ExecutionSet::sell_short(boost::gregorian::date dt, const Price& price, unsigned size)
{
  Execution* pExe = new SellShortExecution(++_eid, dt, price, size);
  return _es.insert(pExe).second;
}


bool ExecutionSet::cover(boost::gregorian::date dt, const Price& price, unsigned size)
{
  Execution* pExe = new CoverExecution(++_eid, dt, price, size);
  return _es.insert(pExe).second;
}
