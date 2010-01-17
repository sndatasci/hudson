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
#include <fstream>
#include <string>

// Boost
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/program_options.hpp>

// Hudson
#include <Database.hpp>
#include <EOMReturnFactors.hpp>
#include <PositionFactorsSet.hpp>
#include <BnHTrader.hpp>
#include <EOMReport.hpp>
#include <PositionsReport.hpp>

// App
#include "EOMTrader.hpp"
#include "Args.hpp"

using namespace std;
using namespace boost::gregorian;
using namespace Series;

namespace po = boost::program_options;


int main(int argc, char* argv[])
{
  Args args;

  try {

    // Command line only options
    po::options_description generic_opt("Command line options");
    generic_opt.add_options()
      ("help,h", "print help message")
      ("symbol,s", po::value<string>(&args.symbol), "tested symbol price series")
      ("begin_date,b", po::value<string>(&args.begin_date), "begin of backtest date (YYYYMMDD)")
      ("end_date,e", po::value<string>(&args.end_date), "end of backtest date (YYYYMMDD)")
      ("config,c", po::value<string>(&args.config), "configuration file")
      ;

    // Config file or command line options
    po::options_description config_opt("Configuration options");
    config_opt.add_options()
      ("database", po::value<string>(&args.database), "price database")
      ("table", po::value<string>(&args.table)->default_value("eod"), "price table")
      ("processors", po::value<unsigned int>(&args.cpus)->default_value(1), "number of system CPUs")
      ;

    po::options_description config_file_options;
    config_file_options.add(config_opt);

    po::options_description cmdline_options;
    cmdline_options.add(generic_opt).add(config_opt);

    // Priority to command line options
    po::variables_map opt_vars;
    po::store(parse_command_line(argc, argv, cmdline_options), opt_vars);
    po::notify(opt_vars);

    // Check for config file
    if( ! args.config.empty() ) {
      ifstream ifs(args.config.c_str());
      po::store(parse_config_file(ifs, config_file_options), opt_vars);
      po::notify(opt_vars);
    }

    if( opt_vars.count("help") ) {
      cout << cmdline_options << endl;
      return 0;
    }

    if( args.symbol.empty() ||
	args.begin_date.empty() ||
	args.end_date.empty() ) {
      cerr << cmdline_options << endl;
      return -1;
    }

    set<string> symbols;

    // SPX
    symbols.insert(args.symbol);

    date begin(1981, Jan, 1), end(2008, May, 31);
    Database db(args.database, date_period(begin, end), symbols);
    db.load();
    db.print();

    const EODSeries& spx_db = EODDB::instance().get(args.symbol);

    EOMTrader trader(args.symbol, spx_db);
    trader.run(3, 3);

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

  } catch( std::exception& ex ) {

    cerr << "Unhandled exception: " << ex.what() << endl;
    exit(EXIT_FAILURE);
  }

  return 0;
}
