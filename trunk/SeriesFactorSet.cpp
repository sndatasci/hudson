/*
* SeriesFactorSet.cpp
*/

#include "StdAfx.h"

// Hudson
#include "SeriesFactorSet.hpp"


SeriesFactorSet::SeriesFactorSet( void ):
  _factor(1)
{
}


bool SeriesFactorSet::insert( const SeriesFactor& sf )
{
  if( ! __SeriesFactorSet::insert(sf).second )
    return false;

  // Cache new overall factor for performance
  _factor *= sf.factor();

  return true;
}


double SeriesFactorSet::factor( void ) const throw(SeriesFactorSetException)
{
  if( empty() )
    throw SeriesFactorSetException("Invalid factor for empty set");

  return _factor;
}