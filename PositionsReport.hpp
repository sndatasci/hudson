/*
* PositionsReport.hpp
*/

#ifndef _POSITIONSREPORT_HPP_
#define _POSITIONSREPORT_HPP_

#ifdef WIN32
#pragma warning (disable:4290)
#endif

// C++
#include <iostream>

// STL
#include <string>

// Hudson
#include "PositionFactorsSet.hpp"
#include "SeriesFactor.hpp"


class PositionsReport
{
public:
  PositionsReport(const PositionFactorsSet& pf);

  void avg_neg_excursion(void) const { std::cout << "Avg neg: " << _pf.avg_neg_excursion()*100 << '%' << std::endl; }
  void avg_pos_excursion(void) const { std::cout << "Avg pos: " << _pf.avg_pos_excursion()*100 << '%' << std::endl; }

  void max_cons_pos(void) const;
  void max_cons_neg(void) const;

  void best_excursion(void) const;
  void worst_excursion(void) const;

  void print(void) const;

private:
  const PositionFactorsSet& _pf;
};

#endif // _POSITIONSREPORT_HPP_
