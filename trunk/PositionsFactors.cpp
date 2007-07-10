/*
* PositionsFactors.cpp
*/

#include <StdAfx.h>

// STL
#include <vector>
#include <algorithm>

// Hudson
#include "PositionsFactors.hpp"
#include "PositionFactors.hpp"

using namespace std;


PositionsFactors::PositionsFactors( const PositionSet& sPositions, const Series::DaySeries<Series::DayPrice>& db ):
  _sPositions(sPositions),
  _db(db)
{
}


double PositionsFactors::avg_neg_excursion( void ) const
{
  double acc = 0;
  for( PositionSet::const_iterator iter = _sPositions.begin(); iter != _sPositions.end(); ++iter ) {
    PositionFactors pf(*(*iter), _db);
    acc += pf.dd();
  }

  return (acc ? (acc/_sPositions.size()) -1 : 0);
}


double PositionsFactors::avg_pos_excursion( void ) const
{
  double acc = 0;
  for( PositionSet::const_iterator iter = _sPositions.begin(); iter != _sPositions.end(); ++iter ) {
    PositionFactors pf(*(*iter), _db);
    acc += pf.pk();
  }

  return (acc ? (acc/_sPositions.size()) - 1 : 0);
}


double PositionsFactors::worst_excursion( void ) const
{
  vector<double> vPosDD;

  for( PositionSet::const_iterator iter = _sPositions.begin(); iter != _sPositions.end(); ++iter ) {
    PositionFactors pf(*(*iter), _db);
    vPosDD.push_back(pf.dd());
  }

  return (*min_element(vPosDD.begin(), vPosDD.end()))-1;
}


double PositionsFactors::best_excursion( void ) const
{
  vector<double> vPosPK;

  for( PositionSet::const_iterator iter = _sPositions.begin(); iter != _sPositions.end(); ++iter ) {
    PositionFactors pf(*(*iter), _db);
    vPosPK.push_back(pf.pk());
  }

  return (*max_element(vPosPK.begin(), vPosPK.end()))-1;
}
