/*
 * YahooDriver.hpp
 */

#ifndef _YAHOODRIVER_HPP_
#define _YAHOODRIVER_HPP_

// STL
#include <fstream>
#include <string>

// Boost
#include <boost/date_time/gregorian/gregorian.hpp>

// Series
#include "FileDriver.hpp"
#include "DayPrice.hpp"

namespace Series
{

  class YahooDriver: public FileDriver<DayPrice>
  {
  public:
	YahooDriver(void);
	~YahooDriver(void);

	virtual bool open(const std::string& filename);
	virtual void close(void);
	virtual bool next(DayPrice& dp) throw(DriverException);
	virtual bool eof(void);

  private:
	enum FIELDS_POS {
	  DATE = 0,
	  OPEN,
	  HIGH,
	  LOW,
	  CLOSE,
	  VOLUME,
	  ADJCLOSE
	};

  private:
	std::ifstream _infile;
	std::string _line;
	unsigned _linenum;
  };

} // namespace Series

#endif // _YAHOODRIVER_HPP_
