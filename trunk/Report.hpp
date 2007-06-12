/*
 * Report.hpp
 */

#ifndef _REPORT_HPP_
#define _REPORT_HPP_

#ifdef WIN32
#pragma warning (disable:4290)
#endif

// C++
#include <iostream>

// STL
#include <string>

// Hudson
#include "ReturnFactors.hpp"
#include "Position.hpp"


class Report
{
public:
  Report(const ReturnFactors& rf);

  void trades(void) const { std::cout <<  "Trades: " << _rf.num() << std::endl; }
  void avg_trade(void) const { std::cout << "Avg trade: " << _rf.avg()*100 << '%' << std::endl; }
  void std_dev(void) const { std::cout << "Std dev: " <<  _rf.stddev()*100 << '%' << std::endl; }
  void skew(void) const { std::cout << "Skew: " << _rf.skew()*100 << std::endl; }
  void std_dev2_range(void) const { double stddev2 = _rf.stddev()*2; std::cout << "2SD Range: " << (_rf.avg() - stddev2)*100 << '%' << " | " << (_rf.avg() + stddev2)*100 << '%' << std::endl; }
  void std_dev3_range(void) const { double stddev3 = _rf.stddev()*3; std::cout << "3SD Range: " << (_rf.avg() - stddev3)*100 << '%' << " | " << (_rf.avg() + stddev3)*100 << '%' << std::endl; }
  void pos_trades(void) const { std::cout << "Pos trades: " << _rf_pos.num() << " (" << _pos_percent*100 << "%)" << std::endl; }
  void neg_trades(void) const { std::cout << "Neg trades: " << _rf_neg.num() << " (" << _neg_percent*100 << "%)" << std::endl; }
  void avg_pos(void) const { std::cout << "Avg pos: " << _rf_pos.avg()*100 << '%' << std::endl; }
  void avg_neg(void) const { std::cout << "Avg neg: " << _rf_neg.avg()*100 << '%' << std::endl; }
  void best(void) const { const Position& pos = _rf.best(); std::cout << "Best: " << (pos.factor()-1)*100 << '%' << " (" << pos.first_exec().dt() << '/' << pos.last_exec().dt() << ')' << std::endl; }
  void worst(void) const { const Position& pos = _rf.worst(); std::cout << "Worst: " << (pos.factor()-1)*100 << '%' << " (" << pos.first_exec().dt() << '/' << pos.last_exec().dt() << ')' << std::endl; }
  void max_cons_pos(void) const { std::cout << "Max cons pos: " << _rf.max_cons_pos() << std::endl; }
  void max_cons_neg(void) const { std::cout << "Max cons neg: " << _rf.max_cons_neg() << std::endl; }
  void max_dd(void) const { std::cout << "Max Drawdown: " << -(1-_rf.dd())*100 << '%' << std::endl; }
  void roi(void) const { std::cout << "ROI: " << _rf.roi()*100 << '%' << std::endl; }
  void cagr(void) const { std::cout << "CAGR: " << _rf.cagr()*100 << '%' << std::endl; }
  void gsdm(void) const { std::cout << "GSDm: " << _rf.gsd()*100 << '%' << std::endl; }

  void print(void) const;

private:
  const ReturnFactors& _rf;
  const ReturnFactors _rf_pos;
  const ReturnFactors _rf_neg;

  double _pos_percent;
  double _neg_percent;
};

#endif // _REPORT_HPP_
