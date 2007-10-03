/*
 * aa.cpp
 */

// Boost
#include <boost/program_options.hpp>

// Hudson
#include "YahooDriver.hpp"
#include "BarraDriver.hpp"
#include "NAREITDriver.hpp"
#include "DaySeries.hpp"
#include "DayPrice.hpp"
#include "ReturnFactors.hpp"
#include "PositionFactors.hpp"
#include "AATrader.hpp"
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
  string spx_dbfile, tnx_dbfile, djc_dbfile, eafe_dbfile, reit_dbfile;

  try {

    /*
    * Extract simulation options
    */
	  po::options_description desc("Allowed options");
	  desc.add_options()
	    ("help", "produce help message")
	    ("spx_file", po::value<string>(&spx_dbfile),     "SPX series database")
	    ("tnx_file", po::value<string>(&tnx_dbfile),     "TNX series database")
	    ("djc_file", po::value<string>(&djc_dbfile),     "DJC series database")
	    ("eafe_file", po::value<string>(&eafe_dbfile),   "EAFE series database")
      ("reit_file", po::value<string>(&reit_dbfile),   "REIT series database")
	    ("begin_date", po::value<string>(&begin_date),   "start of trading period (YYYY-MM-DD)")
	    ("end_date", po::value<string>(&end_date),       "end of trading period (YYYY-MM-DD)")
	    ;

	  po::variables_map vm;
	  po::store(po::parse_command_line(argc, argv, desc), vm);
	  po::notify(vm);

	  if( vm.count("help") ) {
	    cout << desc << endl;
	    exit(0);
	  }

	  if( vm["spx_file"].empty() ||
        vm["tnx_file"].empty() ||
        vm["djc_file"].empty() ||
        vm["eafe_file"].empty() ||
        vm["reit_file"].empty() ||
		    vm["begin_date"].empty() || vm["end_date"].empty() ) {
	    cout << desc << endl;
	    exit(1);
	  }

	  cout << "SPX file: " << spx_dbfile << endl;
    cout << "TNX file: " << tnx_dbfile << endl;
    cout << "DJC file: " << djc_dbfile << endl;
    cout << "EAFE file: " << eafe_dbfile << endl;
    cout << "REIT file: " << reit_dbfile << endl;

    /*
    * Load series data
    */
    YahooDriver yd;
    BarraDriver bd;
    NAREITDriver nd;
  
    DaySeries spx_db("SPX", yd);
    DaySeries tnx_db("TNX", yd);
    DaySeries djc_db("DJC", yd);
    DaySeries eafe_db("EAFE", bd);
    DaySeries reit_db("REIT", nd);

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

	  cout << "Loading " << spx_dbfile << " from " << to_simple_string(load_begin) << " to " << to_simple_string(load_end) << "..." << endl;
	  if( spx_db.load(spx_dbfile, load_begin, load_end) <= 0 ) {
	    cerr << "No records found" << endl;
	    exit(EXIT_FAILURE);
	  }

    cout << "Loading " << tnx_dbfile << " from " << to_simple_string(load_begin) << " to " << to_simple_string(load_end) << "..." << endl;
    if( tnx_db.load(tnx_dbfile, load_begin, load_end) <= 0 ) {
      cerr << "No records found" << endl;
      exit(EXIT_FAILURE);
    }

    cout << "Loading " << djc_dbfile << " from " << to_simple_string(load_begin) << " to " << to_simple_string(load_end) << "..." << endl;
    if( djc_db.load(djc_dbfile, load_begin, load_end) <= 0 ) {
      cerr << "No records found" << endl;
      exit(EXIT_FAILURE);
    }

    cout << "Loading " << eafe_dbfile << " from " << to_simple_string(load_begin) << " to " << to_simple_string(load_end) << "..." << endl;
    if( eafe_db.load(eafe_dbfile, load_begin, load_end) <= 0 ) {
      cerr << "No records found" << endl;
      exit(EXIT_FAILURE);
    }

    cout << "Loading " << reit_dbfile << " from " << to_simple_string(load_begin) << " to " << to_simple_string(load_end) << "..." << endl;
    if( reit_db.load(reit_dbfile, load_begin, load_end) <= 0 ) {
      cerr << "No records found" << endl;
      exit(EXIT_FAILURE);
    }

    cout << "SPX Records: " << spx_db.size() << endl;
    cout << "SPX Period: " << spx_db.period() << endl;
    cout << "SPX Total days: " << spx_db.duration().days() << endl;

    cout << "TNX Records: " << tnx_db.size() << endl;
    cout << "TNX Period: " << tnx_db.period() << endl;
    cout << "TNX Total days: " << tnx_db.duration().days() << endl;

    cout << "DJC Records: " << djc_db.size() << endl;
    cout << "DJC Period: " << djc_db.period() << endl;
    cout << "DJC Total days: " << djc_db.duration().days() << endl;

    cout << "EAFE Records: " << eafe_db.size() << endl;
    cout << "EAFE Period: " << eafe_db.period() << endl;
    cout << "EAFE Total days: " << eafe_db.duration().days() << endl;

    cout << "REIT Records: " << reit_db.size() << endl;
    cout << "REIT Period: " << reit_db.period() << endl;
    cout << "REIT Total days: " << reit_db.duration().days() << endl;

    /*
    * Initialize and run strategy
    */
    AATrader trader(spx_db, tnx_db, djc_db, eafe_db, reit_db);
    
    trader.run();
    trader.positions().closed().print();

    // Print simulation reports
    ReturnFactors rf(trader.positions().closed(), spx_db.duration().days(), 12);
    PositionFactorsSet pf(trader.positions().closed(), spx_db);

    Report rp(rf);
    rp.print();

    // Positions excursion
    cout << endl << "Position Excursions" << endl << "--" << endl;
    PositionsReport pr(pf);
    pr.print();

    // BnH
    cout << endl << "B&H" << endl << "--" << endl;
    BnHTrader bnh(spx_db);
    bnh.run();
    ReturnFactors bnh_rf(bnh.positions().closed(), spx_db.duration().days(), 12);
    Report bnh_rp(bnh_rf);

    bnh_rp.roi();
    bnh_rp.cagr();

  } catch( std::exception& ex ) {

	  cerr << ex.what() << endl;
	  exit(EXIT_FAILURE);
  }

  return 0;
}
