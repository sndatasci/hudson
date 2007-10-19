/*
* EOMReport.cpp
*/

#include <StdAfx.h>

// Hudson
#include "EOMReport.hpp"

using namespace std;


EOMReport::EOMReport( const EOMReturnFactors& rf ):
  Report(rf),
  _eomrf(rf)
{
}


void EOMReport::print( void ) const
{
  Report::print();

  streamsize curr_precision = cout.precision();
  ios_base::fmtflags curr_flags = cout.flags();

  cout.precision(2);
  cout.setf(ios::fixed);

  cagr();
  gsdm();
  sharpe();

  cout.precision(curr_precision);
  cout.flags(curr_flags);
}
