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
#include <EOMReturnFactors.hpp>
#include <PositionFactorsSet.hpp>
#include <PositionsReport.hpp>
#include <BnHTrader.hpp>
#include <EOMReport.hpp>

// App
#include "BOWTrader.hpp"

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

    date begin(1981, Jan, 1), end(2008, May, 31);
    Database db(date_period(begin, end), mSeries);
    db.load();
    db.print();

    const EODSeries& spx_db = EODDB::instance().get("SPX");

    /*
     * Initialize and run strategy
     */
    BOWTrader trader("SPX", spx_db);
    trader.run(0, 1);

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
    ReturnFactors rf(trader.positions());
    Report rp(rf);
    rp.print();

    /*
     * Positions excursion
     */
    Report::header("Positions excursion");
    PositionFactorsSet pf(trader.positions().closed());
    PositionsReport pr(pf);
    pr.print();

    // BnH
    Report::header("BnH");
    BnHTrader bnh(spx_db);
    bnh.run();
    EOMReturnFactors bnh_rf(bnh.positions(), begin, end);
    EOMReport bnh_rp(bnh_rf);
    bnh_rp.print();

  } catch( Series::DriverException& e ) {
    cerr << "Driver error: " << e.what() << endl;
    exit(EXIT_FAILURE);

  } catch( out_of_range& e ) {
    cerr << "Can't get begin/end dates: " << e.what() << endl;
    exit(EXIT_FAILURE);

  } catch( exception& e ) {
    cerr << "Error: " << e.what() << endl;
    exit(EXIT_FAILURE);
  }

  return 0;
}
