/*
 * AATrader.hpp
 */

// Hudson
#include "AATrader.hpp"
#include "Print.hpp"

using namespace std;
using namespace boost::gregorian;


AATrader::AATrader(const DB& spx_db, const DB& tnx_db, const DB& djc_db, const DB& eafe_db, const DB& reit_db):
  _spx_db(spx_db),
  _tnx_db(tnx_db),
  _djc_db(djc_db),
  _eafe_db(eafe_db),
  _reit_db(reit_db)
{
}


void AATrader::run(void) throw(TraderException)
{
  TA ta;

  const DB spx_monthly_db = _spx_db.monthly();
  //const DB tnx_monthly_db = _tnx_db.monthly();
  //const DB djc_monthly_db = _djc_db.monthly();
  //const DB eafe_monthly_db = _eafe_db.monthly();
  //const DB reit_monthly_db = _reit_db.monthly();

  DB::const_iterator spx_iter(spx_monthly_db.begin());
  //DB::const_iterator tnx_iter(tnx_monthly_db.begin());
  //DB::const_iterator djc_iter(djc_monthly_db.begin());
  //DB::const_iterator eafe_iter(eafe_monthly_db.begin());
  //DB::const_iterator reit_iter(reit_monthly_db.begin());

  TA::MACDRes spxMACD = ta.MACD(spx_monthly_db.close(), 12, 26, 9);
  //TA::MACDRes tnxMACD = ta.MACD(tnx_monthly_db.close(), 12, 26, 9);
  //TA::MACDRes djcMACD = ta.MACD(djc_monthly_db.close(), 12, 26, 9);
  //TA::MACDRes eafeMACD = ta.MACD(eafe_monthly_db.close(), 12, 26, 9);
  //TA::MACDRes reitMACD = ta.MACD(reit_monthly_db.close(), 12, 26, 9);

  // Shift series iterator to the beginning of MACD signals in MACD results vector
  advance(spx_iter, spxMACD.begIdx);
  //advance(tnx_iter, tnxMACD.begIdx);
  //advance(djc_iter, djcMACD.begIdx);
  //advance(eafe_iter, eafeMACD.begIdx);
  //advance(reit_iter, reitMACD.begIdx);

  for( int i = 0; spx_iter != spx_monthly_db.end(); ++spx_iter, ++i ) {

    try {

      check_buy(spx_monthly_db, spx_iter, spxMACD, i);
      //check_buy(tnx_monthly_db, tnx_iter, tnxMACD, i);
      //check_buy(djc_monthly_db, djc_iter, djcMACD, i);

      check_sell(spx_monthly_db, spx_iter, spxMACD, i);
      //check_sell(tnx_monthly_db, tnx_iter, tnxMACD, i);
      //check_sell(djc_monthly_db, djc_iter, djcMACD, i);

    } catch( std::exception& e ) {

      cerr << e.what() << endl;
      continue;
    } // try/catch
  } // for( ; ; )
}


void AATrader::check_buy( const DB& db, DB::const_iterator& iter, const TA::MACDRes& macd, int i )
{
  // Buy on MACD > signal
  if( _miPositions.open(db.name()).empty() && macd.macd[i] > macd.macd_signal[i] ) {

    // Buy tomorrow's open
    DB::const_iterator iter_entry = db.after(iter->first);
    if( iter_entry == db.end() ) {
      cerr << "Can't open position after " << iter->first << endl;
      return;
    }

    buy(db.name(), iter_entry->first, iter_entry->second.open);
  }
}


void AATrader::check_sell( const DB& db, DB::const_iterator& iter, const TA::MACDRes& macd, int i )
{
  // Sell on MACD < signal
  if( ! _miPositions.open(db.name()).empty() && macd.macd[i] < macd.macd_signal[i] ) {

    DB::const_iterator iter_exit = db.after(iter->first);
    if( iter_exit == db.end() ) {
      cerr << "Can't close position after " << iter->first << endl;
      return;
    }

    // Close all open positions at tomorrow's close
    PositionSet ps = _miPositions.open(db.name());
    for( PositionSet::const_iterator pos_iter = ps.begin(); pos_iter != ps.end(); ++pos_iter ) {
      PositionPtr pPos = (*pos_iter);
      // Sell at tomorrow's open
      close(pPos->id(), iter_exit->first, iter_exit->second.open);
    } // end of all open positions
  }
}
