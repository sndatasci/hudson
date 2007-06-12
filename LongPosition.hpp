/*
 * LongPosition.hpp
 */

#ifndef _LONGPOSITION_HPP_
#define _LONGPOSITION_HPP_

#ifdef WIN32
#pragma warning (disable:4290)
#endif

// Hudson
#include "Position.hpp"


class LongPosition: public Position
{
public:
  LongPosition(ID id, const std::string& symbol, const boost::gregorian::date& dt, const Price& price, unsigned size) throw(PositionException);

  virtual Type type(void) const { return LONG; }
  virtual std::string type_str(void) const { return "Long"; }

  unsigned buys(void) const { return _buys; }
  unsigned sells(void) const { return _sells; }

  virtual double avgEntryPrice(void) const { return _avgBuyPrice; }
  virtual double avgExitPrice(void) const { return _avgSellPrice; }
  virtual double factor(void) const { return _avgSellPrice / _avgBuyPrice; }

  virtual void buy(const boost::gregorian::date& dt, const Price& price, unsigned size) throw(PositionException);
  virtual void sell(const boost::gregorian::date& dt, const Price& price, unsigned size) throw(PositionException);
  virtual void sell_short(const boost::gregorian::date& dt, const Price& price, unsigned size) throw(PositionException);
  virtual void cover(const boost::gregorian::date& dt, const Price& price, unsigned size) throw(PositionException);
  virtual void close(const boost::gregorian::date& dt, const Price& price) throw(PositionException);


private:
  unsigned _buys;
  unsigned _sells;

  double _avgBuyPrice;
  double _avgSellPrice;
};

#endif // _LONGPOSITION_HPP_
