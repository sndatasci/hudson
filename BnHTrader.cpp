/*
 * BnHTrader.cpp
 */

// Boost
#include <boost/date_time/gregorian/gregorian.hpp>

#include "BnHTrader.hpp"

using namespace std;
using namespace boost::gregorian;


BnHTrader::BnHTrader(const DB& db):
  _db(db)
{
}


void BnHTrader::run(void)
{
  _vf.clear();

  for( month_iterator miter(_db.begin()->first); miter < _db.rbegin()->first; ++miter ) {

	if( (*miter).month() == _db.rbegin()->first.month() && (*miter).year() == _db.rbegin()->first.year() )
	  // Last month in price database, not enough records to find the exit date at the beginning of next month
	  continue;

	DB::const_iterator bom_tradeday_iter = _db.first_in_month((*miter).year(), (*miter).month());
	date next_month = ((*miter).end_of_month() + date_duration(1));
	DB::const_iterator next_bom_tradeday_iter = _db.first_in_month(next_month.year(), next_month.month());

	if( bom_tradeday_iter == _db.end() ) {
	  cerr << "Can't find entry trading day " <<  *miter << endl;
	  continue;
	}

	if( bom_tradeday_iter->second.adjclose == 0 ) {
	  cerr << "Invalid price on " << bom_tradeday_iter->first << endl;
	  continue;
	}

	if( next_bom_tradeday_iter == _db.end() ) {
	  cerr << "Can't find exit trading day " << next_month << endl;
	  continue;
	}

	if( next_bom_tradeday_iter->second.adjclose == 0 ) {
	  cerr << "Invalid price on " << next_bom_tradeday_iter->first << endl;
	  continue;
	}

	try {

	  Position::ID id = buy("BnH EOM", bom_tradeday_iter->first, bom_tradeday_iter->second.adjclose);
	  close(id, next_bom_tradeday_iter->first, next_bom_tradeday_iter->second.adjclose);

	} catch( TraderException& te ) {
	  cerr << "BnH trader error: " << te.what() << endl;
	  continue;

	} catch( std::exception& e ) {
	  cerr << "BnH trader std exception: " << e.what() << endl;
	  continue;
	}

	double factor = next_bom_tradeday_iter->second.adjclose / bom_tradeday_iter->second.adjclose;
	_vf.push_back(factor);
  }

  // There might be a fraction of month for last month in price db
  date bom = date(_db.rbegin()->first.year(), _db.rbegin()->first.month(), 1);
  if( _db.rbegin()->first > bom ) {
	// Get first record for this last month
	DB::const_iterator bom_tradeday_iter = _db.first_in_month(bom.year(), bom.month());

	if( bom_tradeday_iter == _db.end() ) {
	  cerr << "Can't find BOM trading day " << _db.rbegin()->first << endl;
	  return;
	}

	if( bom_tradeday_iter->second.adjclose == 0 ) {
	  cerr << "Invalid price at date " << bom_tradeday_iter->first << endl;
	  return;
	}

	try {

	  Position::ID id = buy("BnH EOM", bom_tradeday_iter->first, bom_tradeday_iter->second.adjclose);
	  close(id, _db.rbegin()->first, _db.rbegin()->second.adjclose);

	} catch( TraderException& te ) {
	  cerr << "BnH trader error: " << te.what() << endl;
	  return;

	} catch( std::exception& e ) {
	  cerr << "BnH trader std exception: " << e.what() << endl;
	  return;
	}

	double factor = _db.rbegin()->second.adjclose / bom_tradeday_iter->second.adjclose;
	_vf.push_back(factor);
  }
}
