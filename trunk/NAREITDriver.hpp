/*
 * NAREITDriver.hpp
 */

#ifndef _BARRADRIVER_HPP_
#define _BARRADRIVER_HPP_

#ifdef WIN32
#pragma warning (disable:4290)
#endif

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

  class NAREITDriver: public FileDriver
  {
  public:
	  NAREITDriver(void);
	  ~NAREITDriver(void);

	  virtual bool open(const std::string& filename);
	  virtual void close(void);
	  virtual bool next(DayPrice& dp) throw(DriverException);
	  virtual bool eof(void);

  private:
	  enum FIELDS_POS {
	    DATE = 0,
      CLOSE
	  };

  private:
	  std::ifstream _infile;
	  std::string _line;
	  unsigned _linenum;
  };

} // namespace Series

#endif // _NAREITDriver_HPP_
