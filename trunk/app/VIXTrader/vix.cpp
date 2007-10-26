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

// Boost
#include <boost/program_options.hpp>

// Hudson
#include "YahooDriver.hpp"
#include "EODSeries.hpp"
#include "ReturnFactors.hpp"
#include "PositionFactors.hpp"
#include "VIXTrader.hpp"
#include "BnHTrader.hpp"
#include "Report.hpp"
#include "PositionsReport.hpp"

using namespace std;
using namespace boost::gregorian;
using namespace Series;

namespace po = boost::program_options;


int main(int argc, char* argv[])
{
  string begin_date, end_date;
  string dbfile, vix_dbfile;

  try {

    /*
    * Extract simulation options
    */
    po::options_description desc("Allowed options");
    desc.add_options()
      ("help", "produce help message")
      ("series_file",     po::value<string>(&dbfile),                       "series database")
      ("vix_series_file", po::value<string>(&vix_dbfile),                   "VIX series database")
      ("begin_date",      po::value<string>(&begin_date),                   "start of trading period (YYYY-MM-DD)")
      ("end_date",        po::value<string>(&end_date),                     "end of trading period (YYYY-MM-DD)")
      ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if( vm.count("help") ) {
      cout << desc << endl;
      exit(0);
    }

    if( vm["series_file"].empty() ||
        vm["vix_series_file"].empty() ||
	      vm["begin_date"].empty() ||
	      vm["end_date"].empty() ) {
      cout << desc << endl;
      exit(1);
    }

    cout << "Series file: " << dbfile << endl;
    cout << "VIX series file: " << vix_dbfile << endl;

    /*
    * Load series data
    */
    YahooDriver yd;
    EODSeries db(dbfile);
    EODSeries vixdb(vix_dbfile);

    date load_begin(from_simple_string(begin_date));
    if( load_begin.is_not_a_date() ) {
      cerr << "Invalid begin date " << begin_date << endl;
      exit(EXIT_FAILURE);
    }

    date load_end(from_simple_string(end_date));
    if( load_end.is_not_a_date() ) {
      cerr << "Invalid end date " << end_date << endl;
      exit(EXIT_FAILURE);
    }

    cout << "Loading " << dbfile << " from " << to_simple_string(load_begin) << " to " << to_simple_string(load_end) << "..." << endl;
    if( db.load(yd, dbfile, load_begin, load_end) <= 0 ) {
      cerr << "No records found" << endl;
      exit(EXIT_FAILURE);
    }

    cout << "Loading " << vix_dbfile << " from " << to_simple_string(load_begin) << " to " << to_simple_string(load_end) << "..." << endl;
    if( vixdb.load(yd, vix_dbfile, load_begin, load_end) <= 0 ) {
      cerr << "No records found" << endl;
      exit(EXIT_FAILURE);
    }

    cout << "Records: " << db.size() << endl;
    cout << "Period: " << db.period() << endl;
    cout << "Total days: " << db.duration().days() << endl;

    /*
    * Initialize and run strategy
    */
    VIXTrader trader(db, vixdb);
    trader.run();

    /*
     * Print open/closed positions
     */
    Price last(db.rbegin()->second.adjclose);
    Report::header("Closed trades");
    trader.positions().closed().print(last);

    Report::header("Open trades");
    trader.positions().open().print(last);

    /*
     * Print simulation reports
     */
    Report::header("Trade results");
    ReturnFactors rf(trader.positions(), last);
    Report rp(rf);
    rp.print();

    /*
     * Positions excursion
     */
    Report::header("Positions excursion");
    PositionFactorsSet pf(trader.positions().closed(), db);
    PositionsReport pr(pf);
    pr.print();

  } catch( std::exception& ex ) {

    cerr << ex.what() << endl;
    exit(EXIT_FAILURE);
  }

  return 0;
}
