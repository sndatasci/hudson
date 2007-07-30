/*
 * ShortPosition.hpp
 */

#ifndef _SHORTPOSITION_HPP_
#define _SHORTPOSITION_HPP_

#ifdef WIN32
#pragma warning (disable:4290)
#endif

// Hudson
#include "Position.hpp"


class ShortPosition: public Position
{
public:
  ShortPosition(ID id, const std::string& symbol, const boost::gregorian::date& dt, const Price& price, unsigned size) throw(PositionException);

  virtual Type type(void) const { return SHORT; }
  virtual std::string type_str(void) const { return "Short"; }

  unsigned shorts(void) const { return _shorts; }
  unsigned covers(void) const { return _covers; }

  virtual double avgEntryPrice(void) const { return _avgShortPrice; }
  virtual double avgExitPrice(void) const { return _avgCoverPrice; }

  virtual double factor(void) const throw(PositionException);
  virtual double factor(const Price& price) const throw(PositionException);
  virtual double factor(const Price& prev_price, const Price& curr_price) const;

  virtual void buy(const boost::gregorian::date& dt, const Price& price, unsigned size) throw(PositionException);
  virtual void sell(const boost::gregorian::date& dt, const Price& price, unsigned size) throw(PositionException);
  virtual void sell_short(const boost::gregorian::date& dt, const Price& price, unsigned size) throw(PositionException);
  virtual void cover(const boost::gregorian::date& dt, const Price& price, unsigned size) throw(PositionException);
  virtual void close(const boost::gregorian::date& dt, const Price& price) throw(PositionException);

private:
  unsigned _shorts;
  unsigned _covers;

  double _avgShortPrice;
  double _avgCoverPrice;
};

#endif // _SHORTPOSITION_HPP_
