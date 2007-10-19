/*
* PortfolioReport.cpp
*/

#include "StdAfx.h"

// STL
#include <iostream>

// Hudson
#include "PortfolioReport.hpp"

using namespace std;


PortfolioReport::PortfolioReport( const PortfolioReturns& pr ):
  _pr(pr)
{

}


void PortfolioReport::print(void)
{
  streamsize curr_precision = cout.precision();
  ios_base::fmtflags curr_flags = cout.flags();
  
  cout.precision(2);
  cout.setf(ios::fixed);
  
  roi();
  cagr();
  gsd();
  
  cout.precision(curr_precision);
  cout.flags(curr_flags);
}