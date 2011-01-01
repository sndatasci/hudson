#ifndef _ARGS_HPP_
#define _ARGS_HPP_

// std
#include <string>

// boost
#include <boost/date_time/gregorian/gregorian.hpp>


struct args
{
  args():
    totret(false)
  {
  }

  boost::gregorian::date begin;
  boost::gregorian::date risk_begin;
  boost::gregorian::date end;
  std::string tickers;
  std::string database;
  int totret;
};

#endif // _ARGS_HPP_
