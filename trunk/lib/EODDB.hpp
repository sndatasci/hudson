/*
* Copyright (C) 2007, Alberto Giannetti
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

#ifndef _SERIES_EODDB_HPP_
#define _SERIES_EODDB_HPP_

// STD
#include <map>
#include <string>
#include <stdexcept>
#include <memory>

// SQLITE
#include <sqlite3.h>

// Hudson
#include "EODSeries.hpp"


namespace Series
{

  class EODDBException: public std::exception
  {
  public:
    EODDBException(const std::string& msg):
      _Str("EODDBException: ")
    {
      _Str += msg;
    }

    virtual ~EODDBException(void) throw() { }
    virtual const char *what(void) const throw() { return _Str.c_str(); }
    
  protected:
    std::string _Str;
  };


  class EODDB
  {
  public:
    enum PriceType {
      PT_NA,
      OPEN,
      CLOSE,
      ADJCLOSE
    };

  public:
    static EODDB& instance(void);

    void load(const std::string& filename, const std::string& symbol,
	      const boost::gregorian::date& begin, const boost::gregorian::date& end) throw(EODDBException);
    const EODSeries& get(const std::string& symbol) const throw(EODDBException);

  protected:
    EODDB(void) { }

    typedef std::map<std::string, EODSeries*> SERIES_MAP;
    SERIES_MAP _mSeries;

  private:
    static std::auto_ptr<EODDB> _pInstance;
  };

} // namespace Series

#endif // _SERIES_EODDB_HPP_
