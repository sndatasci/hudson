/*
* PortfolioReport.hpp
*/

#ifndef _PORTFOLIOREPORT_HPP_
#define _PORTFOLIOREPORT_HPP_

#ifdef WIN32
#pragma warning (disable:4290)
#endif

// STL
#include <string>

// Hudson
#include "PortfolioReturns.hpp"


class PortfolioReport
{
public:
  PortfolioReport(const PortfolioReturns& pr);
  
  void print(void);
  
  void roi(void)  const { std::cout << "ROI: " << _pr.roi()*100 << '%' << std::endl; }
  void cagr(void) const { std::cout << "CAGR: " << _pr.cagr()*100 << '%' << std::endl; }
  void gsd(void)  const { std::cout << "GSDm: " << _pr.gsd()*100 << '%' << std::endl; }
  
protected:
  const PortfolioReturns& _pr;
};

#endif // _PORTFOLIOREPORT_HPP_
