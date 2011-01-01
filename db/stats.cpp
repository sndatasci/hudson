// std
#include <iostream>
#include <fstream>
#include <map>

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

  // For each ticker line
  string line;
  bool header = false;
  while( ! tickers_file.eof() ) {
    getline(tickers_file, line);

    size_t found_begin = line.find_first_not_of("\t\f\v ");
    size_t found_end = line.find_last_not_of("\t\f\v ");
    if( found_begin == string::npos || found_end == string::npos )
      continue; // skip empty line

    // Extract symbol
    string symbol = line.substr(found_begin, found_end-found_begin+1);

    // Run risk periods query
    std::stringstream ss;
    std::string sql;
    ss << "SELECT day_date, adjclose_price, volume FROM eod WHERE symbol='" << symbol << "' AND day_date BETWEEN '" << to_iso_extended_string(m_a.risk_begin) << "' AND '" << to_iso_extended_string(m_a.end) << "'";
    sql = ss.str();

    char** risk_periods;
    int risk_rows, risk_cols;
    char* errmsg;
    if( sqlite3_get_table(db, sql.c_str(), &risk_periods, &risk_rows, &risk_cols, &errmsg) != SQLITE_OK ) {
      cerr << "Cannot run db query on symbol " << symbol << ": " << errmsg << endl;
      continue;
    }

    if( risk_rows == 0 ) {
      cerr << "No risk data found for " << symbol << endl;
      continue;
    }

    // Run returns query
    char** return_periods;
    int return_rows, return_cols;
    std::stringstream ss2;
    ss2 << "SELECT day_date, adjclose_price FROM eod WHERE symbol='" << symbol << "' AND day_date BETWEEN '" << to_iso_extended_string(m_a.begin) << "' AND '" << to_iso_extended_string(m_a.end) << "'";
    sql = ss2.str();
    if( sqlite3_get_table(db, sql.c_str(), &return_periods, &return_rows, &return_cols, &errmsg) != SQLITE_OK ) {
      cerr << "Cannot run db query on symbol " << symbol << ": " << errmsg << endl;
      continue;
    }

    if( return_rows == 0 ) {
      cerr << "No return data found for " << symbol << endl;
      continue;
    }

    boost::accumulators::accumulator_set<double, boost::accumulators::stats<boost::accumulators::tag::variance(boost::accumulators::lazy)> > acc1;
    boost::accumulators::accumulator_set<unsigned, boost::accumulators::stats<boost::accumulators::tag::mean> > acc2;

    // Calculate risk
    for( int i = 1; i <= risk_rows; i++ ) {
      if( i > 1 ) {
        const double current = atof(risk_periods[i*risk_cols+1]);
        const double prev = atof(risk_periods[(i-1)*risk_cols+1]);
        const double ret = (current - prev)/prev;
        acc1(ret);
      }
    }
    
    // Calculate volume
    for( int i = risk_rows; i > 0 && i > risk_rows-21; i-- ) {
      unsigned vol = atoi(risk_periods[i*risk_cols+2]);
      acc2(vol);
    }

    double first_price = atof(return_periods[return_cols+1]);
    string first_date = return_periods[return_cols];
    double last_price = atof(return_periods[return_rows*return_cols+1]);
    string last_date = return_periods[return_rows*return_cols];
    double tot_ret = (last_price - first_price)/first_price;
    double stddev = std::sqrt(boost::accumulators::variance(acc1));
    double tot_ret_stddev = tot_ret / stddev;
    unsigned vol = boost::accumulators::mean(acc2);

    if( ! header ) {
      unsigned return_months = return_rows / 21;
      unsigned risk_months = risk_rows / 21;
      cout << "Symbol," << return_months << "M Return," << risk_months << "M Stddev,Return/Stddev,21dVol,"
           << first_date << ',' << last_date << endl;
      header = true;
    }

    cout << symbol << ',' << tot_ret << ',' << stddev << "," << tot_ret_stddev << "," << vol << ',' << first_price << ',' << last_price << endl;

    sqlite3_free_table(risk_periods);
    sqlite3_free_table(return_periods);
  } // for each ticker

  return true;
}


ostream& stats::print(ostream& os) const
{
  return os;
}
