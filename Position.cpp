/*
 * Position.cpp
 */

// IO
#include <iostream>

#include <Position.hpp>

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
  cout << "Factor " << factor();
}
