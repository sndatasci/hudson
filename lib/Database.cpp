/*
* Copyright (C) 2007,2008 Alberto Giannetti
*
* This file is part of Hudson.
*
* Hudson is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* Hudson is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with Hudson.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "StdAfx.h"

// Hudson
#include "Database.hpp"

using namespace boost::gregorian;
using namespace std;
using namespace Series;


Series::Database::Database(const date_period& dp, const SERIES_MAP& mSeries):
  _dp(dp),
  _mSeries(mSeries)
{
}


void Series::Database::load(void) throw(DatabaseException)
{
  if( _dp.is_null() )
    throw DatabaseException("Invalid period");

  for( SERIES_MAP::const_iterator iter = _mSeries.begin(); iter != _mSeries.end(); ++iter ) {
    string symbol = iter->first;

    try {

      Series::EODDB::instance().load(symbol, iter->second.filename, iter->second.driver, _dp.begin(), _dp.last());

    } catch(const std::exception& ex) {
      cerr << "Cannot load series " << symbol << " from file " << iter->second.filename << ": " << ex.what() << endl;
      continue;
    }

  } // for(;;)
}


void Series::Database::print(void)
{
  for( SERIES_MAP::const_iterator iter = _mSeries.begin(); iter != _mSeries.end(); ++iter ) {

    try {

      const Series::EODSeries& db = Series::EODDB::instance().get(iter->first);

      cout << "--" << endl;
      cout << "Symbol: " << iter->first << endl;
      cout << "Records: " << db.size() << endl;
      cout << "Total days: " << db.days() << endl;
      cout << "From: " << db.begin()->first << endl;
      cout << "To: " << db.rbegin()->first << endl;

    } catch( const std::exception& ex ) {
      cerr << "Cannot print data for database symbol " << iter->first << ": " << ex.what() << endl;
      continue;
    }

  } // for(;;)
}
