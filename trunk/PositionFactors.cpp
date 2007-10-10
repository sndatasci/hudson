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


PositionFactors::PositionFactors( const Position& pos, const EODSeries& db ):
  _pos(pos),
  _db(db)
{
  // Browse all price series from the position opening and build daily factors
  date prev_date = _pos.first_exec().dt();
  double prev_price = _pos.avgEntryPrice();

  double curr_price = 0;
  date curr_date;

  // Initialize all factors until the end of the series database
  //cout << "Initializing daily factors for position " << _pos.id() << " (" << _pos.first_exec().dt() << "/" << _pos.last_exec().dt() << ")" << endl;
  for( EODSeries::const_iterator iter = _db.after(_pos.first_exec().dt()); iter != _db.end(); ++iter ) {

    // If position is closed we only initialize factors up to the last execution
    if( _pos.closed() && iter->first > _pos.last_exec().dt() )
      break;

    // Get current price mark
    curr_price = iter->second.close;
    curr_date  = iter->first;

    // Calculate position factor until this point
    double f = _pos.factor(prev_price, curr_price);

    // Initialize SeriesFactorSet indexed by time to calculate bfe() and wae(). multi_index secondary index is too slow.
    _sf_fromtm.insert(SeriesFactor(ptime(prev_date), ptime(curr_date), f));
    _sf_totm.insert(SeriesFactor(ptime(prev_date), ptime(curr_date), f));

    prev_price = curr_price;
    prev_date = curr_date;
  }

  //cout << "Time-based items (from_tm): " << (unsigned int)_sf_fromtm.size() << endl;
  //cout << "Time-based items (to_tm): " << (unsigned int)_sf_totm.size() << endl;
}


SeriesFactorSet PositionFactors::max_cons_pos(void) const
{
  SeriesFactorSet maxSFS;

  SF_TOTM::const_iterator iter = _sf_totm.begin();
  while( iter != _sf_totm.end() ) {
    // Skip negative factors
    if( (*iter).factor() <= 1 ) {
      ++iter;
      continue;
    }

    // Found a positive factor, keep going until it last
    SeriesFactorSet sfset;
    while( iter != _sf_totm.end() && (*iter).factor() > 1 ) {
      sfset.insert(*iter);
      ++iter;
    }

    // Store new record high if better than previous one
    if( sfset.size() > maxSFS.size() )
      maxSFS = sfset;
  }

  return maxSFS;
}


SeriesFactorSet PositionFactors::max_cons_neg(void) const
{
  SeriesFactorSet maxSFS;

  SF_TOTM::const_iterator iter = _sf_totm.begin();
  while ( iter != _sf_totm.end() ) {
    if( (*iter).factor() >= 1 ) {
      ++iter;
      continue;
    }

    // Negative factor, look for negative sequence
    SeriesFactorSet sfset;
    while( iter != _sf_totm.end() && (*iter).factor() < 1 ) {
      sfset.insert(*iter);
      ++iter;
    }

    if( sfset.size() > maxSFS.size() )
      maxSFS = sfset;
  }

  return maxSFS;
}


SeriesFactorSet PositionFactors::wae( void ) const throw(PositionFactorsException)
{
  // Look for worst position excursion period starting from each entry in daily series
  //cout << "Calculating worst excursion period for position " << _pos.id() << endl;

  // For each SeriesFactor bar
  PeriodFactor worst_pf;
  for( SF_TOTM::const_iterator iter = _sf_totm.begin(); iter != _sf_totm.end(); ++iter ) {

    // Calculate worst adverse excursion from this point
    //cout << "Calculating position adverse excursion from " << (*iter).from_tm() << endl;
    PeriodFactor pf = _wae(iter); // return drawdown from this bar

    // Store worst adverse excursion
    if( pf.factor < worst_pf.factor )
      worst_pf = pf;
  }

  if( !worst_pf.isValid() ) {
    stringstream ss;
    ss << "Can't get position " << _pos.id() << " worst adverse excursion period";
    throw PositionFactorsException(ss.str());
  }

  //cout << "Found worst excursion from " << worst_pf.from_tm << " to " << worst_pf.to_tm << endl;

  // Locate beginning of stored worst adverse excursion in SeriesFactor set
  SeriesFactor sf(worst_pf.from_tm, worst_pf.to_tm, worst_pf.factor);
  SF_FROMTM::const_iterator src_begin = _sf_fromtm.find(sf);
  if( src_begin == _sf_fromtm.end() ) {
    stringstream ss;
    ss << "Can't get position " << _pos.id() << " beginning of worst adverse excursion period" << endl;
    throw PositionFactorsException(ss.str());
  }

  // Initialize SeriesFactorSet with SeriesFactor(s) included in worst_pf range
  SeriesFactorSet sfs;
  //cout << "Adding SeriesFactor from " << (*src_begin).from_tm() << " to " << worst_pf.to_tm << endl;
  for( SF_FROMTM::const_iterator iter(src_begin); iter != _sf_fromtm.end() && (*iter).to_tm() <= worst_pf.to_tm; ++iter )
    sfs.insert(*iter);

  return sfs; // Return worst drawdown for this position
}


PositionFactors::PeriodFactor PositionFactors::_wae(SF_TOTM::const_iterator& start) const
{
  // Return value defaults
  PeriodFactor pf;
  pf.from_tm = (*start).from_tm(); // Begin of excursion no matter what. Index must be set to from_tm() for later retrieval by from_tm() key

  // Calculate worst excursion from 'start'. We can't use multi_index secondary index for performance reasons.
  SeriesFactorSet sfs;
  for( SF_TOTM::const_iterator iter = start; iter != _sf_totm.end(); ++iter ) {

    sfs.insert(*iter); // current position factors series

    // Check lower excursion
    if( sfs.factor() < pf.factor ) {
      // Store lower excursion
      pf.factor = sfs.factor();
      pf.to_tm = (*iter).to_tm();
    }
  }

  // Return worst drawdown factor including begin/end time marks
  return pf;
}


SeriesFactorSet PositionFactors::bfe(void) const throw(SeriesFactorSetException)
{
  // Look for worst position excursion period starting from each entry in daily series
  //cout << "Calculating best excursion period for position " << _pos.id() << endl;

  // For each SeriesFactor bar
  PeriodFactor best_pf;
  for( SF_TOTM::const_iterator iter = _sf_totm.begin(); iter != _sf_totm.end(); ++iter ) {

    // Calculate best favorable excursion from this point
    //cout << "Calculating position favorable excursion from " << (*iter).from_tm() << endl;
    PeriodFactor pf = _bfe(iter); // return best favorable excursion from this bar

    // Store best favorable excursion till now
    if( pf.factor > best_pf.factor )
      best_pf = pf; 
  }

  if( !best_pf.isValid() ) {
    stringstream ss;
    ss << "Can't get position " << _pos.id() << " best favorable excursion period";
    throw PositionFactorsException(ss.str());
  }

  // Locate beginning of stored best excursion
  SeriesFactor sf(best_pf.from_tm, best_pf.to_tm, best_pf.factor);
  SF_FROMTM::const_iterator src_begin = _sf_fromtm.find(sf);
  if( src_begin == _sf_fromtm.end() ) {
    stringstream ss;
    ss << "Can't get position " << _pos.id() << " beginning of best favorable excursion period" << endl;
    throw PositionFactorsException(ss.str());
  }

  // Initialize SeriesFactorSet with SeriesFactor(s) included in best_pf range
  SeriesFactorSet sfs;
  for( SF_FROMTM::const_iterator iter(src_begin); (*iter).to_tm() <= best_pf.to_tm && iter != _sf_fromtm.end(); ++iter )
    sfs.insert(*iter);

  return sfs; // Return worst drawdown for this position
}


PositionFactors::PeriodFactor PositionFactors::_bfe(SF_TOTM::const_iterator& start) const
{
  // Return value defaults
  PeriodFactor pf;
  pf.from_tm = (*start).from_tm(); // Begin of excursion no matter what

  // Calculate best excursion from 'start'. We can't use multi_index secondary index for performance reasons.
  SeriesFactorSet sfs;
  for( SF_TOTM::const_iterator iter = start; iter != _sf_totm.end(); ++iter ) {
    sfs.insert(*iter);

    // Check higher excursion
    if( sfs.factor() > pf.factor ) {
      // Store higher excursion
      pf.factor = sfs.factor();
      pf.to_tm = (*iter).to_tm();
    }
  }

  return pf;
}


PositionFactors::PeriodFactor::PeriodFactor( void ):
  factor(1)
{
}


bool PositionFactors::PeriodFactor::isValid( void ) const
{
  return from_tm != not_a_date_time && to_tm != not_a_date_time;
}
