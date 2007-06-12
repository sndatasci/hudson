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


ReturnFactors::ReturnFactors(const PositionSet& sPositions, unsigned days, unsigned yf) throw (ReturnFactorsException):
  _sPositions(sPositions),
  _days(days),
  _yperiods(yf)
{
  if( sPositions.empty() )
    throw ReturnFactorsException("Empty positions set");

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
  return _fvalue - 1;
}


double ReturnFactors::avg(void) const
{
  return _mean - 1;
}


double ReturnFactors::dd(void) const
{
  vector<double> vdd; // calculated drawdowns

  // calculate drawdown from each factor point
  for( unsigned i = 0; i < _vFactors.size(); i++ )
	  vdd.push_back(_dd(i));

  // return highest drawdown from all calculated
  return (*min_element(vdd.begin(), vdd.end()));
}


double ReturnFactors::_dd(int start) const
{
  double acc = 1;
  double my_dd = 1;
  for( unsigned i = start; i < _vFactors.size(); i++ ) {
	  acc *= _vFactors[i];
	  if( acc < my_dd )
	    my_dd = acc;
  }

  return my_dd;
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
  return ::pow(_fvalue, 1/_years) - 1;
}


double ReturnFactors::gsd(void) const
{
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

  remove_copy_if( _sPositions.begin(), _sPositions.end(), // source
				          inserter(sPosPositions, sPosPositions.end()), // destination
				          bind2nd(PositionLt(), 1.0) ); // negative condition

  return sPosPositions;
}


PositionSet ReturnFactors::neg(void) const
{
  PositionSet sNegPositions;

  remove_copy_if( _sPositions.begin(), _sPositions.end(), // source vector
				          inserter(sNegPositions, sNegPositions.end()), // destination
				          bind2nd(PositionGt(), 1.0)); // condition

  return sNegPositions;
}


size_t ReturnFactors::num(void) const
{
  return _vFactors.size();
}


unsigned ReturnFactors::max_cons_pos(void) const
{
  if( _vFactors.empty() ) return 0;

  vector<unsigned> cons;
  unsigned pos = 0;

  cons.push_back(0);
  for( unsigned i = 0; i < _vFactors.size(); i++ ) { // for each factor
	  while( _vFactors[i] > 1 && i < _vFactors.size() ) { // add-up consecutive positive factors
	    ++pos;
	    ++i;
	  }

	  if( pos ) { // if positive factors
	    cons.push_back(pos); // store total number of consecutive positive factors
	    pos = 0;
	  }
  }

  return *max_element(cons.begin(), cons.end());
}


unsigned ReturnFactors::max_cons_neg(void) const
{
  if( _vFactors.empty() )
    return 0;

  vector<unsigned> cons;
  unsigned neg = 0;

  cons.push_back(0);
  for( unsigned i = 0; i < _vFactors.size(); i++ ) {
	  while( _vFactors[i] < 1 && i < _vFactors.size() ) {
	    ++neg;
	    ++i;
	  }

	  if( neg ) {
	    cons.push_back(neg);
	    neg = 0;
	  }
  }

  return *max_element(cons.begin(), cons.end());
}
