/*
 * PositionSet.cpp
 */

// C++
#include <iostream>

#include "PositionSet.hpp"

using namespace std;


void PositionSet::print(void) const
{
  iterator iter;
  for( iter = begin(); iter != end(); ++iter ) {
	(*iter)->print();
	cout << endl;
  }
}


const PositionSet PositionSet::closed(void) const
{
  PositionSet closedPos;

  for( const_iterator iter = begin(); iter != end(); ++iter )
	if( (*iter)->closed() )
	  closedPos.insert(*iter);

  return closedPos;
}


const PositionSet PositionSet::open(void) const
{
  PositionSet openPos;

  for( const_iterator iter = begin(); iter != end(); ++iter )
	if( (*iter)->open() )
	  openPos.insert(*iter);

  return openPos;
}


vector<double> PositionSet::factors(void) const
{
  vector<double> v;

  for( position_by_last_exec::iterator iter = get<last_exec_key>().begin();
	   iter != get<last_exec_key>().end();
	   ++iter )
	v.push_back((*iter)->factor());

  return v;
}
