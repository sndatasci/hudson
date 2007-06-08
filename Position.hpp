/*
 * Position.hpp
 */

#ifndef _POSITION_HPP_
#define _POSITION_HPP_

#ifdef WIN32
#pragma warning (disable:4290)
#endif

// C++
#include <stdexcept>

// STL
#include <string>

// Boost
#include <boost/date_time/gregorian/gregorian.hpp>

#include "ExecutionSet.hpp"


class PositionException: public std::exception
{
public:
  PositionException(const std::string& what):
	_what(what)
  {
  }

  virtual ~PositionException(void) throw() { }
  virtual const char* what(void) const throw() { return _what.c_str(); }

protected:
  std::string _what;
};


class Position
{
public:
  typedef unsigned long ID;

  enum Type {
	LONG,
	SHORT
  };

public:
  bool operator<(const Position& p) const { return _id < p._id; }
  bool operator==(const Position& p) const { return _id == p._id; }

  ID id(void) const { return _id; }
  std::string symbol(void) const { return _symbol; }
  int size(void) const { return _size; }

  bool open(void) const { return _size != 0; }
  bool closed(void) const { return !open(); }
  void print(void) const;

  const Execution& first_exec(void) const { return _sExecutions.last_by_date(); }
  const Execution& last_exec(void) const { return _sExecutions.first_by_date(); }

  virtual void buy(const boost::gregorian::date& dt, double price, unsigned size) throw(PositionException) = 0;
  virtual void sell(const boost::gregorian::date& dt, double price, unsigned size) throw(PositionException) = 0;
  virtual void sell_short(const boost::gregorian::date& dt, double price, unsigned size) throw(PositionException) = 0;
  virtual void cover(const boost::gregorian::date& dt, double price, unsigned size) throw(PositionException) = 0;
  virtual void close(const boost::gregorian::date& dt, double price) throw(PositionException) = 0;
  virtual double factor(void) const { return 1; }
  virtual Type type(void) const = 0;
  virtual std::string type_str(void) const = 0;

protected:
  Position(ID id, const std::string& symbol);

  ID _id;
  std::string _symbol;
  unsigned _size;

  ExecutionSet _sExecutions;
};

#endif // _POSITION_HPP_
