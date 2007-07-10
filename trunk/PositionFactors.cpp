/*
* PositionFactors.cpp
*/

#include "StdAfx.h"

// STDLIB
#include <cmath>

// STL
#include <algorithm>

// Hudson
#include "PositionFactors.hpp"
#include "Position.hpp"

using namespace std;
using namespace Series;


PositionFactors::PositionFactors( const Position& pos, const DaySeries<DayPrice>& db ):
  _pos(pos),
  _db(db)
{
  //cout << "Initializing position " << pos.id() << " (" << (_pos.open() ? "open" : "closed") << ')' << ", entry " << pos.avgEntryPrice() << " [" << _pos.first_exec().dt() << '/' << _pos.last_exec().dt() << ']' << endl;

  // Browse all price series from the position opening and build daily factors vector
  bool first_factor = true;
  double prev_price = 0;
  double curr_price = 0;
  double f = 0;
  for( DaySeries<DayPrice>::const_iterator iter = _db.after(_pos.first_exec().dt()); iter != _db.end(); ++iter ) {

    // If position is closed we only initialize factors up to the last execution
    if( _pos.closed() && iter->first > _pos.last_exec().dt() )
      break;

    curr_price = iter->second.close;

    if( first_factor ) {
      f = _pos.factor(curr_price);
      first_factor = false;
    } else {
      f = curr_price / prev_price;
    }

    //cout << setprecision(4) << "On " << iter->first << " mark price: " << iter->second.close << ", factor: " << f << endl;
    _vFactors.push_back(f);

    prev_price = curr_price;
  }

  _mean = accumulate<doubleVector::const_iterator, double>( _vFactors.begin(), _vFactors.end(), 0 ) / _vFactors.size();
  _stddev = ::sqrt( accumulate<doubleVector::const_iterator, double>( _vFactors.begin(), _vFactors.end(), 0, variance_bf(_mean) ) / (_vFactors.size() - 1) );

  //cout << "Num Pos: " << num_pos() << endl;
  //cout << "Num Neg: " << num_neg() << endl;
  //cout << "Max Cons Pos: " << max_cons_pos() << endl;
  //cout << "Max Cons Neg: " << max_cons_neg() << endl;
  //cout << "Best: " << best() << endl;
  //cout << "Worst: " << worst() << endl;
  //cout << "Drawdown: " << dd() << endl;
  //cout << "Peak: " << pk() << endl;
}


double PositionFactors::avg( void ) const
{
  return _mean - 1;
}


double PositionFactors::stddev( void ) const
{
  return _stddev;
}


double PositionFactors::best( void ) const
{
  return *max_element(_vFactors.begin(), _vFactors.end());
}


double PositionFactors::worst( void ) const
{
  return *min_element(_vFactors.begin(), _vFactors.end());
}


int PositionFactors::max_cons_pos( void ) const
{
  vector<unsigned> cons;

  for( unsigned i = 0; i < _vFactors.size(); i++ ) {

    // Filter out negative or break-even factors
    if( _vFactors[i] <= 1 )
      continue;

    // Look for positive sequence
    unsigned counter = 0;
    while( i < _vFactors.size() && _vFactors[i] > 1 ) {
      ++counter;
      ++i;
    }
    cons.push_back(counter);
  }

  if( cons.empty() )
    return 0;

  return *max_element(cons.begin(), cons.end());
}


int PositionFactors::max_cons_neg( void ) const
{
  vector<unsigned> cons;

  for( unsigned i = 0; i < _vFactors.size(); i++ ) {

    // Filter out positive or break-even factors
    if( _vFactors[i] >= 1 )
      continue;

    // Look for negative sequence
    unsigned counter = 0;
    while( i < _vFactors.size() && _vFactors[i] < 1 ) {
      ++counter;
      ++i;
    }
    cons.push_back(counter);
  }

  if( cons.empty() )
    return 0;

  return *max_element(cons.begin(), cons.end());
}


unsigned PositionFactors::num_pos(void) const
{
  unsigned counter = 0;
  for( unsigned i = 0; i < _vFactors.size(); ++i )
    if( _vFactors[i] > 1 )
      ++counter;

  return counter;
}


unsigned PositionFactors::num_neg(void) const
{
  unsigned counter = 0;
  for( unsigned i = 0; i < _vFactors.size(); ++i )
    if( _vFactors[i] < 1 )
      ++counter;

  return counter;
}


double PositionFactors::dd( void ) const
{
  doubleVector vDD; // all drawdowns

  // calculate drawdown from each factor
  for( unsigned i = 0; i != _vFactors.size(); ++i )
    vDD.push_back(_dd(i)); // add drawdown from this point

  if( vDD.empty() )
    return 1;

  // return highest drawdown from all calculated
  return *min_element(vDD.begin(), vDD.end());
}


double PositionFactors::_dd(unsigned start) const
{
  double acc = 1;
  double dd = 1;

  for( unsigned i = start; i < _vFactors.size(); ++i ) {

    acc *= _vFactors[i];

    if( acc < dd )
      dd = acc;
  }

  return dd;
}


double PositionFactors::pk(void) const
{
  doubleVector vPK; // all peaks

  // calculate peak from each factor
  for( unsigned i = 0; i != _vFactors.size(); ++i )
    vPK.push_back(_pk(i));

  if( vPK.empty() )
    return 1;

  return *max_element(vPK.begin(), vPK.end());
}


double PositionFactors::_pk(unsigned start) const
{
  double acc = 1;
  double pk = 1;

  for( unsigned i = start; i < _vFactors.size(); ++i ) {

    acc *= _vFactors[i];

    if( acc > pk )
      pk = acc;
  }

  return pk;
}
