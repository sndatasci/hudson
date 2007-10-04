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


double PositionFactorsSet::avg_neg_excursion( void ) const
{
  if( _sPositions.empty() )
    return 0;

  double acc = 0;
  for( PositionSet::const_iterator iter = _sPositions.begin(); iter != _sPositions.end(); ++iter ) {
    PositionFactors pf(*(*iter), _db);
    acc += pf.dd().factor();
  }

  return (acc ? (acc/_sPositions.size()) - 1 : 0);
}


double PositionFactorsSet::avg_pos_excursion( void ) const
{
  if( _sPositions.empty() )
    return 0;

  double acc = 0;
  for( PositionSet::const_iterator iter = _sPositions.begin(); iter != _sPositions.end(); ++iter ) {
    PositionFactors pf(*(*iter), _db);
    acc += pf.pk().factor();
  }

  return (acc ? (acc/_sPositions.size()) - 1 : 0);
}


SeriesFactorSet PositionFactorsSet::worst_excursion( void ) const throw(PositionFactorsSetException)
{
  if( _sPositions.empty() )
    throw PositionFactorsSetException("Empty positions set");

  vector<SeriesFactorSet> vsfs;

  for( PositionSet::const_iterator iter = _sPositions.begin(); iter != _sPositions.end(); ++iter ) {
    PositionFactors pf(*(*iter), _db);
    vsfs.push_back(pf.dd());
  }

  return *min_element(vsfs.begin(), vsfs.end(), SeriesFactorSetFactorLtCmp());
}


SeriesFactorSet PositionFactorsSet::best_excursion( void ) const throw(PositionFactorsSetException)
{
  if( _sPositions.empty() )
    throw PositionFactorsSetException("Empty positions set");

  vector<SeriesFactorSet> vsfs;

  for( PositionSet::const_iterator iter = _sPositions.begin(); iter != _sPositions.end(); ++iter ) {
    PositionFactors pf(*(*iter), _db);
    vsfs.push_back(pf.pk());
  }

  return *max_element(vsfs.begin(), vsfs.end(), SeriesFactorSetFactorLtCmp());
}


SeriesFactorSet PositionFactorsSet::max_cons_pos( void ) const throw(PositionFactorsSetException)
{
  if( _sPositions.empty() )
    throw PositionFactorsSetException("Empty positions set");

  vector<SeriesFactorSet> vSFS;

  for( PositionSet::const_iterator iter = _sPositions.begin(); iter != _sPositions.end(); ++iter ) {
    PositionFactors pf(*(*iter), _db);
    vSFS.push_back(pf.max_cons_pos());
  }

  return *max_element(vSFS.begin(), vSFS.end(), SeriesFactorSetSizeLtCmp());
}


SeriesFactorSet PositionFactorsSet::max_cons_neg( void ) const throw(PositionFactorsSetException)
{
  if( _sPositions.empty() )
    throw PositionFactorsSetException("Empty positions set");

  vector<SeriesFactorSet> vSFS;

  for( PositionSet::const_iterator iter = _sPositions.begin(); iter != _sPositions.end(); ++iter ) {
    PositionFactors pf(*(*iter), _db);
    vSFS.push_back(pf.max_cons_neg());
  }

  return *max_element(vSFS.begin(), vSFS.end(), SeriesFactorSetSizeLtCmp());
}
