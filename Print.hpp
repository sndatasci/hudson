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

#include <iostream>

template <class T>
inline void PRINT_ELEMENTS(const T& coll, const char* optstr = "")
{
  typename T::const_iterator iter;

  std::cout << optstr;
  for( iter = coll.begin(); iter != coll.end(); ++iter )
    std::cout << *iter << ' ';

  std::cout << std::endl;
}
