/*
 * PositionFactors.hpp
 */

// STL
#include <numeric>
#include <algorithm>

// Boost
#include <boost/lambda/lambda.hpp>

// GSL
#include <gsl/gsl_statistics_double.h>

#include "PositionFactors.hpp"

using namespace std;
namespace lmb = boost::lambda;


PositionFactors::PositionFactors(const PositionSet& ps):
  _ps(ps)
{
  _fv = accumulate<PositionSet::const_iterator, Position*>( _ps.begin(), _ps.end(), 1, acc1() );
  _mean = accumulate<PositionSet::const_iterator, Position*>( _ps.begin(), _ps.end(), 0, acc1() ) / _ps.size();
  _stddev = ::sqrt( accumulate<PositionSet::const_iterator, Position*>( _ps.begin(), _ps.end(), 0, variance1(_mean) ) / (_ps.size() - 1) );
}


PositionFactors::~PositionFactors()
{
}


double PositionFactors::roi(void) const
{
  return _fv - 1;
}


double PositionFactors::avg(void) const
{
  if( _ps.empty() ) return 0;

  return _mean - 1;
}


double PositionFactors::stddev(void) const
{
  if( _ps.empty() )	return 0;

  return _stddev;
}

/*
double PositionFactors::skew(void) const
{
  if( _vf.size() < 2 ) return 0;

  //  return ( accumulate<vdouble::const_iterator, double>( _vf.begin(), _vf.end(), 0, skew1(_mean) ) ) /
  //((_vf.size() - 1) * pow(_stddev, 3));
  return gsl_stats_skew_m_sd(&_vf[0], 1, _vf.size(), _mean, _stddev);
}


double PositionFactors::best(void) const
{
  if( _vf.empty() ) return 0;

  return (*max_element(_vf.begin(), _vf.end())) - 1;
}


double PositionFactors::worst(void) const
{
  if( _vf.empty() ) return 0;

  return (*min_element(_vf.begin(), _vf.end())) - 1;
}


PositionFactors::vdouble PositionFactors::pos(void) const
{
  vdouble vf_pos;

  remove_copy_if( _vf.begin(), _vf.end(), // source
				  inserter(vf_pos, vf_pos.end()), // destination
				  bind2nd(less<double>(), 1) ); // negative condition

  return vf_pos;
}


PositionFactors::vdouble PositionFactors::neg(void) const
{
  vdouble vf_neg;

  remove_copy_if( _vf.begin(), _vf.end(), // source vector
				  inserter(vf_neg, vf_neg.end()), // destination
				  bind2nd(greater<double>(), 1)); // condition

  return vf_neg;
}


unsigned PositionFactors::num(void) const
{
  return _vf.size();
}


unsigned PositionFactors::max_cons_pos(void) const
{
  if( _vf.empty() ) return 0;

  vector<unsigned> cons;
  unsigned pos = 0;

  cons.push_back(0);
  for( int i = 0; i < _vf.size(); i++ ) {
	while( _vf[i] > 1.0 && i < _vf.size() ) {
	  ++pos;
	  ++i;
	}

	if( pos ) {
	  cons.push_back(pos);
	  pos = 0;
	}
  }

  return *max_element(cons.begin(), cons.end());
}


unsigned PositionFactors::max_cons_neg(void) const
{
  if( _vf.empty() ) return 0;

  vector<unsigned> cons;
  unsigned neg = 0;

  cons.push_back(0);
  for( int i = 0; i < _vf.size(); i++ ) {
	while( _vf[i] < (double)1.0 && i < _vf.size() ) {
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


double PositionFactors::dd(void) const
{
  vector<double> vdd;
  for( int i = 0; i < _vf.size(); i++ )
	vdd.push_back(_dd(i));

  return (*min_element(vdd.begin(), vdd.end()));
}


double PositionFactors::_dd(int start) const
{
  double acc = 1;
  double my_dd = 1;
  for( int i = start; i < _vf.size(); i++ ) {
	acc *= _vf[i];
	if( acc < my_dd )
	  my_dd = acc;
  }

  return my_dd;
}
*/
