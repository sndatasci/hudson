/*
* SeriesFactor.hpp
*/

#ifndef _SERIESFACTOR_HPP_
#define _SERIESFACTOR_HPP_

// Boost
#include <boost/date_time/posix_time/posix_time.hpp>

/*!
* Single daily factor. Includes opening/close date and time associated to factor value.
* \see PositionFactors().
*/
class SeriesFactor
{
public:
  /*! Initialize series factor.
  * \param from Factor date/time begin.
  * \param to Factor date/time end.
  * \param f Factor value.
  */
  SeriesFactor(const boost::posix_time::ptime& from, const boost::posix_time::ptime& to, double f);

  bool operator<(const SeriesFactor& sf) const { return _f < sf._f; }

  //! Factor calculation start date/time.
  const boost::posix_time::ptime& from_tm(void) const { return _from; }
  //! Factor calculation end date/time.
  const boost::posix_time::ptime& to_tm(void) const { return _to; }
  
  //! Factor value.
  double factor(void) const { return _f; }

private:
  boost::posix_time::ptime _from;
  boost::posix_time::ptime _to;
  double _f;
};

#endif // _SERIESFACTOR_HPP_
