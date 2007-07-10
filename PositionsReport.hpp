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
#include "PositionsFactors.hpp"


class PositionsReport
{
public:
  PositionsReport(const PositionsFactors& pf);

  void avg_neg_excursion(void) const { std::cout << "Avg neg: " << _pf.avg_neg_excursion()*100 << '%' << std::endl; }
  void avg_pos_excursion(void) const { std::cout << "Avg pos: " << _pf.avg_pos_excursion()*100 << '%' << std::endl; }
  void worst_excursion(void)   const { std::cout << "Worst: "   << _pf.worst_excursion()*100   << '%' << std::endl; }
  void best_excursion(void)    const { std::cout << "Best: "    << _pf.best_excursion()*100    << '%' << std::endl; }

  void print(void) const;

private:
  const PositionsFactors& _pf;
};

#endif // _POSITIONSREPORT_HPP_
