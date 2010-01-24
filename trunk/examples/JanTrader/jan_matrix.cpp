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
#include <boost/date_time/gregorian/gregorian.hpp>

// Hudson
#include <Database.hpp>
#include <PositionFactorsSet.hpp>
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
    
    const int entry_low_offset = -3;
    const int entry_high_offset = 3;
    const int exit_low_offset = -3;
    const int exit_high_offset = 3;
    cout << "Entry, Exit, Avg, Std Dev, Pos, Neg, % Pos, % Neg, Avg Pos, Avg Neg, Best, Worst" << endl;
    cout.precision(4);
    for( int i = entry_low_offset; i <= entry_high_offset; i++ ) {
      for( int j = exit_low_offset; j <= exit_high_offset; j++ ) {

        JanTrader trader("DFSCX", "SPX");
        trader.run(i, j);

        ReturnFactors rf(trader.positions().stratPos());
        ReturnFactors rf_pos(rf.pos());
        ReturnFactors rf_neg(rf.neg());

        double percent_pos = rf_pos.num() / rf.num();
        double percent_neg = rf_neg.num() / rf.num();

        cout << i << ',' << j << ',';
        cout << rf.avg() << ',' << rf.stddev() << ',';
        cout << rf_pos.num() << ',' << rf_neg.num() << ',';
        cout << percent_pos << ',' << percent_neg << ',';
        cout << rf_pos.avg() << ',' << rf_neg.avg() << ',';
        cout << rf.best().factor()-1 << ',' << rf.worst().factor()-1;
        cout << endl;
      }
    }

  } catch( std::exception& ex ) {

    cerr << "Unhandled exception: " << ex.what() << endl;
    exit(-1);
  }

  return 0;
}
