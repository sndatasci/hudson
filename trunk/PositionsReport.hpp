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

  void favorable(void) const;
  void adverse(void) const;

  void print(void) const;

private:
  const PositionFactorsSet& _pf;
};

#endif // _POSITIONSREPORT_HPP_
