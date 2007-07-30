/*
* SeriesFactorSet.cpp
*/

#include "StdAfx.h"

// Hudson
#include "SeriesFactorSet.hpp"


double SeriesFactorSet::factor( void ) const
{
  double acc = 1;
  for( const_iterator iter = begin(); iter != end(); ++iter )
    acc *= (*iter).factor();

  return acc;
}
