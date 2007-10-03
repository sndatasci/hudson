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

  DB::const_iterator spx_iter(_spx_db.begin());
  DB::const_iterator tnx_iter(_tnx_db.begin());
  DB::const_iterator djc_iter(_djc_db.begin());
  DB::const_iterator eafe_iter(_eafe_db.begin());
  DB::const_iterator reit_iter(_reit_db.begin());

  vector<double> series = _spx_db.close();
  cout << "Total of " << series.size() << " bars in SPX series" << endl;

  TA::MACDRes spxMACD = ta.MACD(_spx_db.close(), 12, 26, 9);
  TA::MACDRes tnxMACD = ta.MACD(_tnx_db.close(), 12, 26, 9);
  TA::MACDRes djcMACD = ta.MACD(_djc_db.close(), 12, 26, 9);
  TA::MACDRes eafeMACD = ta.MACD(_eafe_db.close(), 12, 26, 9);
  //TA::MACDRes reitMACD = ta.MACD(_reit_db.close(), 12, 26, 9);

  // Shift series iterator to the beginning of MACD signals in MACD results vector
  advance(spx_iter, spxMACD.begIdx);
  for( int i = 0; spx_iter != _spx_db.end(); ++spx_iter, ++i ) {

    cout << "On " << spx_iter->first
      << " MACD " << spxMACD.macd[i]
        << ", MACD Hist " << spxMACD.macd_hist[i]
          << ", MACD signal " << spxMACD.macd_signal[i] << endl;

    try {

      spx_buy(spx_iter, spxMACD, i);
      spx_sell(spx_iter, spxMACD, i);

    } catch( std::exception& e ) {

      cerr << e.what() << endl;
      continue;

    } // try/catch
  } // for( ; ; )
}


void AATrader::spx_buy( DB::const_iterator& iter, const TA::MACDRes& macd, int i )
{
  // Check buy signal
  if( _miPositions.open().empty() && macd.macd_signal[i] > macd.macd[i] ) {
    // Buy tomorrow's close
    DB::const_iterator iter_entry = _spx_db.after(iter->first);
    if( iter_entry == _spx_db.end() ) {
      cerr << "Can't open position after " << iter->first << endl;
      return;
    }

    buy(_spx_db.name(), iter_entry->first, iter_entry->second.open);
  }
}


void AATrader::spx_sell( DB::const_iterator& iter, const TA::MACDRes& macd, int i )
{
  // Check sell signal
  if( ! _miPositions.open().empty() && macd.macd_signal[i] < macd.macd[i] ) {
    // Get next bar
    DB::const_iterator iter_exit = _spx_db.after(iter->first);
    if( iter_exit == _spx_db.end() ) {
      cerr << "Can't close position after " << iter->first << endl;
      return;
    }

    // Close all open positions at tomorrow's close
    PositionSet ps = _miPositions.open();
    for( PositionSet::const_iterator pos_iter = ps.begin(); pos_iter != ps.end(); ++pos_iter ) {
      PositionPtr pPos = (*pos_iter);
      close(pPos->id(), iter_exit->first, iter_exit->second.open);
    } // end of all open positions
  }
}
