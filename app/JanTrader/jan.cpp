/*
* Copyright (C) 2007,2009 A. Giannetti
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
#include <boost/date_time/gregorian/gregorian.hpp>

// Hudson
#include <Database.hpp>
#include <PositionFactorsSet.hpp>
#include <PositionsReport.hpp>
#include <Report.hpp>

// App
#include "JanTrader.hpp"

using namespace std;
using namespace boost::gregorian;
using namespace Series;


int main(int argc, char* argv[])
{
  try {

    Database::SeriesFile sf;
    Database::SERIES_MAP mSeries;

    // Short SPX
    sf.filename = "../../db/SPX.csv";
    sf.driver = EODDB::YAHOO;
    mSeries.insert(Database::SERIES_MAP::value_type("SPX", sf));

    // Long DFSCX
    sf.filename = "../../db/DFSCX.csv";
    sf.driver = EODDB::YAHOO;
    mSeries.insert(Database::SERIES_MAP::value_type("DFSCX", sf));

    // Load time series
    Database db(date_period(date(1996, Jan, 1), date(2008, Jan, 31)), mSeries);
    db.load();
    db.print();
    
    // Run trading simulation
    JanTrader trader("DFSCX", "SPX");
    trader.run(0, 0); // canonical entry/exit dates (12/20 - 1/9)

    // Print closed positions
    Report::header("Closed trades");
    trader.positions().naturalPos().closed().print();

    Report::header("Strategy trades");
    trader.positions().stratPos().print();

    // Print simulation reports
    Report::header("Trade results");
    ReturnFactors rf(trader.positions().stratPos());
    Report rp(rf);
    rp.print();

    // Print Position stats
    Report::header("Position stats");
    PositionFactorsSet pf(trader.positions().stratPos().closed());
    PositionsReport pr(pf);
    pr.print();
    
  } catch ( std::exception& ex ) {
  
    cerr << ex.what() << endl;
    exit(EXIT_FAILURE);
  }
  
  return 0;
}
