/*
 * DayPrice.hpp
 */

#ifndef _SERIES_DAYPRICE_HPP_
#define _SERIES_DAYPRICE_HPP_

#ifdef WIN32
#pragma warning (disable:4290)
#endif

// Boost
#include <boost/date_time/gregorian/gregorian.hpp>


namespace Series
{

  class DayPrice
  {
  public:
	boost::gregorian::date key;

	double high;
	double low;
	double open;
	double close;
	double adjclose;
	unsigned long volume;
  };

} // namespace Series

#endif // _SERIES_DAYPRICE_HPP_
