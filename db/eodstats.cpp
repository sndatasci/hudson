// STD
#include <getopt.h>
#include <string>

// Boost
#include <boost/date_time/gregorian/gregorian.hpp>

#include "args.hpp"
#include "stats.hpp"

using namespace std;
using namespace boost::gregorian;

void usage()
{
  cerr << "Usage: eodstats --begin YYYYMMDD --rbegin YYYYMMDD --end YYYYMMDD --tickers tickers_file --database database_file" << endl;
}


int main(int argc, char* argv[])
{
  args margs;
  static struct option longopts[] = {
    { "begin",       required_argument,      NULL,            'b' },
    { "rbegin",      required_argument,      NULL,            'r' },
    { "end",         required_argument,      NULL,            'e' },
    { "tickers",     required_argument,      NULL,            't' },
    { "database",    required_argument,      NULL,            'd' },
    { NULL,          0,                      NULL,             0  }
  };

  int c;
  while( (c = getopt_long(argc, argv, "b:e:d:t:r:", longopts, NULL)) != -1 ) {
    switch( c ) {
    case 'b':
      {
        try {
          date arg_date(from_undelimited_string(optarg));
          margs.begin = arg_date;
        } catch( const exception& ex ) {
          cerr << "Invalid begin date: " << optarg << endl;
          exit(-1);
        }
      }
      break;

    case 'r':
      {
        try {
          date arg_date(from_undelimited_string(optarg));
          margs.risk_begin = arg_date;
        } catch( const exception& ex ) {
          cerr << "Invalid risk begin date: " << optarg << endl;
          exit(-1);
        }
      }
      break;

    case 'e':
      {
        try {
          date arg_date(from_undelimited_string(optarg));
          margs.end = arg_date;
        } catch( const exception& ex ) {
          cerr << "Invalid end date: " << optarg << endl;
          exit(-1);
        }
      }
      break;

    case 'd':
      margs.database = optarg;
      break;

    case 't':
      margs.tickers = optarg;
      break;

    case 0:
      break;

    default:
      break;
    }
  }

  if( margs.tickers.empty() || margs.database.empty() ) {
    usage();
    exit(-1);
  }

  if( margs.risk_begin > margs.begin ) {
    margs.risk_begin = margs.begin;
  }

  stats s(margs);
  s.calc();
  cout << s;

  return 0;
}
