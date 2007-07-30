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
#include "DaySeries.hpp"
#include "DayPrice.hpp"
#include "SeriesFactorSet.hpp"


class PositionFactorsSet
{
public:
  PositionFactorsSet(const PositionSet& sPositions, const Series::DaySeries& db);

  double avg_neg_excursion(void) const; // average negative position excursion
  double avg_pos_excursion(void) const; // average positive position excursion

  SeriesFactorSet max_cons_pos(void) const; // max consecutive positive series factors
  SeriesFactorSet max_cons_neg(void) const; // max consecutive negative series factors

  SeriesFactorSet worst_excursion(void) const; // worst position excursion
  SeriesFactorSet best_excursion(void) const; // best position excursion

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
  const Series::DaySeries& _db;
};

#endif // _POSITIONSFACTORS_HPP_
