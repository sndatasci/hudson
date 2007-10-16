/*
 * Position.cpp
 */

#include "StdAfx.h"

// STL
#include <iostream>
#include <iomanip>

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


void Position::print(double curr_price) const
{
  cout << _symbol << ": ";
  _sExecutions.print();
  double factor = pfactor(*this, curr_price);

  if( open() )
    cout << " (" << curr_price << ") ";
    
  cout << " - " << "Factor " << factor << " (" << ((factor-1)*100) << "%)";
}
