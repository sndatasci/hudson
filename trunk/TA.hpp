/*
* TA.hpp
*/

#ifndef _TA_HPP_ 
#define _TA_HPP_

#ifdef WIN32
#pragma warning (disable:4290)
#endif

// STL
#include <stdexcept>
#include <string>


class TAException: public std::exception
{
  public:
    TAException(const std::string& msg):
      _Str("TAException: ")
    {
      _Str += msg;
    }

    virtual ~TAException(void) throw() { }
    virtual const char *what() const throw() { return _Str.c_str(); }

  protected:
    std::string _Str;
};


class TA
{
public:
  TA(void) throw(TAException);
  ~TA(void);
};

#endif // _TA_HPP_
