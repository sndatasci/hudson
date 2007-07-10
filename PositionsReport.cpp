/*
* PositionsReport.cpp
*/

#include <StdAfx.h>

// STL
#include <iostream>

// Hudson
#include "PositionsReport.hpp"

using namespace std;


PositionsReport::PositionsReport( const PositionsFactors& pf ):
  _pf(pf)
{

}


void PositionsReport::print( void ) const
{
  streamsize curr_precision = cout.precision();
  ios_base::fmtflags curr_flags = cout.flags();

  cout.precision(2);
  cout.setf(ios::fixed);

  avg_neg_excursion();
  avg_pos_excursion();
  worst_excursion();
  best_excursion();

  cout.precision(curr_precision);
  cout.flags(curr_flags);
}
