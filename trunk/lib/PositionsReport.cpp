/*
* Copyright (C) 2007,2008 Alberto Giannetti
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

  cout << "Favorable Excursion" << endl;

  PositionFactorsSet::ExcursionResults er = _pf.favorable();

  // Average favorable excursion
  cout << "Average FE: " << er.avg*100 << '%' << endl;
  
  _consecutive_favorable(er.consecutive);
  _best_favorable(er.high);
}


void PositionsReport::adverse(void) const
{
  if( _pf.num() == 0 )
    return; // avoid exception in report

  std::cout << "Adverse Excursion" << std::endl;

  PositionFactorsSet::ExcursionResults er = _pf.adverse();

  // Average adverse excursion
  std::cout << "Avg AE: " << er.avg*100 << '%' << std::endl;

  _consecutive_adverse(er.consecutive);
  _worst_adverse(er.high);
}


void PositionsReport::_consecutive_favorable(const SeriesFactorSet& consecutive) const
{
  cout << "Longest FE: ";
  if( consecutive.empty() ) {
    cout << 0 << endl;
    return;
  }
   
  const SeriesFactor& sf_begin = *(consecutive.get<to_key>().begin());
  const SeriesFactor& sf_end   = *(consecutive.get<to_key>().rbegin());

  cout << " Position " << consecutive.position() << " [" << sf_begin.from_tm() << '/' << sf_end.to_tm() << ']' << endl;
}


void PositionsReport::_best_favorable(const SeriesFactorSet& high) const
{
  cout << "Best FE: ";
  if( high.empty() ) {
    cout << 0 << endl;
    return;
  }

  const SeriesFactor& sf_begin = *(high.get<from_key>().begin());
  const SeriesFactor& sf_end = *(high.get<to_key>().rbegin());

  cout << (high.factor()-1)*100 << '%';
  cout << " Position " << high.position() << " [" << sf_begin.from_tm() << '/' << sf_end.to_tm() << ']' << endl << endl;
}


void PositionsReport::_consecutive_adverse(const SeriesFactorSet& consecutive) const
{
  cout << "Longest AE: ";
  if( consecutive.empty() ) {
    cout << 0 << endl;
    return;
  }

  const SeriesFactor& sf_begin = *(consecutive.get<to_key>().begin());
  const SeriesFactor& sf_end   = *(consecutive.get<to_key>().rbegin());

  cout << " Position " << consecutive.position() << " [" << sf_begin.from_tm() << '/' << sf_end.to_tm() << ']' << endl;
}


void PositionsReport::_worst_adverse(const SeriesFactorSet& high) const
{
  cout << "Worst AE: ";
  if( high.empty() ) {
    cout << 0 << endl;
    return;
  }

  const SeriesFactor& sf_begin = *(high.get<from_key>().begin());
  const SeriesFactor& sf_end = *(high.get<to_key>().rbegin());

  cout << (high.factor()-1)*100 << '%';
  cout << " Position " << high.position() << " [" << sf_begin.from_tm() << '/' << sf_end.to_tm() << ']' << endl << endl;
}
