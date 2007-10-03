/*
* DaySeries.cpp
*/

#include "StdAfx.h"

// Hudson
#include "DaySeries.hpp"
#include "Print.hpp"

using namespace std;


Series::DaySeries::DaySeries(const std::string& name, Series::FileDriver& driver):
  _name(name),
  _isLoaded(false),
  _driver(driver)
{
}


size_t Series::DaySeries::load(const std::string& filename)
{
  ThisMap::clear();

  if( !_driver.open(filename) )
    return 0;

  DayPrice rec;
  while( !_driver.eof() ) {

    try {

      if( _driver.next(rec) == false )
        continue;

      if( ThisMap::insert(ThisMap::value_type(rec.key, rec)).second == false ) {
        cerr << "Duplicate record " << rec.key << endl;
        continue;
      }

    } catch( DriverException& e ) {
      cerr << e.what() << endl;
      continue;
    }

  }	// while not EOF

  _driver.close();

  _isLoaded = true;

  return std::map<boost::gregorian::date, DayPrice>::size();
}


size_t Series::DaySeries::load(const std::string& filename, const boost::gregorian::date& begin, const boost::gregorian::date& end)
{
  ThisMap::clear();

  if( !_driver.open(filename) )
    return 0;

  DayPrice rec;
  while( !_driver.eof() ) {

    try {

      if( _driver.next(rec) == false ) // EOF
        continue;

      if( rec.key < begin || rec.key > end )
        continue;					// out of range

      if( ThisMap::insert(ThisMap::value_type(rec.key, rec)).second == false ) {
        cerr << "Duplicate record " << rec.key << endl;
        continue;
      }

    } catch( DriverException& e ) {
      cerr << e.what() << endl;
      continue;
    }
  }	// while not EOF

  _driver.close();

  _isLoaded = true;

  return ThisMap::size();
}


boost::gregorian::date_period Series::DaySeries::period(void) const
{
  return boost::gregorian::date_period(ThisMap::begin()->first, ThisMap::rbegin()->first);
}


boost::gregorian::date_duration Series::DaySeries::duration(void) const
{
  return boost::gregorian::date_duration(ThisMap::rbegin()->first - ThisMap::begin()->first);
}


long Series::DaySeries::days(void) const
{
  return boost::gregorian::date_duration(ThisMap::rbegin()->first - ThisMap::begin()->first).days();
}


Series::DaySeries::ThisMap::const_iterator Series::DaySeries::at_or_before(const boost::gregorian::date& k) const
{
  ThisMap::const_iterator iter;
  if( (iter = ThisMap::find(k)) != ThisMap::end() )
    return iter;

  return before(k, 1);
}


Series::DaySeries::ThisMap::const_iterator Series::DaySeries::before(const boost::gregorian::date& k, unsigned recs) const
{
  ThisMap::const_iterator iter;
  if( (iter = ThisMap::lower_bound(k)) == ThisMap::begin() && recs > 0 )
    return ThisMap::end();

  for( unsigned i = 0; i < recs; i++ )
    if( --iter == ThisMap::begin() )
      return ThisMap::end();

  return iter;
}


Series::DaySeries::ThisMap::const_iterator Series::DaySeries::after(const boost::gregorian::date& k, unsigned recs) const
{
  ThisMap::const_iterator iter;
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


Series::DaySeries::ThisMap::const_iterator Series::DaySeries::first_in_month(boost::gregorian::greg_year year, boost::gregorian::greg_month month) const
{
  // Call upper_bound() using the last calendar day for previous month to retrieve first record in requested month
  boost::gregorian::date request_date(year, month, 1); // first calendar day of requested month
  boost::gregorian::date key_date = request_date - boost::gregorian::date_duration(1); // last calendar day of previous month
  ThisMap::const_iterator iter = ThisMap::upper_bound(key_date); // first price record after last calendar day of prev month
  if( iter == ThisMap::end() )
    return iter;

  if( iter->first.month() != month ) // we're over the requested month
    return ThisMap::end();

  return iter;
}


Series::DaySeries::ThisMap::const_iterator Series::DaySeries::last_in_month(boost::gregorian::greg_year year, boost::gregorian::greg_month month) const
{
  boost::gregorian::date request_date(year, month, 1);
  ThisMap::const_iterator iter = ThisMap::lower_bound(request_date);
  if( iter == ThisMap::end() )
    return iter;

  while( iter->first.month() == month )
    ++iter;

  return --iter;
}


Series::DaySeries::ThisMap::const_iterator Series::DaySeries::first_in_week(boost::gregorian::greg_year year, boost::gregorian::greg_month month, boost::gregorian::greg_day day) const
{
  boost::gregorian::date request_date(year, month, day);
  boost::gregorian::date begin_of_week;	// Monday in requested date week

  // Look for previous Monday in requested week
  begin_of_week = ( request_date.day_of_week() == boost::gregorian::Monday ) ? request_date :
  boost::gregorian::first_day_of_the_week_before(boost::gregorian::Monday).get_date(request_date); // first Monday before request_date

  // Look for first available day in price database
  ThisMap::const_iterator iter = ThisMap::lower_bound(begin_of_week);
  if( iter == ThisMap::end() )
    return iter;

  // Make sure we're on the same week than requested date
  if( iter->first.week_number() == request_date.week_number() )
    return iter;

  // Different week, sorry no EOW for this request date
  return ThisMap::end();
}


Series::DaySeries::ThisMap::const_iterator Series::DaySeries::last_in_week(boost::gregorian::greg_year year, boost::gregorian::greg_month month, boost::gregorian::greg_day day) const
{
  boost::gregorian::date request_date(year, month, day);
  boost::gregorian::date end_of_week;

  // Look for first Friday starting from requested date (included)
  end_of_week = ( request_date.day_of_week() == boost::gregorian::Friday ) ? request_date :
  boost::gregorian::first_day_of_the_week_after(boost::gregorian::Friday).get_date(request_date);	// first Friday of request_date

  // Look for this Friday in price database
  ThisMap::const_iterator iter = ThisMap::lower_bound(end_of_week);
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


std::vector<double> Series::DaySeries::open( const_iterator iter, unsigned long num ) const
{
  vector<double> v;

  if( !_isLoaded || iter == end() )
    return v;

  // reverse iterator init skips the first element in collection. We must manually insert the current element.
  v.insert(v.begin(), iter->second.open);
  unsigned i = 1;
  for( const_reverse_iterator rev_iter(iter); i < num && rev_iter != rend(); ++rev_iter, ++i )
    v.insert(v.begin(), rev_iter->second.open);

  return v; 
}


std::vector<double> Series::DaySeries::open( void ) const
{
  vector<double> v;

  if( !_isLoaded )
    return v;

  for( const_iterator iter(begin()); iter != end(); ++iter )
    v.push_back(iter->second.open);

  return v;
}


std::vector<double> Series::DaySeries::open( const_iterator itbegin, const_iterator itend ) const
{
  vector<double> v;

  if( !_isLoaded || itbegin == itend || itbegin == end() )
    return v;

  for( const_iterator iter(itbegin); iter != itend; ++iter )
    v.push_back(iter->second.open);

  return v;
}


std::vector<double> Series::DaySeries::close( const_iterator iter, unsigned long num ) const
{
  vector<double> v;

  if( !_isLoaded || iter == end() )
    return v;

  // reverse iterator init skips the first element in collection. We must manually insert the current element.
  v.insert(v.begin(), iter->second.close);
  unsigned i = 1;
  for( const_reverse_iterator rev_iter(iter); i < num && rev_iter != rend(); ++rev_iter, ++i )
    v.insert(v.begin(), rev_iter->second.close);

  return v;
}


std::vector<double> Series::DaySeries::close( void ) const
{
  vector<double> v;

  if( !_isLoaded )
    return v;

  for( const_iterator iter(begin()); iter != end(); ++iter )
    v.push_back(iter->second.close);

  return v;
}


std::vector<double> Series::DaySeries::close( const_iterator itbegin, const_iterator itend ) const
{
  vector<double> v;

  if( !_isLoaded || itbegin == itend || itbegin == end() )
    return v;

  for( const_iterator iter(itbegin); iter != itend; ++iter )
    v.push_back(iter->second.close);

  return v;
}


std::vector<double> Series::DaySeries::adjclose( const_iterator iter, unsigned long num ) const
{
  vector<double> v;

  if( !_isLoaded || iter == end() )
    return v;

  // reverse iterator init skips the first element in collection. We must manually insert the current element.
  v.insert(v.begin(), iter->second.adjclose);
  unsigned i = 1;
  for( const_reverse_iterator rev_iter(iter); i < num && rev_iter != rend(); ++rev_iter, ++i )
    v.insert(v.begin(), rev_iter->second.adjclose);

  return v;
}


std::vector<double> Series::DaySeries::adjclose( void ) const
{
  vector<double> v;

  if( !_isLoaded )
    return v;

  for( const_iterator iter(begin()); iter != end(); ++iter )
    v.push_back(iter->second.adjclose);

  return v;
}


std::vector<double> Series::DaySeries::adjclose( const_iterator itbegin, const_iterator itend ) const
{
  vector<double> v;

  if( !_isLoaded || itbegin == itend || itbegin == end() )
    return v;

  for( const_iterator iter(itbegin); iter != itend; ++iter )
    v.push_back(iter->second.adjclose);

  return v;
}


std::vector<double> Series::DaySeries::high( const_iterator iter, unsigned long num ) const
{
  vector<double> v;

  if( !_isLoaded || iter == end() )
    return v;

  // reverse iterator init skips the first element in collection. We must manually insert the current element.
  v.insert(v.begin(), iter->second.high);
  unsigned i = 1;
  for( const_reverse_iterator rev_iter(iter); i < num && rev_iter != rend(); ++rev_iter, ++i )
    v.insert(v.begin(), rev_iter->second.high);

  return v;
}


std::vector<double> Series::DaySeries::high( void ) const
{
  vector<double> v;

  if( !_isLoaded )
    return v;

  for( const_iterator iter(begin()); iter != end(); ++iter )
    v.push_back(iter->second.high);

  return v;
}


std::vector<double> Series::DaySeries::high( const_iterator itbegin, const_iterator itend ) const
{
  vector<double> v;

  if( !_isLoaded || itbegin == itend || itbegin == end() )
    return v;

  for( const_iterator iter(itbegin); iter != itend; ++iter )
    v.push_back(iter->second.high);

  return v;
}


std::vector<double> Series::DaySeries::low( const_iterator iter, unsigned long num ) const
{
  vector<double> v;

  if( !_isLoaded || iter == end() )
    return v;

  // reverse iterator init skips the first element in collection. We must manually insert the current element.
  v.insert(v.begin(), iter->second.low);
  unsigned i = 1;
  for( const_reverse_iterator rev_iter(iter); i < num && rev_iter != rend(); ++rev_iter, ++i )
    v.insert(v.begin(), rev_iter->second.low);

  return v;
}


std::vector<double> Series::DaySeries::low( void ) const
{
  vector<double> v;

  if( !_isLoaded )
    return v;

  for( const_iterator iter(begin()); iter != end(); ++iter )
    v.push_back(iter->second.low);

  return v;
}


std::vector<double> Series::DaySeries::low( const_iterator itbegin, const_iterator itend ) const
{
  vector<double> v;

  if( !_isLoaded || itbegin == itend || itbegin == end() )
    return v;

  for( const_iterator iter(itbegin); iter != itend; ++iter )
    v.push_back(iter->second.low);

  return v;
}


std::vector<double> Series::DaySeries::volume( const_iterator iter, unsigned long num ) const
{
  vector<double> v;

  if( !_isLoaded || iter == end() )
    return v;

  // reverse iterator init skips the first element in collection. We must manually insert the current element.
  v.insert(v.begin(), iter->second.volume);
  unsigned i = 1;
  for( const_reverse_iterator rev_iter(iter); i < num && rev_iter != rend(); ++rev_iter, ++i )
    v.insert(v.begin(), rev_iter->second.volume);

  return v;
}


std::vector<double> Series::DaySeries::volume( void ) const
{
  vector<double> v;

  if( !_isLoaded )
    return v;

  for( const_iterator iter(begin()); iter != end(); ++iter )
    v.push_back(iter->second.volume);

  return v;
}


std::vector<double> Series::DaySeries::volume( const_iterator itbegin, const_iterator itend ) const
{
  vector<double> v;

  if( !_isLoaded || itbegin == itend || itbegin == end() )
    return v;

  for( const_iterator iter(itbegin); iter != itend; ++iter )
    v.push_back(iter->second.volume);

  return v;
}
