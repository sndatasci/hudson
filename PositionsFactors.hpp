/*
* PositionsFactors.hpp
*/

#ifndef _POSITIONSFACTORS_HPP_
#define _POSITIONSFACTORS_HPP_

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


class PositionsFactors
{
public:
  PositionsFactors(const PositionSet& sPositions, const Series::DaySeries<Series::DayPrice>& db);

  double avg_neg_excursion(void) const; // average negative position excursion
  double avg_pos_excursion(void) const; // average positive position excursion
  double worst_excursion(void) const; // worst position excursion
  double best_excursion(void) const; // best position excursion

private:
  const PositionSet _sPositions;
  const Series::DaySeries<Series::DayPrice>& _db;
};

#endif // _POSITIONSFACTORS_HPP_
