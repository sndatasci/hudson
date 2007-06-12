/*
 * BnHTrader.cpp
 */

#include "StdAfx.h"

// Boost
#include <boost/date_time/gregorian/gregorian.hpp>

// Hudson
#include "BnHTrader.hpp"

using namespace std;
using namespace boost::gregorian;


BnHTrader::BnHTrader(const DB& db):
  _db(db)
{
}


void BnHTrader::run(void)
{
  Position::ID id = buy("BnH entry", _db.begin()->first, _db.begin()->second.adjclose);
  close(id, _db.rbegin()->first, _db.rbegin()->second.adjclose);
}
