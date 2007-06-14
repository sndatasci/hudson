/*
 * ReturnFactors.hpp
 */

#include "StdAfx.h"

// STDLIB
#include <cmath>

// STL
#include <numeric>
#include <algorithm>

// Boost
#include <boost/lambda/lambda.hpp>

// GSL
#include <gsl/gsl_statistics_double.h>

// Hudson
#include "ReturnFactors.hpp"


using namespace std;
namespace lmb = boost::lambda;


ReturnFactors::ReturnFactors(const PositionSet& sPositions, unsigned days, unsigned yf):
  _sPositions(sPositions),
  _days(days),
  _yperiods(yf)
{
  _years = (days == 0 ? 0 : days/(double)365);

  // Initialize time-ordered position factors by last execution (position close)
  for( position_by_last_exec::iterator iter = _sPositions.get<last_exec_key>().begin(); iter != _sPositions.get<last_exec_key>().end(); ++iter )
    _vFactors.push_back((*iter)->factor());

  // Initialize factor logs vector
  transform(_vFactors.begin(), _vFactors.end(), back_insert_iterator<doubleVector>(_vLogFactors), log10_uf());

  _fvalue = accumulate<doubleVector::const_iterator, double>( _vFactors.begin(), _vFactors.end(), 1, lmb::_1 * lmb::_2 );
  _mean = accumulate<doubleVector::const_iterator, double>( _vFactors.begin(), _vFactors.end(), 0 ) / _vFactors.size();
  _stddev = ::sqrt( accumulate<doubleVector::const_iterator, double>( _vFactors.begin(), _vFactors.end(), 0, variance_bf(_mean) ) / (_vFactors.size() - 1) );
}


ReturnFactors::~ReturnFactors()
{
}


double ReturnFactors::roi(void) const
{
  return _sPositions.empty() ? 0 : _fvalue - 1;
}


double ReturnFactors::avg(void) const
{
  return _sPositions.empty() ? 0 : _mean - 1;
}


double ReturnFactors::stddev(void) const
{
  return _stddev;
}


double ReturnFactors::skew(void) const
{
  if( _vFactors.size() < 2 ) return 0;

  //  return ( accumulate<vdouble::const_iterator, double>( _vf.begin(), _vf.end(), 0, skew1(_mean) ) ) /
  //((_vf.size() - 1) * pow(_stddev, 3));
  return gsl_stats_skew_m_sd(&_vFactors[0], 1, _vFactors.size(), _mean, _stddev);
}


double ReturnFactors::cagr(void) const
{
  return _sPositions.empty() ? 0 : std::pow(_fvalue, 1/_years) - 1;
}


double ReturnFactors::gsd(void) const
{
  if( _sPositions.empty() )
    return 0;

  double lfavg = accumulate<doubleVector::const_iterator, double>( _vLogFactors.begin(), _vLogFactors.end(), 0 ) / _vLogFactors.size();
  double lfstddev = ::sqrt( accumulate<doubleVector::const_iterator, double>( _vLogFactors.begin(), _vLogFactors.end(), 0, variance_bf(lfavg) ) / (_vLogFactors.size() - 1) );
  return ::pow( (double)10, lfstddev * ::sqrt((double)_yperiods) ) - 1;
}


const Position& ReturnFactors::best(void) const
{
  return **max_element(_sPositions.begin(), _sPositions.end(), PositionLtCmp());
}


const Position& ReturnFactors::worst(void) const
{
  return **min_element(_sPositions.begin(), _sPositions.end(), PositionLtCmp());
}


PositionSet ReturnFactors::pos(void) const
{
  PositionSet sPosPositions;

  for( PositionSet::const_iterator iter = _sPositions.begin(); iter != _sPositions.end(); ++iter )
    if( (*iter)->factor() > 1 )
      sPosPositions.insert(*iter);

  return sPosPositions;
}


PositionSet ReturnFactors::neg(void) const
{
  PositionSet sNegPositions;

  for( PositionSet::const_iterator iter = _sPositions.begin(); iter != _sPositions.end(); ++iter )
    if( (*iter)->factor() < 1 )
      sNegPositions.insert(*iter);

  return sNegPositions;
}


size_t ReturnFactors::num(void) const
{
  return _sPositions.size();
}


PositionSet ReturnFactors::max_cons_pos(void) const
{
  vector<PositionSet> cons;
  cons.push_back(PositionSet()); // At least one position set for max_element to work
  
  position_by_last_exec::iterator iter = _sPositions.get<last_exec_key>().begin();
  while( iter != _sPositions.get<last_exec_key>().end() ) {
    // Filter out negative factors
    if( (*iter)->factor() <= 1 ) {
      ++iter;
      continue;
    }

    // Positive factor, look for positive sequence
    PositionSet pset;
    while( iter != _sPositions.get<last_exec_key>().end() && (*iter)->factor() > 1 ) {
      pset.insert(*iter);
      ++iter;
    }

    // Store sequence
    cons.push_back(pset);
  }

  return *max_element(cons.begin(), cons.end(), PositionSetSizeCmp());
}


PositionSet ReturnFactors::max_cons_neg(void) const
{
  vector<PositionSet> cons;
  cons.push_back(PositionSet()); // At least one position set for max_element to work
  
  position_by_last_exec::iterator iter = _sPositions.get<last_exec_key>().begin();
  while( iter != _sPositions.get<last_exec_key>().end() ) {
    // Filter out positive factors
    if( (*iter)->factor() >= 1 ) {
      ++iter;
      continue;
    }

    // Negative factor, look for negative sequence
    PositionSet pset;
    while( iter != _sPositions.get<last_exec_key>().end() && (*iter)->factor() < 1 ) {
      pset.insert(*iter);
      ++iter;
    }

    // Store sequence
    cons.push_back(pset);
  }

  return *max_element(cons.begin(), cons.end(), PositionSetSizeCmp());
}


PositionSet ReturnFactors::dd(void) const
{
  if( _sPositions.empty() )
    return PositionSet();

  vector<PositionSet> dd; // all drawdowns
  dd.push_back(PositionSet()); // at least one position set for max_element to work

  // calculate drawdown from each 
  for( position_by_last_exec::iterator iter = _sPositions.get<last_exec_key>().begin(); iter != _sPositions.get<last_exec_key>().end(); ++iter )
    dd.push_back(_dd(iter)); // add drawdown from this point

  // return highest drawdown from all calculated
  return *min_element(dd.begin(), dd.end(), PositionSetRealizedCmp());
}


PositionSet ReturnFactors::_dd(position_by_last_exec::iterator& start) const
{
  PositionSet pset, dd_pset;
  double acc = 1;
  double my_dd = 1;

  for( position_by_last_exec::iterator iter = start; iter != _sPositions.get<last_exec_key>().end(); ++iter ) {
    pset.insert(*iter);
    acc *= (*iter)->factor();

    if( acc < my_dd ) {
      my_dd = acc;
      dd_pset = pset;
    }
  }

  return dd_pset;
}
