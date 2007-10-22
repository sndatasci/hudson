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

#include "StdAfx.h"

// Hudson
#include "PortfolioReturns.hpp"

using namespace std;


PortfolioReturns::PortfolioReturns( void )
{
}


void PortfolioReturns::add( const EOMReturnFactors* rf)
{
  _vRF.push_back(rf);
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

