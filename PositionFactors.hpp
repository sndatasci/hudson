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

#ifndef _POSITIONFACTORS_HPP_
#define _POSITIONFACTORS_HPP_

#ifdef WIN32
#pragma warning (disable:4290)
#endif

// CSTD
#include <cmath>

// STL
#include <stdexcept>

// Hudson
#include "EODSeries.hpp"
#include "Position.hpp"
#include "SeriesFactor.hpp"
#include "SeriesFactorSet.hpp"


class PositionFactorsException: public std::exception
{
  public:
    PositionFactorsException(const std::string& msg):
      _Str("PositionFactorsException: ")
    {
      _Str += msg;
    }

    virtual ~PositionFactorsException(void) throw() { }
    virtual const char *what() const throw() { return _Str.c_str(); }

  protected:
    std::string _Str;
};


/*!
* Position daily factors. Used to analyze position adverse/favorable excursions.
*/
class PositionFactors
{
  public:
    /*!
    * Initialize position factors by passing a position and the series database that encompass all execution dates for this position.
    * \param pos The position that must be analyzed.
    * \param db Series database. It should include data for all series from the first opening execution to the closing execution, or the
    * current date if the position is still open.
    */
    PositionFactors(const Position& pos, const Series::EODSeries& db);

    //! Maximum consecutive positive daily factors
    SeriesFactorSet max_cons_pos(void) const;
    //! Maximum consecutive negative daily factors
    SeriesFactorSet max_cons_neg(void) const;

    //! Best favorable excursion period for this position
    SeriesFactorSet bfe(void) const throw(PositionFactorsException);
    //! Worst adverse excursion period for this position
    SeriesFactorSet wae(void) const throw(PositionFactorsException);

  private:
    const Position& _pos;
    const Series::EODSeries& _db;

    // Must index SeriesFactor by date to speed up excursion calculations
    struct SeriesFactorToTmCmp: public std::binary_function<SeriesFactor, SeriesFactor, bool>
    {
      bool operator()(const SeriesFactor& sf1, const SeriesFactor& sf2) const { return sf1.to_tm() < sf2.to_tm(); }
    };

    struct SeriesFactorFromTmCmp: public std::binary_function<SeriesFactor, SeriesFactor, bool>
    {
      bool operator()(const SeriesFactor& sf1, const SeriesFactor& sf2) const { return sf1.from_tm() < sf2.from_tm(); }
    };

    typedef std::set<SeriesFactor, SeriesFactorToTmCmp>   SF_TOTM;
    typedef std::set<SeriesFactor, SeriesFactorFromTmCmp> SF_FROMTM;

    SF_TOTM   _sf_totm;
    SF_FROMTM _sf_fromtm;    

private:
  struct PeriodFactor
  {
    PeriodFactor(void);
    bool isValid(void) const;

    double factor;
    boost::posix_time::ptime from_tm;
    boost::posix_time::ptime to_tm;
  };

  PeriodFactor _wae(SF_TOTM::const_iterator& start) const;
  PeriodFactor _bfe(SF_TOTM::const_iterator& start) const;
};


#endif // _POSITIONFACTORS_HPP_
