/*
* Copyright (C) 2007, Alberto Giannetti
*
* This file is part of Hudson.
*
* Hudson is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* Hudson is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with Hudson.  If not, see <http://www.gnu.org/licenses/>.
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
