/*
 * EOMReturnFactors.cpp
 */

#include <StdAfx.h>

// CSTD
#include <cmath>

// Hudson
#include "EOMReturnFactors.hpp"

using namespace std;
using namespace Series;


EOMReturnFactors::EOMReturnFactors( const PositionSet& sPositions, const Series::EODSeries& db, double rf_rate ):
  ReturnFactors(sPositions, db),
  _monthly_db(_db.monthly()),
  _rf_rate(rf_rate),
  _mmean(0),
  _mstddev(0),
  _msharpe(0)
{
  _calculateM2M();
}


double EOMReturnFactors::gsd( void ) const
{
  if( _sPositions.empty() )
    return 0;

  double lfavg = accumulate<doubleVector::const_iterator, double>( _vLogMFactors.begin(), _vLogMFactors.end(), 0 ) / _vLogMFactors.size();
  double lfstddev = ::sqrt( accumulate<doubleVector::const_iterator, double>( _vLogMFactors.begin(), _vLogMFactors.end(), 0, variance_bf(lfavg) ) / (_vLogMFactors.size() - 1) );
  return ::pow( (double)10, lfstddev * ::sqrt((double)_yperiods) ) - 1;
}


double EOMReturnFactors::sharpe( void ) const
{
  return _msharpe;
}


void EOMReturnFactors::_calculateM2M(void)
{
  // For each month in db
  EODSeries::const_iterator prev_em_mark(_db.begin());
  for( EODSeries::const_iterator em_mark(_monthly_db.begin()); em_mark != _monthly_db.end(); ++em_mark ) {

    //cout << "Calculating M2M factor from " << prev_em_mark->first << " to " << em_mark->first << endl;

    // For each position in our local set
    double f_acc = 1;
    for( PositionSet::const_iterator piter(_sPositions.begin()); piter != _sPositions.end(); ++piter ) {

      // Skip positions closed before bm_mark or opened after em_mark
      if( ((*piter)->closed() && (*piter)->last_exec().dt() <= prev_em_mark->first) ) {
        //cout << "Skipping position " << (*piter)->id() << ". Position closed before beginning of this month" << endl;
        continue;
      }

      if( (*piter)->first_exec().dt() > em_mark->first ) {
        //cout << "Skipping position " << (*piter)->id() << ". Position opened after end of this month" << endl;
        continue; // Doesn't qualify for this month factor
      }

      // Calculate monthly factor for this position
      double f = _monthlyFactor(prev_em_mark, em_mark, *piter);
      //cout << "Monthly factor for position " << (*piter)->id() << " (" << em_mark->first.month() << "): " << f << endl;

      f_acc *= f;
    } // End of positions for this month

    // Add monthly factor
    //cout << em_mark->first << ", factor " << f_acc << endl;
    _vMFactors.push_back(f_acc);
    _vLogMFactors.push_back(::log10(f_acc));
    _mDateMFactors.insert(DATEMFACTOR::value_type(em_mark->first, f_acc));

    prev_em_mark = em_mark;
  }

  _mmean = accumulate<doubleVector::const_iterator, double>( _vMFactors.begin(), _vMFactors.end(), 0 ) / _vMFactors.size();
  _mstddev = ::sqrt( accumulate<doubleVector::const_iterator, double>( _vMFactors.begin(), _vMFactors.end(), 0, variance_bf(_mmean) ) / (_vMFactors.size() - 1) );
  _msharpe = (((_mmean-1)*12) - (_rf_rate/100)) / (_mstddev*::sqrt((double)12));
}


double EOMReturnFactors::_monthlyFactor(EODSeries::const_iterator prev_em_mark, EODSeries::const_iterator em_mark, const PositionPtr pos)
{
  // XXX: Factors should be calculated for each execution in the position, not using entry/exit averages

  //cout << "Calculating position " << pos->id() << " from " << prev_em_mark->first << " to " << em_mark->first << endl;

  double begin_price = 0;
  if( pos->first_exec().dt() <= prev_em_mark->first ) {
    begin_price = prev_em_mark->second.adjclose;
    //cout << "Position opened before or at previous EOM mark price, using " << prev_em_mark->first << " adjclose" << endl;
  } else if( pos->first_exec().dt() > prev_em_mark->first && pos->first_exec().dt() <= em_mark->first ) {
    begin_price = pos->avgEntryPrice();
    //cout << "Position opened after previous EOM mark price, using position avg entry price" << endl;
  } else {
    cerr << "Error: position opened after EOM end mark" << endl;
    return 1;
  }

  double end_price = 0;
  if( pos->open() || pos->last_exec().dt() > em_mark->first ) {
    end_price = em_mark->second.adjclose;
    //cout << "Position still open or closed after EOM mark price, using " << em_mark->first << " adjclose" << endl;
  } else if( pos->last_exec().dt() <= em_mark->first ) {
    end_price = pos->avgExitPrice();
    //cout << "Position closed before EOM mark price, using position avg exit price" << endl;
  } else {
    cerr << "Error: position " << endl;
  }

  return end_price / begin_price;
}
