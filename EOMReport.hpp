/*
* EOMReport.hpp
*/

#ifndef _EOMREPORT_HPP_
#define _EOMREPORT_HPP_

#ifdef WIN32
#pragma warning (disable:4290)
#endif

// STL
#include <iostream>

// Hudson
#include "Report.hpp"
#include "EOMReturnFactors.hpp"


class EOMReport: public Report
{
public:
  EOMReport(const EOMReturnFactors& rf);

  void gsdm(void) const { std::cout << "GSDm: " << _eomrf.gsd()*100 << '%' << std::endl; }

  void print(void) const;

private:
  const EOMReturnFactors& _eomrf;
};


#endif // _EOMREPORT_HPP_
