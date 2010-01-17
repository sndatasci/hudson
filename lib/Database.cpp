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

// Hudson
#include "Database.hpp"
#include "EODDB.hpp"

using namespace boost::gregorian;
using namespace std;
using namespace Series;


Series::Database::Database(const std::string& filename, const date_period& dp, const std::set<std::string>& symbols):
  _filename(filename),
  _dp(dp),
  _symbols(symbols)
{
}


void Series::Database::load(void) throw(DatabaseException)
{
  if( _dp.is_null() )
    throw DatabaseException("Invalid period");

  for( set<string>::const_iterator iter = _symbols.begin(); iter != _symbols.end(); ++iter ) {

    string symbol = *iter;

    try {

      Series::EODDB::instance().load(_filename, symbol, _dp.begin(), _dp.last());

    } catch(const std::exception& ex) {
      cerr << "Cannot load " << symbol << " data from " << _filename << ": " << ex.what() << endl;
      throw DatabaseException("Invalid series");
    }

  } // for(;;)
}


void Series::Database::print(void)
{
  for( set<string>::const_iterator iter = _symbols.begin(); iter != _symbols.end(); ++iter ) {

    try {

      const Series::EODSeries& series = Series::EODDB::instance().get(*iter);

      cout << "--" << endl;
      cout << "Symbol: " << *iter << endl;
      cout << "Records: " << series.size() << endl;
      cout << "Total days: " << series.days() << endl;
      cout << "From: " << series.begin()->first << endl;
      cout << "To: " << series.rbegin()->first << endl;

    } catch( const std::exception& ex ) {
      cerr << "Cannot print data for database symbol " << *iter << ": " << ex.what() << endl;
      continue;
    }

  } // for(;;)
}
