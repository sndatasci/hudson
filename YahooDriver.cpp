/*
 * YahooDriver.cpp
 */

// Posix
#include <cstdlib>

// Boost
#include <boost/tokenizer.hpp>

// Series
#include "YahooDriver.hpp"

using namespace std;
using namespace boost;
using namespace boost::gregorian;


Series::YahooDriver::YahooDriver(void):
  _linenum(0)
{
}


Series::YahooDriver::~YahooDriver(void)
{
  if( _infile.is_open() )
	_infile.close();
}


bool Series::YahooDriver::open(const std::string& filename)
{
  // Check if another file was open previously
  if( _infile.is_open() ) {
	_infile.close();
	_linenum = 0;
  }

  _infile.open(filename.c_str());
  if( _infile.is_open() == false )
	return false;

  // First line is header line
  getline(_infile, _line);
  _linenum = 1;

  return _infile.good();
}


void Series::YahooDriver::close(void)
{
  if( _infile.is_open() )
	_infile.close();

  _infile.clear();

  _linenum = 0;
}


bool Series::YahooDriver::next(DayPrice& dp) throw(Series::DriverException)
{
  if( _infile.eof() )
	return false;

  typedef tokenizer<boost::char_separator<char> > TokensRdr;
  getline(_infile, _line);
  ++_linenum;

  // Get line fields
  int i = 0;
  TokensRdr tok(_line, boost::char_separator<char>(" ,\t\n\r"));
  for( TokensRdr::iterator iter = tok.begin(); iter != tok.end(); ++iter, ++i ) {

	string field = *iter;		// extract line field (.csv file)

	switch( i ) {

	case DATE: {
	  dp.key = from_string(field);
	  if( dp.key.is_not_a_date() ) {
		stringstream ss;
		ss << "Invalid key at line " << _linenum;
		throw DriverException(ss.str());
	  }
	}
	break;

	case OPEN:
	  dp.open = atof(field.c_str());
	  break;

	case HIGH:
	  dp.high = atof(field.c_str());
	  break;

	case LOW:
	  dp.low = atof(field.c_str());
	  break;

	case CLOSE:
	  dp.close = atof(field.c_str());
	  break;

	case VOLUME:
	  dp.volume = atoi(field.c_str());
	  break;

	case ADJCLOSE:
	  dp.adjclose = atof(field.c_str());
	  break;

	default: {
		stringstream ss;
		ss << "Unknown field at line " << _linenum;
		throw DriverException(ss.str());
	  }
	}
  }

  return true;
}


bool Series::YahooDriver::eof(void)
{
  return _infile.eof();
}

