/*
 * VIXTrader.hpp
 */

// Hudson
#include "VIXTrader.hpp"
#include "TA.hpp"
#include "Print.hpp"

using namespace std;
using namespace boost::gregorian;


VIXTrader::VIXTrader(const DB& db, const DB& vixdb):
  _db(db),
  _vixdb(vixdb),
  _invested_days(0)
{
}


void VIXTrader::run(void) throw(TraderException)
{
  TA ta;
  _invested_days = days(0);

  for( DB::const_iterator iter(_vixdb.begin()); iter != _vixdb.end(); ++iter ) {

	  try {

      // 1. Calculate current invested days
      if( ! _miPositions.open().empty() )
        _invested_days = _invested_days + days(1);

      // 2. Calculate VIX BBANDS
      TA::BBRes resBBANDS3 = ta.BBANDS(_db.close(iter, 100), 100, 3, 3);
      TA::BBRes resBBANDS1 = ta.BBANDS(_db.close(iter, 100), 100, 1, 1);

      // 3. Check buy signal
      if( _miPositions.open().empty() && iter->second.close > resBBANDS3.upper_band[0] ) {
        // Buy at the open the next day
        DB::const_iterator iter_entry = _db.after(iter->first);
        if( iter_entry == _db.end() ) {
          cerr << "Can't open position after " << iter->first << endl;
          continue;
        }

	      buy(_db.name(), iter_entry->first, iter_entry->second.open);
      } 

      // 4. Check sell signal
      if( ! _miPositions.open().empty() && iter->second.close < resBBANDS1.upper_band[0] ) {
        // Get next bar
        DB::const_iterator iter_exit = _db.after(iter->first);
        if( iter_exit == _db.end() ) {
          cerr << "Can't close position after " << iter->first << endl;
          continue;
        }

        // Close all open positions at tomorrow's open
        PositionSet ps = _miPositions.open();
        for( PositionSet::const_iterator pos_iter = ps.begin(); pos_iter != ps.end(); ++pos_iter ) {
          PositionPtr pPos = (*pos_iter);
	        close(pPos->id(), iter_exit->first, iter_exit->second.open);
        } // end of all open positions
      }
  	
	  } catch( std::exception& e ) {

	    cerr << e.what() << endl;
	    continue;
	  }

  }	// for each bar

}
