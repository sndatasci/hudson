/*
 * PositionSet.cpp
 */

#include "StdAfx.h"

// STL
#include <iostream>
#include <functional>
#include <algorithm>

// Hudson
#include "PositionSet.hpp"

using namespace std;


void PositionSet::print(void) const
{
  for( const_iterator iter = begin(); iter != end(); ++iter ) {
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


double PositionSet::realized( void ) const
{
  double acc = 1;
  for( const_iterator iter = begin(); iter != end(); ++iter )
    if( (*iter)->closed() )
      acc *= (*iter)->factor();

  return acc;
}
