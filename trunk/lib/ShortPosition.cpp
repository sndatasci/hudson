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

#include "StdAfx.h"

// Boost
#include <boost/date_time/gregorian/gregorian.hpp>

// Hudson
#include "ShortPosition.hpp"
#include "EODDB.hpp"

using namespace std;
using namespace boost::gregorian;
using namespace Series;


ShortPosition::ShortPosition(ID id, const string& symbol, const date& dt, const Price& price, unsigned size) throw(PositionException):
  Position(id, symbol),
  _shorts(0),
  _covers(0),
  _avgShortPrice(0),
  _avgCoverPrice(0)
{
  if( size == 0 )
	  throw PositionException("Invalid size");

  if( dt.is_not_a_date() )
	  throw PositionException("Invalid date");

  if( _sExecutions.sell_short(dt, price, size) == false )
	  throw PositionException("Invalid execution");

  _avgShortPrice = ((_avgShortPrice * _shorts) + (price * size)) / (double)(_shorts + size);

  _size += size;
  _shorts += size;
}


void ShortPosition::sell_short(const date& dt, const Price& price, unsigned size) throw(PositionException)
{
  if( closed() )
	  throw PositionException("Position is closed");

  if( size == 0 )
	  throw PositionException("Invalid size");

  if( dt.is_not_a_date() )
	  throw PositionException("Invalid date");

  if( _sExecutions.sell_short(dt, price, size) == false )
	  throw PositionException("Invalid execution");

  _avgShortPrice = ((_avgShortPrice * _shorts) + (price * size)) / (double)(_shorts + size);

  _size += size;
  _shorts += size;
}


void ShortPosition::cover(const date& dt, const Price& price, unsigned size) throw(PositionException)
{
  if( closed() )
	  throw PositionException("Position is closed");

  if( size == 0 || size > _size )
	  throw PositionException("Invalid size");

  if( dt.is_not_a_date() )
	  throw PositionException("Invalid date");

  if( _sExecutions.cover(dt, price, size) == false )
	  throw PositionException("Invalid execution");

  _avgCoverPrice = ((_avgCoverPrice * _covers) + (price * size)) / (_covers + size);

  _size -= size;
  _covers += size;
}


void ShortPosition::close(const date& dt, const Price& price) throw(PositionException)
{
  if( closed() )
	  throw PositionException("Position is closed");

  cover(dt, price, _size);
}


void ShortPosition::close( const boost::gregorian::date& dt, Series::EODDB::PriceType pt ) throw(PositionException)
{
  if( closed() )
    throw PositionException("Position is closed");

  // Retrieve market price
  Series::EODSeries::const_iterator citer = Series::EODDB::instance().get(_symbol).find(dt);
  if( citer == Series::EODDB::instance().get(_symbol).end() )
    throw PositionException("Can't get EODSeries price record");

  double priceval = 0;
  switch( pt ) {

    case EODDB::OPEN:
      priceval = citer->second.open;
      break;

    case EODDB::CLOSE:
      priceval = citer->second.close;
      break;

    case EODDB::ADJCLOSE:
      priceval = citer->second.adjclose;
      break;
      
    case EODDB::LIMIT:
      throw PositionException("Limit price not implemented yet");

    default:
      throw PositionException("Invalid price type");
  }

  cover(dt, Price(priceval), _size);
}


void ShortPosition::buy(const boost::gregorian::date& dt, const Price& price, unsigned size) throw(PositionException)
{
  throw PositionException("Short position");
}


void ShortPosition::sell(const boost::gregorian::date& dt, const Price& price, unsigned size) throw(PositionException)
{
  throw PositionException("Short position");
}


double ShortPosition::factor( void ) const throw(PositionException)
{
  if( _avgShortPrice <= 0 )
    throw PositionException("Invalid average short price");

  if( closed() && _avgCoverPrice <= 0 )
    throw PositionException("Invalid average cover price");

  return closed() ? _avgShortPrice / _avgCoverPrice : _avgShortPrice / Series::EODDB::instance().get(_symbol).last().adjclose;
}


double ShortPosition::factor( const Price& price ) const throw(PositionException)
{
  if( closed() )
    throw PositionException("Closed position");

  if( _avgShortPrice <= 0 )
    throw PositionException("Invalid short price");

  return _avgShortPrice / price.value();
}


double ShortPosition::factor( const Price& prev_price, const Price& curr_price ) const throw(PositionException)
{
  return prev_price / curr_price;
}


double ShortPosition::factor( const boost::gregorian::date::month_type& month, const boost::gregorian::date::year_type& year ) const throw(PositionException)
{
  // Generate monthly begin/end dates
  date period_month(year, month, 1);

  date begin_mark = period_month - days(1); // Last day in previous month
  date end_mark = period_month.end_of_month();

  // If position was opened after end_mark or was closed before begin_mark, period is out of range
  if( first_exec().dt() > end_mark || (closed() && last_exec().dt() < begin_mark) )
    throw PositionException("Position executions are not included in given range");

  // Get symbol series
  const Series::EODSeries& db = Series::EODDB::instance().get(_symbol);

  // Extract begin of period price
  double begin_price = 0;
  // If position was opened before begin mark, then use begin_mark price (previous month last close)
  if( first_exec().dt() <= begin_mark ) {
    EODSeries::const_iterator citer = db.at_or_before(begin_mark);
    if( citer == db.end() )
      throw PositionException("Can't get begin-period price");

    begin_price = citer->second.adjclose;
    //cout << "Position opened before or at previous EOM mark price, using " << citer->first << " adjclose" << endl;

    // Else if position was opened after begin mark, use position opening price
  } else if( first_exec().dt() > begin_mark && first_exec().dt() <= end_mark ) {
    begin_price = avgEntryPrice();
    //cout << "Position opened after previous EOM mark price, using position avg entry price" << endl;

    // Should never end up here 
  } else {
    throw PositionException("Position factor(): Internal error on first execution check");
  }

  // Extract end of period price
  double end_price = 0;
  // If position is open or was closed after end-month mark, use end-month price
  if( open() || last_exec().dt() > end_mark ) {
    EODSeries::const_iterator citer = db.at_or_before(end_mark);
    if( citer == db.end() )
      throw PositionException("Can't get end-period price");

    end_price = citer->second.adjclose;
    //cout << "Position still open or closed after EOM mark price, using " << em_mark->first << " adjclose" << endl;

    // Else if position closing execution is before end-month mark, use execution price
  } else if( last_exec().dt() <= end_mark ) {
    end_price = avgExitPrice();
    //cout << "Position closed before EOM mark price, using position avg exit price" << endl;

    // Should never finish here
  } else {
    throw PositionException("Position factor(): Internal error on last execution check");
  }

  // Return monthly factors
  return begin_price / end_price;
}