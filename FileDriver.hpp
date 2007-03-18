/*
 * FileDriver.hpp
 */

#ifndef _SERIES_FILEDRIVER_HPP_
#define _SERIES_FILEDRIVER_HPP_

// STL
#include <string>
#include <stdexcept>

namespace Series
{

  class DriverException: public std::exception
  {
  public:
	DriverException(const std::string& what):
	  _what(what)
	{
	}

	virtual ~DriverException(void) throw() { }
	virtual const char* what(void) throw() { return _what.c_str(); }

  protected:
	std::string _what;
  };

  template <class T>
  class FileDriver 
  {
  public:
	virtual bool open(const std::string& filename) = 0;
	virtual void close(void) = 0;
	virtual bool next(T& record) throw(DriverException) = 0;
	virtual bool eof(void) = 0;
  };
  
} // namespace Series

#endif // _SERIES_FILEDRIVER_HPP_
