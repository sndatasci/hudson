/*
 * ReturnFactors.hpp
 */

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


template <class Arg, class Res>
struct folog10: public std::unary_function<Arg, Res>
{
  Res operator() (Arg x) const {
    return std::log10(x);
  }
};


ReturnFactors::ReturnFactors(const vdouble& vf, unsigned days, unsigned yf):
  _vf(vf),
  _days(days),
  _yf(yf)
{
  _years = (days == 0 ? 0 : days/(double)365);
  _fv = accumulate<vdouble::const_iterator, double>( _vf.begin(), _vf.end(), 1, lmb::_1 * lmb::_2 );
  _mean = accumulate<vdouble::const_iterator, double>( _vf.begin(), _vf.end(), 0 ) / _vf.size();
  _stddev = ::sqrt( accumulate<vdouble::const_iterator, double>( _vf.begin(), _vf.end(), 0, variance1(_mean) ) / (_vf.size() - 1) );

  transform(_vf.begin(), _vf.end(), _vlf.begin(), folog10<double, double>());
}


ReturnFactors::~ReturnFactors()
{
}


double ReturnFactors::roi(void) const
{
  return _fv - 1;
}


double ReturnFactors::avg(void) const
{
  if( _vf.empty() ) return 0;

  return _mean - 1;
}


double ReturnFactors::dd(void) const
{
  if( _vf.empty() ) return 0;

  vector<double> vdd;
  for( unsigned i = 0; i < _vf.size(); i++ )
	vdd.push_back(_dd(i));

  return (*min_element(vdd.begin(), vdd.end()));
}


double ReturnFactors::_dd(int start) const
{
  double acc = 1;
  double my_dd = 1;
  for( unsigned i = start; i < _vf.size(); i++ ) {
	acc *= _vf[i];
	if( acc < my_dd )
	  my_dd = acc;
  }

  return my_dd;
}


double ReturnFactors::stddev(void) const
{
  if( _vf.empty() )	return 0;

  return _stddev;
}


double ReturnFactors::skew(void) const
{
  if( _vf.size() < 2 ) return 0;

  //  return ( accumulate<vdouble::const_iterator, double>( _vf.begin(), _vf.end(), 0, skew1(_mean) ) ) /
  //((_vf.size() - 1) * pow(_stddev, 3));
  return gsl_stats_skew_m_sd(&_vf[0], 1, _vf.size(), _mean, _stddev);
}


double ReturnFactors::cagr(void) const
{
  if( _years == 0 ) return 0;

  return ::pow(_fv, 1/_years) - 1;
}


double ReturnFactors::gsd(void) const
{
  if( _vlf.empty() ) return 0;

  double lfavg = accumulate<vdouble::const_iterator, double>( _vlf.begin(), _vlf.end(), 0 ) / _vlf.size();
  double lfstddev = ::sqrt( accumulate<vdouble::const_iterator, double>( _vlf.begin(), _vlf.end(), 0, variance1(lfavg) ) / (_vlf.size() - 1) );
  return ::pow( (double)10, lfstddev * ::sqrt((double)_yf)) - 1;
}


double ReturnFactors::best(void) const
{
  if( _vlf.empty() ) return 0;

  return (*max_element(_vf.begin(), _vf.end())) - 1;
}


double ReturnFactors::worst(void) const
{
  if( _vlf.empty() ) return 0;

  return (*min_element(_vf.begin(), _vf.end())) - 1;
}


ReturnFactors::vdouble ReturnFactors::pos(void) const
{
  vdouble vf_pos;

  remove_copy_if( _vf.begin(), _vf.end(), // source
				  inserter(vf_pos, vf_pos.end()), // destination
				  bind2nd(less<double>(), 1) ); // negative condition

  return vf_pos;
}


ReturnFactors::vdouble ReturnFactors::neg(void) const
{
  vdouble vf_neg;

  remove_copy_if( _vf.begin(), _vf.end(), // source vector
				  inserter(vf_neg, vf_neg.end()), // destination
				  bind2nd(greater<double>(), 1)); // condition

  return vf_neg;
}


size_t ReturnFactors::num(void) const
{
  return _vf.size();
}


unsigned ReturnFactors::max_cons_pos(void) const
{
  if( _vlf.empty() ) return 0;

  vector<unsigned> cons;
  unsigned pos = 0;

  cons.push_back(0);
  for( unsigned i = 0; i < _vf.size(); i++ ) {
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


unsigned ReturnFactors::max_cons_neg(void) const
{
  if( _vlf.empty() ) return 0;

  vector<unsigned> cons;
  unsigned neg = 0;

  cons.push_back(0);
  for( unsigned i = 0; i < _vf.size(); i++ ) {
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
