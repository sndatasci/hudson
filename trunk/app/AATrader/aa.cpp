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

// STL
#include <iostream>
#include <string>

// Boost
#include <boost/date_time/gregorian/gregorian.hpp>

// Hudson
#include <Database.hpp>
#include <PositionFactors.hpp>
#include <PositionFactorsSet.hpp>
#include <BnHTrader.hpp>
#include <EOMReturnFactors.hpp>
#include <EOMReport.hpp>
#include <PositionsReport.hpp>
#include <PortfolioReturns.hpp>
#include <PortfolioReport.hpp>

// App
#include "AATrader.hpp"

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

    // TNX
    sf.filename = "../../db/TNX.csv";
    sf.driver = EODDB::YAHOO;
    mSeries.insert(Database::SERIES_MAP::value_type("TNX", sf));

    // DJC
    sf.filename = "../../db/DJC.csv";
    sf.driver = EODDB::YAHOO;
    mSeries.insert(Database::SERIES_MAP::value_type("DJC", sf));

    // EAFE
    sf.filename = "../../db/EAFE.csv";
    sf.driver = EODDB::DMYC;
    mSeries.insert(Database::SERIES_MAP::value_type("EAFE", sf));

    // REIT
    sf.filename = "../../db/NAREIT_All.csv";
    sf.driver = EODDB::DMYC;
    mSeries.insert(Database::SERIES_MAP::value_type("REIT", sf));

    date begin(1991, Jan, 1), end(2008, May, 31);
    Database db(date_period(begin, end), mSeries);
    db.load();
    db.print();
    
    // Initialize and run strategy
    const EODSeries& spx_db = EODDB::instance().get("SPX");
    const EODSeries& tnx_db = EODDB::instance().get("TNX");
    const EODSeries& djc_db = EODDB::instance().get("DJC");
    const EODSeries& eafe_db = EODDB::instance().get("EAFE");
    const EODSeries& reit_db = EODDB::instance().get("REIT");

    AATrader trader(spx_db, tnx_db, djc_db, eafe_db, reit_db);
    trader.run();

    // Print trades  
    Report::header("Closed trades");
    trader.positions("SPX").closed().print();
    trader.positions("TNX").closed().print();
    trader.positions("DJC").closed().print();
    trader.positions("EAFE").closed().print();
    trader.positions("REIT").closed().print();
    
    Report::header("Open trades");
    trader.positions("SPX").open().print();
    trader.positions("TNX").open().print();
    trader.positions("DJC").open().print();
    trader.positions("EAFE").open().print();
    trader.positions("REIT").open().print();
            
    // SPX stats
    Report::header("SPX Stats");
    EOMReturnFactors spx_eomrf(trader.positions("SPX"), begin, end);
    EOMReport rp(spx_eomrf);
    rp.print();
    
    // TNX stats
    Report::header("TNX Stats");
    EOMReturnFactors tnx_eomrf(trader.positions("TNX"), begin, end);
    EOMReport tnx_rp(tnx_eomrf);
    tnx_rp.print();
    
    // DJC stats
    Report::header("DJC Stats");
    EOMReturnFactors djc_eomrf(trader.positions("DJC"), begin, end);
    EOMReport djc_rp(djc_eomrf);
    djc_rp.print();
    
    // EAFE stats
    Report::header("EAFE Stats");
    EOMReturnFactors eafe_eomrf(trader.positions("EAFE"), begin, end);
    EOMReport eafe_rp(eafe_eomrf);
    eafe_rp.print();
    
    // REIT stats
    Report::header("REIT Stats");
    EOMReturnFactors reit_eomrf(trader.positions("REIT"), begin, end);
    EOMReport reit_rp(reit_eomrf);
    reit_rp.print();
    
    // Position analysis
    //Report::header("Positions");
    //PositionFactorsSet pfs(trader.positions());
    //PositionsReport pr(pfs);
    //pr.print();
    
    // Portfolio stats
    Report::header("Portfolio Stats");
    PortfolioReturns pr;
    pr.add(&spx_eomrf);
    pr.add(&tnx_eomrf);
    pr.add(&djc_eomrf);
    pr.add(&eafe_eomrf);
    pr.add(&reit_eomrf);
    PortfolioReport preport(pr);
    preport.print();

    // BnH
    Report::header("SPX BnH");
    BnHTrader bnh(spx_db); 
    bnh.run();
    bnh.positions().print();
    EOMReturnFactors bnh_eomrf(bnh.positions(), begin, end);
    EOMReport bnh_rp(bnh_eomrf);
    bnh_rp.print();

  } catch( std::exception& ex ) {

    cerr << "Unhandled exception: " << ex.what() << endl;
    exit(EXIT_FAILURE);
  }

  return 0;
}
