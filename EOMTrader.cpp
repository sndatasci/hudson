/*
 * EOMTrader.hpp
 */


#include "EOMTrader.hpp"

using namespace std;
using namespace boost::gregorian;


EOMTrader::EOMTrader(const DB& db):
  _db(db),
  _invested_days(0)
{
}


void EOMTrader::run(unsigned entry_days, unsigned exit_days) throw(TraderException)
{
  _invested_days = days(0);
  date my_first_entry;
  date my_last_exit;

  for( month_iterator miter(_db.begin()->first); miter < _db.rbegin()->first; ++miter ) {

	  if( (*miter).month() == _db.rbegin()->first.month() && (*miter).year() == _db.rbegin()->first.year() )
	    // We're on the last month of database record, we don't have enough records to find next month exit date
	    continue;

	  DB::const_iterator last_tradeday_iter = _db.last_in_month((*miter).year(), (*miter).month());
	  if( last_tradeday_iter == _db.end() ) {
	    cerr << "Can't find last trade day at date " << (*miter) << endl;
	    continue;
	  }

	  DB::const_iterator entry_iter = _db.before(last_tradeday_iter->first, entry_days - 1);
	  if( entry_iter == _db.end() ) {
	    cerr << "Can't find actual entry date for EOM " << last_tradeday_iter->first << endl;
	    continue;
	  }

	  DB::const_iterator exit_iter = _db.after(last_tradeday_iter->first, exit_days);
	  if( exit_iter == _db.end() ) {
	    cerr << "Can't find actual exit date for EOM " << last_tradeday_iter->first << endl;
	    continue;
	  }

	  try {

	    Position::ID id = buy("EOM", entry_iter->first, entry_iter->second.close);
	    close(id, exit_iter->first, exit_iter->second.close);

	  } catch( TraderException& te ) {
	    cerr << "Trader error: " << te.what() << endl;
	    continue;
  	
	  } catch( std::exception& e ) {
	    cerr << "Standard exception: " << e.what() << endl;
	    continue;
	  }

	  _invested_days = _invested_days + date_period(entry_iter->first, exit_iter->first).length();
  	
	  if( my_first_entry.is_not_a_date() ) my_first_entry = entry_iter->first;
	    my_last_exit = exit_iter->first;

  }	// for each month in price database

  _first_entry = my_first_entry;
  _last_exit = my_last_exit;
}
