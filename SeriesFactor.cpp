/*
* SeriesFactor.cpp
*/

#include "StdAfx.h"

// Hudson
#include "SeriesFactor.hpp"


SeriesFactor::SeriesFactor(const boost::posix_time::ptime& from, const boost::posix_time::ptime& to, double f):
  _from(from),
  _to(to),
  _f(f)
{
}
