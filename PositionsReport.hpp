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
