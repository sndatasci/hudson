/*
 * FileDriver.hpp
 */

#ifndef _SERIES_FILEDRIVER_HPP_
#define _SERIES_FILEDRIVER_HPP_

#ifdef WIN32
#pragma warning (disable:4290)
#endif

// STL
#include <string>
#include <stdexcept>

namespace Series
{

  class DriverException: public std::exception
  {
  public:
    DriverException(const std::string& msg):
      _Str("DriverException: ")
    {
      _Str += msg;
    }
    
    virtual ~DriverException(void) throw() { }
    virtual const char *what() const throw() { return _Str.c_str(); }

  private:
    std::string _Str;
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
