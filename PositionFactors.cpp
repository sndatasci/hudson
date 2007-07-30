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
#include "SeriesFactorSet.hpp"

using namespace std;
using namespace boost::posix_time;
using namespace boost::gregorian;
using namespace Series;


PositionFactors::PositionFactors( const Position& pos, const DaySeries& db ):
  _pos(pos),
  _db(db)
{
  //cout << "Initializing position " << pos.id() << " (" << (_pos.open() ? "open" : "closed") << ')' << ", entry " << pos.avgEntryPrice() << " [" << _pos.first_exec().dt() << '/' << _pos.last_exec().dt() << ']' << endl;

  // Browse all price series from the position opening and build daily factors
  double prev_price = _pos.avgEntryPrice();
  double curr_price = 0;

  date prev_date = _pos.first_exec().dt();
  date curr_date;

  // Initialize all factors until the end of the series database
  for( DaySeries::const_iterator iter = _db.after(_pos.first_exec().dt()); iter != _db.end(); ++iter ) {

    // If position is closed we only initialize factors up to the last execution
    if( _pos.closed() && iter->first > _pos.last_exec().dt() )
      break;

    // Get current price mark
    curr_price = iter->second.close;
    curr_date  = iter->first;

    // Calculate long/short factor
    double f = _pos.factor(prev_price, curr_price);

    // Initialize doubles vector
    _vFactors.push_back(f);

    //cout << setprecision(4) << "On " << iter->first << " mark price: " << curr_price << ", factor: " << f << endl;
    _sfs.insert(SeriesFactor(ptime(prev_date), ptime(curr_date), f));

    prev_price = curr_price;
    prev_date = curr_date;
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


const SeriesFactor& PositionFactors::best( void ) const
{
  return *max_element(_sfs.begin(), _sfs.end());
}


const SeriesFactor& PositionFactors::worst( void ) const
{
  return *min_element(_sfs.begin(), _sfs.end());
}


SeriesFactorSet PositionFactors::max_cons_pos(void) const
{
  vector<SeriesFactorSet> cons;

  series_factor_by_end_mark::iterator iter = _sfs.get<to_key>().begin();
  while( iter != _sfs.get<to_key>().end() ) {
    if( (*iter).factor() <= 1 ) {
      ++iter;
      continue;
    }

    // Positive factor, look for positive sequence
    SeriesFactorSet sfset;
    while( iter != _sfs.get<to_key>().end() && (*iter).factor() > 1 ) {
      sfset.insert(*iter);
      ++iter;
    }

    cons.push_back(sfset);
  }

  if( cons.empty() )
    return SeriesFactorSet(); // max_element() crashes on empty collection

  return *max_element(cons.begin(), cons.end(), SeriesFactorSetSizeCmp());
}


SeriesFactorSet PositionFactors::max_cons_neg(void) const
{
  vector<SeriesFactorSet> cons;

  series_factor_by_end_mark::iterator iter = _sfs.get<to_key>().begin();
  while ( iter != _sfs.get<to_key>().end() ) {
    if( (*iter).factor() >= 1 ) {
      ++iter;
      continue;
    }

    // Negative factor, look for negative sequence
    SeriesFactorSet sfset;
    while( iter != _sfs.get<to_key>().end() && (*iter).factor() < 1 ) {
      sfset.insert(*iter);
      ++iter;
    }

    cons.push_back(sfset);
  }

  if( cons.empty() )
    return SeriesFactorSet();

  return *max_element(cons.begin(), cons.end(), SeriesFactorSetSizeCmp());
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


SeriesFactorSet PositionFactors::dd( void ) const
{
  vector<SeriesFactorSet> vsfs;

  // store drawdown from each series factor
  for( series_factor_by_end_mark::iterator iter = _sfs.get<to_key>().begin(); iter != _sfs.get<to_key>().end(); ++iter )
    vsfs.push_back(_dd(iter));

  if( vsfs.empty() )
    return SeriesFactorSet();

  // return highest drawdown from all calculated
  return *min_element(vsfs.begin(), vsfs.end(), SeriesFactorSetFactorCmp());
}


SeriesFactorSet PositionFactors::_dd(series_factor_by_end_mark::const_iterator& start) const
{
  SeriesFactorSet sfs, dd_sfs;
  double acc = 1;
  double dd = 1;

  for( series_factor_by_end_mark::const_iterator iter = start; iter != _sfs.get<to_key>().end(); ++iter ) {
    sfs.insert(*iter);
    acc *= (*iter).factor();

    if( acc < dd ) {
      dd = acc;
      dd_sfs = sfs;
    }
  }

  return dd_sfs;
}


SeriesFactorSet PositionFactors::pk(void) const
{
  vector<SeriesFactorSet> vsfs;

  // store drawdown from each series factor
  for( series_factor_by_end_mark::iterator iter = _sfs.get<to_key>().begin(); iter != _sfs.get<to_key>().end(); ++iter )
    vsfs.push_back(_pk(iter));

  if( vsfs.empty() )
    return SeriesFactorSet();

  // return highest drawdown from all calculated
  return *max_element(vsfs.begin(), vsfs.end(), SeriesFactorSetFactorCmp());
}


SeriesFactorSet PositionFactors::_pk(series_factor_by_end_mark::const_iterator& start) const
{
  SeriesFactorSet sfs, pk_sfs;
  double acc = 1;
  double pk = 1;

  for( series_factor_by_end_mark::const_iterator iter = start; iter != _sfs.get<to_key>().end(); ++iter ) {
    sfs.insert(*iter);
    acc *= (*iter).factor();

    if( acc > pk ) {
      pk = acc;
      pk_sfs = sfs;
    }
  }

  return pk_sfs;
}
