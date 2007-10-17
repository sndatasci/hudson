/*
 * PositionSet.hpp
 */

#ifndef _POSITIONSET_HPP_
#define _POSITIONSET_HPP_

#ifdef WIN32
#pragma warning (disable:4290)
#endif

// STL
#include <string>

// Boost
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/tag.hpp>
#include <boost/shared_ptr.hpp>

// Hudson
#include "Position.hpp"
#include "Price.hpp"


struct symbol_key { };
struct first_exec_key { };
struct last_exec_key { };

typedef boost::shared_ptr<Position> PositionPtr;

typedef boost::multi_index::multi_index_container<

  PositionPtr,

  boost::multi_index::indexed_by<
    boost::multi_index::ordered_unique<boost::multi_index::identity<Position> >,
    boost::multi_index::ordered_non_unique<boost::multi_index::tag<symbol_key>, boost::multi_index::const_mem_fun<Position, std::string, &Position::symbol> >,
    boost::multi_index::ordered_non_unique<boost::multi_index::tag<first_exec_key>, boost::multi_index::const_mem_fun<Position, const Execution&, &Position::first_exec> >,
    boost::multi_index::ordered_non_unique<boost::multi_index::tag<last_exec_key>, boost::multi_index::const_mem_fun<Position, const Execution&, &Position::last_exec> >
  >
> __PositionSet;


class PositionSet: private __PositionSet
{
public:
  typedef __PositionSet::iterator                     iterator;
  typedef __PositionSet::const_iterator               const_iterator;
  typedef __PositionSet::index<symbol_key>::type      by_symbol;
  typedef __PositionSet::index<first_exec_key>::type  by_first_exec;
  typedef __PositionSet::index<last_exec_key>::type   by_last_exec;

  using __PositionSet::get;
  using __PositionSet::size;
  using __PositionSet::empty;
  using __PositionSet::begin;
  using __PositionSet::end;
  using __PositionSet::insert;
  using __PositionSet::find;
  using __PositionSet::replace;

public:
  const PositionSet closed(void) const;
  const PositionSet open(void) const;

  const PositionSet closed(const std::string& symbol);
  const PositionSet open(const std::string& symbol);

  double realized(void) const;
  double unrealized(void) const;

  void print(Price curr_price) const;
};


struct pos_comp_id
{
  bool operator()(Position::ID id, Position& p) const { return id < p.id(); }
  bool operator()(Position& p, Position::ID id) const { return p.id() < id; }
};


// Look for position by ID:
//
// PositionSet positions;
// ...populate collection...
// Position::ID id = 5;
// PositionSet::iterator iter = positions.find(id, pos_comp_id());

// Look for position by symbol:
//
// typedef PositionSet::index<symbol_key>::type position_set_by_symbol;
// position_set_by_symbol::iterator it = _miPositions.get<symbol_key>().find("SP500");

#endif // _POSITIONSET_HPP_
