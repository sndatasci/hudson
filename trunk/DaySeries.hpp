/*
 * DaySeries.hpp
 */

#ifndef _SERIES_DAYSERIES_HPP_
#define _SERIES_DAYSERIES_HPP_

#ifdef WIN32
#pragma warning (disable:4290)
#endif

// STL
#include <iostream>
#include <map>
#include <string>
#include <vector>

// Boost
#include <boost/date_time/gregorian/gregorian.hpp>

// Series
#include "DayPrice.hpp"
#include "FileDriver.hpp"


namespace Series
{

  class DaySeries: public std::map<boost::gregorian::date, Series::DayPrice>
  {
	  typedef std::map<boost::gregorian::date, DayPrice> ThisMap;

  public:
	  DaySeries(const std::string& name, FileDriver& driver);

	  std::string name(void) const { return _name; }
  	
    bool isLoaded(void) const { return _isLoaded; }

	  std::size_t load(const std::string& filename); // load all records returned by the driver
	  std::size_t load(const std::string& filename, const boost::gregorian::date& begin, const boost::gregorian::date& end); // load date range

	  boost::gregorian::date_period period(void) const;
	  boost::gregorian::date_duration duration(void) const;
	  long days(void) const;

    std::vector<double> open(const_iterator iter, unsigned num) const;
    std::vector<double> close(const_iterator iter, unsigned num) const;
    std::vector<double> adjclose(const_iterator iter, unsigned num) const;
    std::vector<double> high(const_iterator iter, unsigned num) const;
    std::vector<double> low(const_iterator iter, unsigned num) const;
    std::vector<double> volume(const_iterator iter, unsigned num) const;

	  ThisMap::const_iterator at_or_before(const boost::gregorian::date& k) const;
	  ThisMap::const_iterator before(const boost::gregorian::date& k, unsigned recs = 1) const;
	  ThisMap::const_iterator after(const boost::gregorian::date& k, unsigned recs = 1) const;
	  ThisMap::const_iterator first_in_month(boost::gregorian::greg_year year, boost::gregorian::greg_month month) const;
	  ThisMap::const_iterator last_in_month(boost::gregorian::greg_year year, boost::gregorian::greg_month month) const;
	  ThisMap::const_iterator first_in_week(boost::gregorian::greg_year year, boost::gregorian::greg_month month, boost::gregorian::greg_day day) const;
	  ThisMap::const_iterator last_in_week(boost::gregorian::greg_year year, boost::gregorian::greg_month month, boost::gregorian::greg_day day) const;

  private:
	  std::string _name;
    bool _isLoaded;
	  FileDriver& _driver;
  };

} // namespace Series

#endif // _SERIES_DAYSERIES_HPP_
