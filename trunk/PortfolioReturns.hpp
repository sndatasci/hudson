/*
* PortfolioReturns.hpp
*/

#ifndef _PORTFOLIORETURNS_HPP_
#define _PORTFOLIORETURNS_HPP_

#ifdef WIN32
#pragma warning (disable:4290)
#endif

// STL
#include <vector>

// Hudson
#include "EOMReturnFactors.hpp"


class PortfolioReturns
{
public:
  PortfolioReturns(void);
  
  double roi(void) const;
  double cagr(void) const;
  double gsd(void) const;
  
  void add(const EOMReturnFactors* rf);
  unsigned series(void) const { return (unsigned)_vRF.size(); }
  
public:
  std::vector<const EOMReturnFactors*> _vRF;
};

#endif // _PORTFOLIORETURNS_HPP_
