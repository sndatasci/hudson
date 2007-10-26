/*
* Copyright (C) 2007, Alberto Giannetti
*
* This file is part of Hudson.
*
* Hudson is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* Hudson is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with Hudson.  If not, see <http://www.gnu.org/licenses/>.
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

  // Average peak
  std::cout << "Avg peak: " << er.avg*100 << '%' << std::endl;
  
  // Consecutive positives
  cout << "Longest peak: ";
  if( er.consecutive.empty() ) {
    cout << 0 << endl;
  } else {
    const SeriesFactor& sf_begin = *(er.consecutive.get<to_key>().begin());
    const SeriesFactor& sf_end   = *(er.consecutive.get<to_key>().rbegin());

    cout << (int)er.consecutive.size() << " [" << sf_begin.from_tm() << '/' << sf_end.to_tm() << ']' << endl;
  }

  // Best peak
  const SeriesFactorSet& sfs = er.high;
  const SeriesFactor& sf_begin = *(er.high.get<from_key>().begin());
  const SeriesFactor& sf_end = *(er.high.get<to_key>().rbegin());

  std::cout << "Best peak: " << (sfs.factor()-1)*100 << '%';
  std::cout << " [" << sf_begin.from_tm() << '/' << sf_end.to_tm() << ']' << std::endl;
}


void PositionsReport::adverse(void) const
{
  if( _pf.num() == 0 )
    return; // avoid exception in report

  PositionFactorsSet::ExcursionResults er = _pf.adverse();

  // Average adverse excursion
  std::cout << "Avg drawdown: " << er.avg*100 << '%' << std::endl;
  
  // Consecutive adverse
  cout << "Longest drawdown: ";
  if( er.consecutive.empty() ) {
    cout << 0 << endl;
  } else {
    const SeriesFactor& sf_begin = *(er.consecutive.get<to_key>().begin());
    const SeriesFactor& sf_end   = *(er.consecutive.get<to_key>().rbegin());

    cout << (int)er.consecutive.size() << " [" << sf_begin.from_tm() << '/' << sf_end.to_tm() << ']' << endl;
  }

  // Worst adverse excursion
  const SeriesFactorSet& sfs = er.high;
  const SeriesFactor& sf_begin = *(er.high.get<from_key>().begin());
  const SeriesFactor& sf_end = *(er.high.get<to_key>().rbegin());

  std::cout << "Worst drawdown: " << (sfs.factor()-1)*100 << '%';
  std::cout << " [" << sf_begin.from_tm() << '/' << sf_end.to_tm() << ']' << std::endl;
}
