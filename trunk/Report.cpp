/*
 * Report.cpp
 */

// STL
#include <iostream>

#include "Report.hpp"

using namespace std;


Report::Report(const ReturnFactors& rf):
  _rf(rf),
  _rf_pos(rf.pos(), rf.days(), rf.yperiods()),
  _rf_neg(rf.neg(), rf.days(), rf.yperiods()),
  _pos_percent(0),
  _neg_percent(0)
{
  if( _rf.num() == 0 ) return;
  if( _rf_pos.num() ) _pos_percent = _rf_pos.num() / (double)rf.num();
  if( _rf_neg.num() ) _neg_percent = _rf_neg.num() / (double)rf.num();
}


void Report::print(void) const
{
  streamsize curr_precision = cout.precision();
  ios_base::fmtflags curr_flags = cout.flags();

  cout.precision(2);
  cout.setf(ios::fixed);

  trades();
  avg_trade();
  std_dev();
  skew();
  std_dev2_range();
  std_dev3_range();
  pos_trades();
  neg_trades();
  avg_pos();
  avg_neg();
  best();
  worst();
  max_cons_pos();
  max_cons_neg();
  max_dd();

  cout << endl;
  roi();
  cagr();
  gsdm();

  cout.precision(curr_precision);
  cout.flags(curr_flags);
}
