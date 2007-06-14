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


struct symbol_key { };
struct type_key   { };
struct factor_key { };
struct last_exec_key { };

typedef boost::shared_ptr<Position> PositionPtr;

typedef boost::multi_index::multi_index_container<

  PositionPtr,

  boost::multi_index::indexed_by<
    boost::multi_index::ordered_unique<boost::multi_index::identity<Position> >,
    boost::multi_index::ordered_non_unique<boost::multi_index::tag<symbol_key>, boost::multi_index::const_mem_fun<Position, std::string, &Position::symbol> >,
    boost::multi_index::ordered_non_unique<boost::multi_index::tag<type_key>, boost::multi_index::const_mem_fun<Position, Position::Type, &Position::type> >,
    boost::multi_index::ordered_non_unique<boost::multi_index::tag<factor_key>, boost::multi_index::const_mem_fun<Position, double, &Position::factor> >,
    boost::multi_index::ordered_non_unique<boost::multi_index::tag<last_exec_key>, boost::multi_index::const_mem_fun<Position, const Execution&, &Position::last_exec> >
  >
> __PositionSet;


class PositionSet: public __PositionSet
{
public:
  
public:
  const PositionSet closed(void) const;
  const PositionSet open(void) const;

  double realized(void) const;

  void print(void) const;
};


typedef PositionSet::index<symbol_key>::type position_by_symbol;
typedef PositionSet::index<type_key>::type position_by_type;
typedef PositionSet::index<last_exec_key>::type position_by_last_exec;


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
