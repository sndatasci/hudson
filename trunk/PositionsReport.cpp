/*
* PositionsReport.cpp
*/

#include <StdAfx.h>

// STL
#include <iostream>

// Hudson
#include "PositionsReport.hpp"
#include "SeriesFactorSet.hpp"
#include "SeriesFactor.hpp"

using namespace std;


PositionsReport::PositionsReport( const PositionFactorsSet& pf ):
  _pf(pf)
{
}


void PositionsReport::print( void ) const
{
  streamsize curr_precision = cout.precision();
  ios_base::fmtflags curr_flags = cout.flags();

  cout.precision(2);
  cout.setf(ios::fixed);

  adverse();
  favorable();

  cout.precision(curr_precision);
  cout.flags(curr_flags);
}


void PositionsReport::favorable(void) const
{
  if( _pf.num() == 0 )
    return; // avoid exception in report

  PositionFactorsSet::ExcursionResults er = _pf.favorable();

  // Consecutive positives
  cout << "Longest favorable: ";
  if( er.consecutive.empty() ) {
    cout << 0 << endl;
  } else {
    const SeriesFactor& sf_begin = *(er.consecutive.get<to_key>().begin());
    const SeriesFactor& sf_end   = *(er.consecutive.get<to_key>().rbegin());

    cout << (int)er.consecutive.size() << " [" << sf_begin.from_tm() << '/' << sf_end.to_tm() << ']' << endl;
  }

  // Average favorable excursion
  std::cout << "Avg favorable: " << er.avg*100 << '%' << std::endl;

  // Best favorable excursion
  const SeriesFactorSet& sfs = er.high;
  const SeriesFactor& sf_begin = *(er.high.get<from_key>().begin());
  const SeriesFactor& sf_end = *(er.high.get<to_key>().rbegin());

  std::cout << "Best favorable: " << (sfs.factor()-1)*100 << '%';
  std::cout << " [" << sf_begin.from_tm() << '/' << sf_end.to_tm() << ']' << std::endl;
}


void PositionsReport::adverse(void) const
{
  if( _pf.num() == 0 )
    return; // avoid exception in report

  PositionFactorsSet::ExcursionResults er = _pf.adverse();

  // Consecutive adverse
  cout << "Longest adverse: ";
  if( er.consecutive.empty() ) {
    cout << 0 << endl;
  } else {
    const SeriesFactor& sf_begin = *(er.consecutive.get<to_key>().begin());
    const SeriesFactor& sf_end   = *(er.consecutive.get<to_key>().rbegin());

    cout << (int)er.consecutive.size() << " [" << sf_begin.from_tm() << '/' << sf_end.to_tm() << ']' << endl;
  }

  // Average adverse excursion
  std::cout << "Avg adverse: " << er.avg*100 << '%' << std::endl;

  // Worst adverse excursion
  const SeriesFactorSet& sfs = er.high;
  const SeriesFactor& sf_begin = *(er.high.get<from_key>().begin());
  const SeriesFactor& sf_end = *(er.high.get<to_key>().rbegin());

  std::cout << "Worst adverse: " << (sfs.factor()-1)*100 << '%';
  std::cout << " [" << sf_begin.from_tm() << '/' << sf_end.to_tm() << ']' << std::endl;
}
