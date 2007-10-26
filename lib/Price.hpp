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

#ifndef _PRICE_HPP_
#define _PRICE_HPP_

#ifdef WIN32
#pragma warning (disable:4290)
#endif

// C++
#include <stdexcept>

// STL
#include <string>


class PriceException: public std::exception
{
public:
  PriceException(const std::string& msg):
    _Str("PriceException: ")
  {
    _Str += msg;
  }

  virtual ~PriceException(void) throw() { }
  virtual const char *what() const throw() { return _Str.c_str(); }

protected:
  std::string _Str;
};


class Price
{
  public:
    //! Initialize Price with value and checks for valid value > 0.
    Price(double value) throw(PriceException);
    Price(const Price& src);

    Price& operator=(const Price& src);

    double value(void) const { return _value; }

  protected:
    double _value;
};


inline double operator-(const Price& first, const Price& second) { return first.value() - second.value(); }
inline double operator+(const Price& first, const Price& second) { return first.value() + second.value(); }
inline double operator*(const Price& first, const Price& second) { return first.value() * second.value(); }
inline double operator/(const Price& first, const Price& second) { return first.value() / second.value(); }

inline double operator-(const Price& first, double second) { return first.value() - second; }
inline double operator+(const Price& first, double second) { return first.value() + second; }
inline double operator*(const Price& first, double second) { return first.value() * second; }
inline double operator/(const Price& first, double second) { return first.value() / second; }

#endif // _PRICE_HPP_
