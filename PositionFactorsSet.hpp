/*
* PositionFactorsSet.hpp
*/

#ifndef _POSITIONFACTORSSET_HPP_
#define _POSITIONFACTORSSET_HPP_

#ifdef WIN32
#pragma warning (disable:4290)
#endif

// STL
#include <vector>
#include <numeric>
#include <functional>

// Hudson
#include "PositionSet.hpp"
#include "EODSeries.hpp"
#include "SeriesFactorSet.hpp"


class PositionFactorsSetException: public std::exception
{
public:
  PositionFactorsSetException(const std::string& msg):
    _Str("PositionFactorsSetException: ")
  {
    _Str += msg;
  }

  virtual ~PositionFactorsSetException(void) throw() { }
  virtual const char *what(void) const throw() { return _Str.c_str(); }

protected:
  std::string _Str;
};


class PositionFactorsSet
{
public:
  struct ExcursionResults 
  {
    ExcursionResults(void);

    double avg;
    SeriesFactorSet high;
    SeriesFactorSet consecutive;
  };

public:
  PositionFactorsSet(const PositionSet& sPositions, const Series::EODSeries& db);

  unsigned num(void) const { return (unsigned)_sPositions.size(); }

  ExcursionResults favorable(void) const throw(PositionFactorsSetException);
  ExcursionResults adverse(void) const throw(PositionFactorsSetException);

private:
  struct SeriesFactorSetSizeLtCmp: public std::binary_function<SeriesFactorSet, SeriesFactorSet, bool>
  {
    bool operator()(const SeriesFactorSet& sfs1, const SeriesFactorSet& sfs2) const { return sfs1.size() < sfs2.size(); }
  };

  struct SeriesFactorSetFactorLtCmp: public std::binary_function<SeriesFactorSet, SeriesFactorSet, bool>
  {
    bool operator()(const SeriesFactorSet& sfs1, const SeriesFactorSet& sfs2) const { return sfs1.factor() < sfs2.factor(); }
  };

private:
  const PositionSet _sPositions;
  const Series::EODSeries& _db;
};

#endif // _POSITIONSFACTORS_HPP_
