/*
* PositionFactorsSet.cpp
*/

#include <StdAfx.h>

// STL
#include <vector>
#include <algorithm>

// Hudson
#include "PositionFactorsSet.hpp"
#include "PositionFactors.hpp"

using namespace std;


PositionFactorsSet::PositionFactorsSet( const PositionSet& sPositions, const Series::EODSeries& db ):
  _sPositions(sPositions),
  _db(db)
{
}


PositionFactorsSet::ExcursionResults PositionFactorsSet::favorable(void) const throw(PositionFactorsSetException)
{
  if ( _sPositions.empty() )
    throw PositionFactorsSetException("Empty position set");

  ExcursionResults ret;
  std::vector<SeriesFactorSet> vSFSHighest, vSFSConsecutive;

  // Calculate average favorable position excursion
  for( PositionSet::const_iterator iter = _sPositions.begin(); iter != _sPositions.end(); ++iter ) {
    PositionFactors pf(*(*iter), _db);
    vSFSHighest.push_back(pf.bfe()); // Best favorable excursion for this position
    vSFSConsecutive.push_back(pf.max_cons_pos()); // Maximum consecutive favorable bars for this position
  }

  // Calculate average favorable excursion
  double acc = 0;
  for( unsigned i = 0; i < vSFSHighest.size(); i++ )
    acc += vSFSHighest[i].factor();

  ret.avg = (acc ? (acc/vSFSHighest.size()) - 1 : 0);

  // Set best favorable excursion
  ret.high = *max_element(vSFSHighest.begin(), vSFSHighest.end(), SeriesFactorSetFactorLtCmp());

  // Set longest favorable excursion
  ret.consecutive = *max_element(vSFSConsecutive.begin(), vSFSConsecutive.end(), SeriesFactorSetSizeLtCmp());

  return ret;
}


PositionFactorsSet::ExcursionResults PositionFactorsSet::adverse(void) const throw(PositionFactorsSetException)
{
  if ( _sPositions.empty() )
    throw PositionFactorsSetException("Empty position set");

  ExcursionResults ret;
  std::vector<SeriesFactorSet> vSFSHighest, vSFSConsecutive;

  // Calculate average adverse position excursion
  for( PositionSet::const_iterator iter = _sPositions.begin(); iter != _sPositions.end(); ++iter ) {
    PositionFactors pf(*(*iter), _db);
    vSFSHighest.push_back(pf.wae()); // Worst adverse excursion for this position
    vSFSConsecutive.push_back(pf.max_cons_neg()); // Maximum consecutive adverse bars for this position
  }

  // Calculate average adverse excursion
  double acc = 0;
  for( unsigned i = 0; i < vSFSHighest.size(); i++ )
    acc += vSFSHighest[i].factor();

  ret.avg = (acc ? (acc/vSFSHighest.size()) - 1 : 0);

  // Set worst adverse excursion
  ret.high = *min_element(vSFSHighest.begin(), vSFSHighest.end(), SeriesFactorSetFactorLtCmp());

  // Set longest adverse excursion
  ret.consecutive = *max_element(vSFSConsecutive.begin(), vSFSConsecutive.end(), SeriesFactorSetSizeLtCmp());

  return ret;
}


PositionFactorsSet::ExcursionResults::ExcursionResults( void ):
  avg(0)
{
}