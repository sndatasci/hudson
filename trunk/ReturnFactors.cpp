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
#include "PositionFactors.hpp"


using namespace std;
namespace lmb = boost::lambda;

using namespace Series;


ReturnFactors::ReturnFactors(const PositionSet& sPositions, const EODSeries& db):
  _sPositions(sPositions),
  _db(db),
  _last_close(0),
  _last_open(0),
  _yperiods(12),
  _fvalue(0),
  _mean(0),
  _stddev(0)
{
  _sClosedPositions = _sPositions.closed();
  _sOpenPositions = _sPositions.open();

  _days = _db.duration().days();
  _years = (_days == 0 ? 0 : _days/(double)365);

  // Last bar prices for open position factors
  if( _db.empty() )
    return;

  _last_open = _db.rbegin()->second.open;
  _last_close = _db.rbegin()->second.close;

  if( _sPositions.empty() )
    return;

  // Initialize time-ordered position factors by last execution (position close)
  for( PositionSet::by_last_exec::iterator iter = _sPositions.get<last_exec_key>().begin(); iter != _sPositions.get<last_exec_key>().end(); ++iter )
    _vFactors.push_back( pfactor(*(*iter), _last_close) );

  // Initialize factor logs vector
  transform(_vFactors.begin(), _vFactors.end(), back_insert_iterator<doubleVector>(_vLogFactors), log10_uf());

  _fvalue = accumulate<doubleVector::const_iterator, double>( _vFactors.begin(), _vFactors.end(), 1, lmb::_1 * lmb::_2 );
  _mean = accumulate<doubleVector::const_iterator, double>( _vFactors.begin(), _vFactors.end(), 0 ) / _vFactors.size();
  _stddev = ::sqrt( accumulate<doubleVector::const_iterator, double>( _vFactors.begin(), _vFactors.end(), 0, variance_bf(_mean) ) / (_vFactors.size() - 1) );
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


const Position& ReturnFactors::best(void) const throw(ReturnFactorsException)
{
  if( _sPositions.empty() )
    throw ReturnFactorsException("Empty positions set");

  return **max_element(_sPositions.begin(), _sPositions.end(), PositionLtCmp(_last_close));
}


const Position& ReturnFactors::worst(void) const throw(ReturnFactorsException)
{
  if( _sPositions.empty() )
    throw ReturnFactorsException("Empty positions set");

  return **min_element(_sPositions.begin(), _sPositions.end(), PositionLtCmp(_last_close));
}


PositionSet ReturnFactors::pos(void) const
{
  PositionSet sPosPositions;

  for( PositionSet::const_iterator iter = _sPositions.begin(); iter != _sPositions.end(); ++iter )
    if( pfactor(*(*iter), _last_close) > 1 )
      sPosPositions.insert(*iter);

  return sPosPositions;
}


PositionSet ReturnFactors::neg(void) const
{
  PositionSet sNegPositions;

  for( PositionSet::const_iterator iter = _sPositions.begin(); iter != _sPositions.end(); ++iter )
    if( pfactor(*(*iter), _last_close) < 1 )
      sNegPositions.insert(*iter);

  return sNegPositions;
}


int ReturnFactors::num(void) const
{
  return (int)_sPositions.size();
}


PositionSet ReturnFactors::max_cons_pos(void) const throw(ReturnFactorsException)
{
  if( _sClosedPositions.empty() )
    throw ReturnFactorsException("Empty positions set");
 
  vector<PositionSet> cons;

  PositionSet::by_last_exec::iterator iter = _sClosedPositions.get<last_exec_key>().begin();
  while( iter != _sClosedPositions.get<last_exec_key>().end() ) {
    // Filter out negative factors
    if( pfactor(*(*iter), _last_close) <= 1 ) {
      ++iter;
      continue;
    }

    // Positive factor, look for positive sequence
    PositionSet pset;
    while( iter != _sClosedPositions.get<last_exec_key>().end() && pfactor(*(*iter), _last_close) > 1 ) {
      pset.insert(*iter);
      ++iter;
    }

    // Store sequence
    cons.push_back(pset);
  }

  if( cons.empty() )
    return PositionSet(); // max_element crashes on empty set

  return *max_element(cons.begin(), cons.end(), PositionSetSizeCmp());
}


PositionSet ReturnFactors::max_cons_neg(void) const throw(ReturnFactorsException)
{
  if( _sClosedPositions.empty() )
    throw ReturnFactorsException("Empty positions set");

  vector<PositionSet> cons;
  
  PositionSet::by_last_exec::iterator iter = _sClosedPositions.get<last_exec_key>().begin();
  while( iter != _sClosedPositions.get<last_exec_key>().end() ) {
    // Filter out positive factors
    if( pfactor(*(*iter), _last_close) >= 1 ) {
      ++iter;
      continue;
    }

    // Negative factor, look for negative sequence
    PositionSet pset;
    while( iter != _sClosedPositions.get<last_exec_key>().end() && pfactor(*(*iter), _last_close) < 1 ) {
      pset.insert(*iter);
      ++iter;
    }

    // Store sequence
    cons.push_back(pset);
  }

  if( cons.empty() )
    return PositionSet(); // max_element crashes on empty set

  return *max_element(cons.begin(), cons.end(), PositionSetSizeCmp());
}


PositionSet ReturnFactors::dd(void) const throw(ReturnFactorsException)
{
  if( _sPositions.empty() )
    throw ReturnFactorsException("Empty positions set");

  vector<PositionSet> dd; // all drawdowns
  // calculate drawdown from each position
  for( PositionSet::by_last_exec::iterator iter = _sPositions.get<last_exec_key>().begin(); iter != _sPositions.get<last_exec_key>().end(); ++iter )
    dd.push_back(_dd(iter)); // add drawdown from this point

  if( dd.empty() )
    return PositionSet(); // min_element() crashes on empty vector

  // return highest drawdown
  return *min_element(dd.begin(), dd.end(), PositionSetRealizedCmp());
}


PositionSet ReturnFactors::_dd(PositionSet::by_last_exec::iterator& start) const
{
  PositionSet pset, dd_pset;
  double acc = 1;
  double my_dd = 1;

  for( PositionSet::by_last_exec::iterator iter = start; iter != _sPositions.get<last_exec_key>().end(); ++iter ) {
    pset.insert(*iter);
    acc *= pfactor(*(*iter), _last_close);

    if( acc < my_dd ) {
      my_dd = acc;
      dd_pset = pset;
    }
  }

  return dd_pset;
}
