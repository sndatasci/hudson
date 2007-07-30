/*
* SeriesFactor.hpp
*/

#ifndef _SERIESFACTOR_HPP_
#define _SERIESFACTOR_HPP_

// Boost
#include <boost/date_time/posix_time/posix_time.hpp>


class SeriesFactor
{
public:
  SeriesFactor(const boost::posix_time::ptime& from, const boost::posix_time::ptime& to, double f);

  bool operator<(const SeriesFactor& sf) const { return _f < sf._f; }

  const boost::posix_time::ptime& from_tm(void) const { return _from; }
  const boost::posix_time::ptime& to_tm(void) const { return _to; }
  
  double factor(void) const { return _f; }

private:
  boost::posix_time::ptime _from;
  boost::posix_time::ptime _to;
  double _f;
};

#endif // _SERIESFACTOR_HPP_
