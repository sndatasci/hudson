/*
* Copyright (C) 2007, Alberto Giannetti
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

// STD
#include <sstream>
#include <cstdlib>

// Hudson
#include "EODDB.hpp"
#include "EODSeries.hpp"

using namespace boost::gregorian;


std::auto_ptr<Series::EODDB> Series::EODDB::_pInstance;


Series::EODDB& Series::EODDB::instance(void)
{
  if( _pInstance.get() == 0 )
    _pInstance.reset(new EODDB);

  return *_pInstance;
}


void Series::EODDB::load(const std::string& filename, const std::string& symbol,
			 const boost::gregorian::date& begin, const boost::gregorian::date& end) throw(EODDBException)
{
  if( _mSeries.find(symbol) != _mSeries.end() )
    throw EODDBException("Series already loaded");

  // Open database
  sqlite3* db;
  int ret;
  if( (ret = sqlite3_open(filename.c_str(), &db) != SQLITE_OK ) )
    throw EODDBException(sqlite3_errmsg(db));

  // Prepare query
  std::stringstream ss;
  ss << "SELECT day_date, open_price, high_price, low_price, close_price, adjclose_price, volume FROM eod WHERE symbol='" << symbol << "' AND day_date BETWEEN '" << to_iso_extended_string(begin) << "' AND '" << to_iso_extended_string(end) << "'";
  std::string sql_query = ss.str();

  char** results;
  int row, col;
  char* errmsg;
  if( sqlite3_get_table(db, sql_query.c_str(), &results, &row, &col, &errmsg) != SQLITE_OK ) {
    sqlite3_close(db);
    throw EODDBException(sqlite3_errmsg(db));
  }

  // Load new series
  EODSeries* pSeries = new EODSeries(symbol);
  DayPrice dp;
  for( int i = 1; i < row; i++ ) { // start at index 1 to skip title row
    dp.key = from_string(results[i*col+0]);
    dp.open = atof(results[i*col+1]);
    dp.high = atof(results[i*col+2]);
    dp.low = atof(results[i*col+3]);
    dp.close = atof(results[i*col+4]);
    dp.adjclose = atof(results[i*col+5]);
    dp.volume = atoi(results[i*col+6]);

    pSeries->insert(EODSeries::value_type(dp.key, dp));
  }

  _mSeries.insert(SERIES_MAP::value_type(symbol, pSeries));

  sqlite3_free_table(results);

  sqlite3_close(db);		// TODO: Collapse EODDB and Database classes
}


const Series::EODSeries& Series::EODDB::get(const std::string& symbol) const throw(EODDBException)
{
  SERIES_MAP::const_iterator iter;
  if( (iter = _mSeries.find(symbol)) == _mSeries.end() )
    throw EODDBException("Unknown series");

  return *(iter->second);
}
