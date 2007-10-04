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
  PositionFactorsSet(const PositionSet& sPositions, const Series::EODSeries& db);

  unsigned num(void) const { return _sPositions.size(); }

  double avg_neg_excursion(void) const; // average negative position excursion
  double avg_pos_excursion(void) const; // average positive position excursion

  SeriesFactorSet max_cons_pos(void) const throw(PositionFactorsSetException); // max consecutive positive series factors
  SeriesFactorSet max_cons_neg(void) const throw(PositionFactorsSetException); // max consecutive negative series factors

  SeriesFactorSet worst_excursion(void) const throw(PositionFactorsSetException); // worst position excursion
  SeriesFactorSet best_excursion(void) const throw(PositionFactorsSetException); // best position excursion

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
