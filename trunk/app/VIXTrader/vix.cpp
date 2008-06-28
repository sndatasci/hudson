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

// STD
#include <iostream>
#include <string>

// Boost
#include <boost/program_options.hpp>

// Hudson
#include <Database.hpp>
#include <PositionFactorsSet.hpp>
#include <BnHTrader.hpp>
#include <Report.hpp>
#include <PositionsReport.hpp>

// App
#include "VIXTrader.hpp"

using namespace std;
using namespace boost::gregorian;
using namespace Series;


int main(int argc, char* argv[])
{
  try {

    Database::SeriesFile sf;
    Database::SERIES_MAP mSeries;

    // SPX
    sf.filename = "../../db/SPX.csv";
    sf.driver = EODDB::YAHOO;
    mSeries.insert(Database::SERIES_MAP::value_type("SPX", sf));

    // VIX
    sf.filename = "../../db/VIX.csv";
    sf.driver = EODDB::YAHOO;
    mSeries.insert(Database::SERIES_MAP::value_type("VIX", sf));

    date begin(1991, Jan, 1), end(2008, May, 31);
    Database db(date_period(begin, end), mSeries);
    db.load();
    db.print();

    // Initialize and run strategy
    const EODSeries& spx_db = EODDB::instance().get("SPX");
    const EODSeries& vix_db = EODDB::instance().get("VIX");
    
    /*
    * Initialize and run strategy
    */
    VIXTrader trader(spx_db, vix_db);
    trader.run();
    PositionSet all_positions(trader.positions());

    /*
     * Print open/closed positions
     */
    Report::header("Closed trades");
    trader.positions().closed().print();

    Report::header("Open trades");
    trader.positions().open().print();

    /*
     * Print simulation reports
     */
    Report::header("Trade results");
    ReturnFactors rf(all_positions);
    Report rp(rf);
    rp.print();

    /*
     * Positions excursion
     */
    Report::header("Positions excursion");
    
    PositionFactorsSet pf(all_positions);
    PositionsReport pr(pf);
    pr.print();

  } catch( std::exception& ex ) {

    cerr << "Unhandled exception: " << ex.what() << endl;
    exit(EXIT_FAILURE);
  }

  return 0;
}
