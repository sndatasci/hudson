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

// STD
#include <vector>
#include <algorithm>

// Hudson
#include "PositionFactorsSet.hpp"
#include "PositionFactors.hpp"

using namespace std;


PositionFactorsSet::PositionFactorsSet( const PositionSet& sPositions ):
  _sPositions(sPositions)
{
}


PositionFactorsSet::ExcursionResults PositionFactorsSet::favorable(void) const throw(PositionFactorsSetException)
{
  if ( _sPositions.empty() )
    throw PositionFactorsSetException("Empty position set");

  std::vector<SeriesFactorSet> vSFSHighest, vSFSConsecutive;

  // Calculate average favorable position excursion
  for( PositionSet::const_iterator iter = _sPositions.begin(); iter != _sPositions.end(); ++iter ) {
    PositionFactors pf(*iter);
    vSFSHighest.push_back(pf.bfe()); // Best favorable excursion for this position
    vSFSConsecutive.push_back(pf.max_cons_pos()); // Maximum consecutive favorable bars for this position
  }

  // Calculate average favorable excursion
  double acc = 0;
  for( unsigned i = 0; i < vSFSHighest.size(); i++ )
    acc += vSFSHighest[i].factor();

  double avg = (acc ? (acc/vSFSHighest.size()) - 1 : 0);

  return ExcursionResults(avg,
                          *max_element(vSFSHighest.begin(), vSFSHighest.end(), SeriesFactorSetFactorLtCmp()),
                          *max_element(vSFSConsecutive.begin(), vSFSConsecutive.end(), SeriesFactorSetSizeLtCmp()));
}


PositionFactorsSet::ExcursionResults PositionFactorsSet::adverse(void) const throw(PositionFactorsSetException)
{
  if ( _sPositions.empty() )
    throw PositionFactorsSetException("Empty position set");

  std::vector<SeriesFactorSet> vSFSHighest, vSFSConsecutive;

  // Calculate average adverse position excursion
  for( PositionSet::const_iterator iter = _sPositions.begin(); iter != _sPositions.end(); ++iter ) {
    PositionFactors pf(*iter);
    vSFSHighest.push_back(pf.wae()); // Worst adverse excursion for this position
    vSFSConsecutive.push_back(pf.max_cons_neg()); // Maximum consecutive adverse bars for this position
  }

  // Calculate average adverse excursion
  double acc = 0;
  for( unsigned i = 0; i < vSFSHighest.size(); i++ )
    acc += vSFSHighest[i].factor();

  double avg = (acc ? (acc/vSFSHighest.size()) - 1 : 0);

  return ExcursionResults(avg,
                          *min_element(vSFSHighest.begin(), vSFSHighest.end(), SeriesFactorSetFactorLtCmp()),
                          *max_element(vSFSConsecutive.begin(), vSFSConsecutive.end(), SeriesFactorSetSizeLtCmp()));
}


PositionFactorsSet::ExcursionResults::ExcursionResults( double a, const SeriesFactorSet& h, const SeriesFactorSet& co ):
  avg(a),
  high(h),
  consecutive(co)
{
}
