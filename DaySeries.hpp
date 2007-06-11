/*
 * DaySeries.hpp
 */

#ifndef _SERIES_DAYSERIES_HPP_
#define _SERIES_DAYSERIES_HPP_

#include <iostream>

// STL
#include <map>
#include <string>

// Boost
#include <boost/date_time/gregorian/gregorian.hpp>

// Series
#include "FileDriver.hpp"


namespace Series
{

  template <class T>
  class DaySeries: public std::map<boost::gregorian::date, T>
  {
	typedef std::map<boost::gregorian::date, T> ThisMap;

  public:
	DaySeries(const std::string& name, FileDriver<T>& driver);

	std::string name(void) const { return _name; }
	
	int load(const std::string& filename); // load all records returned by the driver
	int load(const std::string& filename, const boost::gregorian::date& begin, const boost::gregorian::date& end); // load date range

	boost::gregorian::date_period period(void) const;
	boost::gregorian::date_duration duration(void) const;
	long days(void) const;

	typename ThisMap::const_iterator at_or_before(const boost::gregorian::date& k) const;
	typename ThisMap::const_iterator before(const boost::gregorian::date& k, unsigned recs = 1) const;
	typename ThisMap::const_iterator after(const boost::gregorian::date& k, unsigned recs = 1) const;
	typename ThisMap::const_iterator first_in_month(boost::gregorian::greg_year year, boost::gregorian::greg_month month) const;
	typename ThisMap::const_iterator last_in_month(boost::gregorian::greg_year year, boost::gregorian::greg_month month) const;
	typename ThisMap::const_iterator first_in_week(boost::gregorian::greg_year year, boost::gregorian::greg_month month, boost::gregorian::greg_day day) const;
	typename ThisMap::const_iterator last_in_week(boost::gregorian::greg_year year, boost::gregorian::greg_month month, boost::gregorian::greg_day day) const;

  private:
	std::string _name;
	FileDriver<T>& _driver;
  };

} // namespace Series


template <typename T>
Series::DaySeries<T>::DaySeries(const std::string& name, Series::FileDriver<T>& driver):
  _name(name),
  _driver(driver)
{
}


template <typename T>
boost::gregorian::date_period Series::DaySeries<T>::period(void) const
{
  return boost::gregorian::date_period(ThisMap::begin()->first, ThisMap::rbegin()->first);
}


template <typename T>
boost::gregorian::date_duration Series::DaySeries<T>::duration(void) const
{
  return boost::gregorian::date_duration(ThisMap::rbegin()->first - ThisMap::begin()->first);
}


template <typename T>
long Series::DaySeries<T>::days(void) const
{
  return boost::gregorian::date_duration(ThisMap::rbegin()->first - ThisMap::begin()->first).days();
}


template <typename T>
typename Series::DaySeries<T>::ThisMap::const_iterator Series::DaySeries<T>::at_or_before(const boost::gregorian::date& k) const
{
  typename ThisMap::const_iterator iter;
  if( (iter = ThisMap::find(k)) != ThisMap::end() )
	return iter;

  return before(k, 1);
}


template <typename T>
typename Series::DaySeries<T>::ThisMap::const_iterator Series::DaySeries<T>::before(const boost::gregorian::date& k, unsigned recs) const
{
  typename ThisMap::const_iterator iter;
  if( (iter = ThisMap::lower_bound(k)) == ThisMap::begin() && recs > 0 )
	return ThisMap::end();

  for( unsigned i = 0; i < recs; i++ )
	if( --iter == ThisMap::begin() )
	  return ThisMap::end();

  return iter;
}


template <typename T>
typename Series::DaySeries<T>::ThisMap::const_iterator Series::DaySeries<T>::after(const boost::gregorian::date& k, unsigned recs) const
{
  typename ThisMap::const_iterator iter;
  if( (iter = ThisMap::find(k)) == ThisMap::end() ) {
	if( (iter = ThisMap::upper_bound(k)) == ThisMap::end() ) {
	  return ThisMap::end();				// k out of range
	} else {
	  // returning from upper_bound(), we are already one record past the key
	  if( recs ) --recs;
	}
  }

  for( unsigned i = 0; i < recs; i++ )
	if( ++iter == ThisMap::end() )
	  return ThisMap::end();

  return iter;
}


template <typename T>
typename Series::DaySeries<T>::ThisMap::const_iterator Series::DaySeries<T>::first_in_month(boost::gregorian::greg_year year, boost::gregorian::greg_month month) const
{
  // Call upper_bound() using the last calendar day for previous month to retrieve first record in requested month
  boost::gregorian::date request_date(year, month, 1); // first calendar day of requested month
  boost::gregorian::date key_date = request_date - boost::gregorian::date_duration(1); // last calendar day of previous month
  typename ThisMap::const_iterator iter = ThisMap::upper_bound(key_date); // first price record after last calendar day of prev month
  if( iter == ThisMap::end() )
	return iter;

  if( iter->first.month() != month ) // we're over the requested month
	return ThisMap::end();

  return iter;
}


template <typename T>
typename Series::DaySeries<T>::ThisMap::const_iterator Series::DaySeries<T>::last_in_month(boost::gregorian::greg_year year, boost::gregorian::greg_month month) const
{
  boost::gregorian::date request_date(year, month, 1);
  typename ThisMap::const_iterator iter = ThisMap::lower_bound(request_date);
  if( iter == ThisMap::end() )
	return iter;

  while( iter->first.month() == month )
	++iter;

  return --iter;
}


template <typename T>
typename Series::DaySeries<T>::ThisMap::const_iterator Series::DaySeries<T>::first_in_week(boost::gregorian::greg_year year, boost::gregorian::greg_month month, boost::gregorian::greg_day day) const
{
  boost::gregorian::date request_date(year, month, day);
  boost::gregorian::date begin_of_week;	// Monday in requested date week

  // Look for previous Monday in requested week
  begin_of_week = ( request_date.day_of_week() == boost::gregorian::Monday ) ? request_date :
	boost::gregorian::first_day_of_the_week_before(boost::gregorian::Monday).get_date(request_date); // first Monday before request_date

  // Look for first available day in price database
  typename ThisMap::const_iterator iter = ThisMap::lower_bound(begin_of_week);
  if( iter == ThisMap::end() )
	return iter;

  // Make sure we're on the same week than requested date
  if( iter->first.week_number() == request_date.week_number() )
	return iter;

  // Different week, sorry no EOW for this request date
  return ThisMap::end();
}


template <typename T>
typename Series::DaySeries<T>::ThisMap::const_iterator Series::DaySeries<T>::last_in_week(boost::gregorian::greg_year year, boost::gregorian::greg_month month, boost::gregorian::greg_day day) const
{
  boost::gregorian::date request_date(year, month, day);
  boost::gregorian::date end_of_week;
  
  // Look for first Friday starting from requested date (included)
  end_of_week = ( request_date.day_of_week() == boost::gregorian::Friday ) ? request_date :
	boost::gregorian::first_day_of_the_week_after(boost::gregorian::Friday).get_date(request_date);	// first Friday of request_date

  // Look for this Friday in price database
  typename ThisMap::const_iterator iter = ThisMap::lower_bound(end_of_week);
  if( iter == ThisMap::end() )
	return iter;

  // lower_bound() returns next week first record if Friday couldn't be found
  if( iter->first.day_of_week() == boost::gregorian::Friday )
	return iter;

  // We're on next week. Go back one record to locate requested EOW
  --iter;
  if( iter->first.week_number() == request_date.week_number() )
	return iter;

  // Can't find EOW
  return ThisMap::end();
}


template <typename T>
int Series::DaySeries<T>::load(const std::string& filename)
{
  ThisMap::clear();

  if( !_driver.open(filename) )
	return 0;

  T rec;
  while( !_driver.eof() ) {

	try {

	  if( _driver.next(rec) == false )
		continue;

	  if( ThisMap::insert(typename ThisMap::value_type(rec.key, rec)).second == false ) {
		std::cerr << "Duplicate record " << rec.key << std::endl;
		continue;
	  }

	} catch( DriverException& e ) {
	  std::cerr << e.what() << std::endl;
	  continue;
	}

  }	// while not EOF

  _driver.close();

  return std::map<boost::gregorian::date, T>::size();
}


template <typename T>
int Series::DaySeries<T>::load(const std::string& filename, const boost::gregorian::date& begin, const boost::gregorian::date& end)
{
  ThisMap::clear();

  if( !_driver.open(filename) )
	return 0;

  T rec;
  while( !_driver.eof() ) {

	try {

	  if( _driver.next(rec) == false ) // EOF
		continue;

	  if( rec.key < begin || rec.key > end )
		continue;					// out of range

	  if( ThisMap::insert(typename ThisMap::value_type(rec.key, rec)).second == false ) {
		std::cerr << "Duplicate record " << rec.key << std::endl;
		continue;
	  }

	} catch( DriverException& e ) {
	  std::cerr << e.what() << std::endl;
	  continue;
	}
  }	// while not EOF

  _driver.close();

  return ThisMap::size();
}


#endif // _SERIES_DAYSERIES_HPP_
