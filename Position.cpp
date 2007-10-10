/*
 * Position.cpp
 */

#include "StdAfx.h"

// STL
#include <iostream>

// Hudson
#include "Price.hpp"
#include "Position.hpp"

using namespace std;
using namespace boost::gregorian;


Position::Position(ID id, const string& symbol):
  _id(id),
  _symbol(symbol),
  _size(0)
{
}


void Position::print(void) const
{
  cout << _symbol << ": ";
  _sExecutions.print();

  if( closed() )
    cout << " - " << "Factor " << factor();
}
