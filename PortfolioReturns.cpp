/*
* PortfolioReturns.cpp
*/

#include "StdAfx.h"

// Hudson
#include "PortfolioReturns.hpp"

using namespace std;


PortfolioReturns::PortfolioReturns( void )
{
}


double PortfolioReturns::roi( void ) const
{
  if( _vRF.empty() )
    return 0;

  double acc_cagr = 0;
  for( size_t i = 0; i < _vRF.size(); ++i )
    acc_cagr += _vRF[i]->roi();

  return acc_cagr/_vRF.size();
}


void PortfolioReturns::add( const EOMReturnFactors* rf)
{
  _vRF.push_back(rf);
}


double PortfolioReturns::cagr( void ) const
{
  if( _vRF.empty() )
    return 0;

  double acc_cagr = 0;
  for( size_t i = 0; i < _vRF.size(); ++i )
    acc_cagr += _vRF[i]->cagr();

  return acc_cagr/_vRF.size();
}


double PortfolioReturns::gsd( void ) const
{
  if( _vRF.empty() )
    return 0;

  double acc_cagr = 0;
  for( size_t i = 0; i < _vRF.size(); ++i )
    acc_cagr += _vRF[i]->gsd();

  return acc_cagr/_vRF.size();
}

