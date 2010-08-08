// std
#include <iostream>
#include <fstream>

// boost
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/variance.hpp>

// sqlite3
#include <sqlite3.h>

#include "stats.hpp"

using namespace std;
using namespace boost::gregorian;

stats::stats(const args& a):
  m_a(a)
{
}


bool stats::calc()
{
  // Open database
  sqlite3* db;
  int ret;
  if( (ret = sqlite3_open(m_a.database.c_str(), &db)) != SQLITE_OK ) {
    cerr << "Cannot open database " << m_a.database << endl;
    return false;
  }

  // Open tickers file
  ifstream tickers_file(m_a.tickers.c_str());
  if( ! tickers_file.is_open() ) {
    cerr << "Cannot open " << m_a.tickers << endl;
    return false;
  }

  cout.precision(4);
  cout.setf(ios::fixed);

  string line;
  bool header = false;
  while( ! tickers_file.eof() ) {
    getline(tickers_file, line);

    size_t found_begin = line.find_first_not_of("\t\f\v ");
    size_t found_end = line.find_last_not_of("\t\f\v ");
    if( found_begin == string::npos || found_end == string::npos )
      continue; // skip empty line

    string symbol = line.substr(found_begin, found_end-found_begin+1);

    // Run query for this symbol
    std::stringstream ss;
    std::string sql;
    ss << "SELECT day_date, adjclose_price, volume FROM eod WHERE symbol='" << symbol << "' AND day_date BETWEEN '" << to_iso_extended_string(m_a.begin) << "' AND '" << to_iso_extended_string(m_a.end) << "'";
    sql = ss.str();

    char** results;
    int row, col;
    char* errmsg;
    if( sqlite3_get_table(db, sql.c_str(), &results, &row, &col, &errmsg) != SQLITE_OK ) {
      cerr << "Cannot run db query on symbol " << symbol << ": " << errmsg << endl;
      continue;
    }

    if( row == 0 ) {
      cerr << "No data found for " << symbol << endl;
      continue;
    }

    if( ! header ) {
      unsigned months = row / 21;
      cout << "Symbol," << months << "M Return,Stddev,Return/Stddev,21dVol" << endl;
      header = true;
    }

    boost::accumulators::accumulator_set<double, boost::accumulators::stats<boost::accumulators::tag::variance(boost::accumulators::lazy)> > acc1;
    boost::accumulators::accumulator_set<unsigned, boost::accumulators::stats<boost::accumulators::tag::mean> > acc2;

    for( int i = 1; i <= row; i++ ) {
      if( i > 1 ) {
        double current = atof(results[i*col+1]);
        double prev = atof(results[(i-1)*col+1]);
        double ret = (current - prev)/prev;
        acc1(ret);
      }
    }

    for( int i = row; i > 0 && i > row-21; i-- ) {
      unsigned vol = atoi(results[i*col+2]);
      acc2(vol);
    }

    const double first_price = atof(results[col+1]);
    const double last_price = atof(results[row*col+1]);
    const double tot_ret = (last_price - first_price)/first_price;
    const double stddev = std::sqrt(boost::accumulators::variance(acc1));
    const double tot_ret_stddev = tot_ret / stddev;
    cout << symbol << ',' << tot_ret << ',' << stddev << "," << tot_ret_stddev << "," << boost::accumulators::mean(acc2) << endl;

    sqlite3_free_table(results);
  }

  return true;
}


ostream& stats::print(ostream& os) const
{
  return os;
}
