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

#ifndef _EXECUTIONSET_HPP_
#define _EXECUTIONSET_HPP_

#ifdef WIN32
#pragma warning (disable:4290)
#endif

// Boost
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/tag.hpp>

// Hudson
#include "Execution.hpp"


struct date_key { };
struct side_key { };

typedef boost::shared_ptr<Execution> ExecutionPtr;

typedef boost::multi_index::multi_index_container<

  ExecutionPtr,

  boost::multi_index::indexed_by<
    boost::multi_index::ordered_unique<boost::multi_index::identity<Execution> >,
    boost::multi_index::ordered_non_unique<boost::multi_index::tag<date_key>, boost::multi_index::const_mem_fun<Execution, boost::gregorian::date, &Execution::dt> >,
    boost::multi_index::ordered_non_unique<boost::multi_index::tag<side_key>, boost::multi_index::const_mem_fun<Execution, Execution::Side, &Execution::side> >
  >
> __ExecutionSet;


class ExecutionSet: private __ExecutionSet
{
public:
  using __ExecutionSet::size;
  using __ExecutionSet::empty;

public:
  ExecutionSet(void);
  ~ExecutionSet(void) { }

  bool buy(boost::gregorian::date dt, const Price& price, unsigned size);
  bool sell(boost::gregorian::date dt, const Price& price, unsigned size);
  bool sell_short(boost::gregorian::date dt, const Price& price, unsigned size);
  bool cover(boost::gregorian::date dt, const Price& price, unsigned size);

  const Execution& first_by_date(void) const { return **(get<date_key>().begin()); }
  const Execution& last_by_date(void) const { return **(get<date_key>().rbegin()); }

  void print(void) const;

private:
  Execution::ID _eid;

private:
  typedef __ExecutionSet::index<side_key>::type by_side;
  typedef __ExecutionSet::index<date_key>::type by_date;  
};

#endif // _EXECUTIONSET_HPP_
