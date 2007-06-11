/*
 * Execution.hpp
 */

#ifndef _EXECUTION_HPP_
#define _EXECUTION_HPP_

// STL
#include <set>

// Boost
#include <boost/date_time/gregorian/gregorian.hpp>

// Hudson
#include "Price.hpp"


class Execution
{
public:
  enum Side {
	  BUY,
	  SELL,
	  SHORT,
	  COVER
  };

  typedef unsigned long ID;

public:
  bool operator<(const Execution& e) const { return _id < e._id; }

  ID id(void) const { return _id; }
  boost::gregorian::date dt(void) const { return _dt; }
  Price price(void) const { return _price; }
  unsigned size(void) const { return _size; }

  void print(void) const;

  virtual std::string action(void) const = 0;
  virtual Side side(void) const = 0;

protected:
  Execution(ID id, const boost::gregorian::date& dt, const Price& price, unsigned size);

  const ID _id;
  const boost::gregorian::date _dt;
  const Price _price;
  const unsigned _size;
};


class BuyExecution: public Execution
{
public:
  BuyExecution(ID id, const boost::gregorian::date& dt, const Price& price, unsigned size);

  virtual std::string action(void) const { return "Bought"; }
  virtual Side side(void) const { return BUY; }
};


class SellExecution: public Execution
{
public:
  SellExecution(ID id, const boost::gregorian::date& dt, const Price& price, unsigned size);

  virtual std::string action(void) const { return "Sold"; }
  virtual Side side(void) const { return SELL; }
};


class SellShortExecution: public Execution
{
public:
  SellShortExecution(ID id, const boost::gregorian::date& dt, const Price& price, unsigned size);

  virtual std::string action(void) const { return "Shorted"; }
  virtual Side side(void) const { return SHORT; }
};


class CoverExecution: public Execution
{
public:
  CoverExecution(ID id, const boost::gregorian::date& dt, const Price& price, unsigned size);

  virtual std::string action(void) const { return "Covered"; }
  virtual Side side(void) const { return COVER; }
};

#endif // _EXECUTION_HPP_
