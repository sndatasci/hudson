/*
* Copyright (C) 2007,2008 Alberto Giannetti
*
* This file is part of Hudson.
*
* Hudson is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* Hudson is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with Hudson.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _SERIES_DATABASE_HPP_
#define _SERIES_DATABASE_HPP_

// STL
#include <string>
#include <set>

// Boost
#include <boost/date_time/gregorian/gregorian.hpp>


#ifdef WIN32
#pragma warning (disable:4290)
#endif

namespace Series
{

class DatabaseException: public std::exception
{
public:
  DatabaseException(const std::string& msg):
    _Str("DatabaseException: ")
  {
    _Str += msg;
  }

  virtual ~DatabaseException(void) throw() { }
  virtual const char *what(void) const throw() { return _Str.c_str(); }
    
protected:
  std::string _Str;
};

/*!
  Database is a convenience class to load time series used in Trader simulation.
  \see Trader
  \see Series::EODDB
*/
class Database
{
public:
  Database(const std::string& filename, const boost::gregorian::date_period& dp, const std::set<std::string>& symbols);

  //! Load all time-series defined in class constructor
  void load(void) throw(DatabaseException);

  //! Print loaded series statistics
  void print(void);

protected:
  const std::string _filename;
  const boost::gregorian::date_period _dp;
  const std::set<std::string> _symbols;
};


} // namespace Series

#endif // _SERIES_DATABASE_HPP_
