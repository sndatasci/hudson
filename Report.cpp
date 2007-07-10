/*
 * Report.cpp
 */

#include "StdAfx.h"

// STL
#include <iostream>

// Hudson
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


void Report::max_cons_pos( void ) const
{
  PositionSet pset = _rf.max_cons_pos();
  cout << "Max cons pos: ";
  if( pset.empty() ) {
    cout << 0 << endl;
    return;
  }

  PositionPtr pFirstPos = *(pset.get<last_exec_key>().begin());
  PositionPtr pLastPos = *(pset.get<last_exec_key>().rbegin());
  cout << (int)pset.size() << " [" << pFirstPos->last_exec().dt() << '/' << pLastPos->last_exec().dt() << ']' << endl;
}


void Report::max_cons_neg( void ) const
{
  PositionSet pset = _rf.max_cons_neg();
  cout << "Max cons neg: ";
  if( pset.empty() ) {
    cout << 0 << endl;
    return;
  }

  PositionPtr pFirstPos = *(pset.get<last_exec_key>().begin());
  PositionPtr pLastPos = *(pset.get<last_exec_key>().rbegin());
  cout << (int)pset.size() << " [" << pFirstPos->last_exec().dt() << '/' << pLastPos->last_exec().dt() << ']' << endl;
}


void Report::max_dd( void ) const
{
  PositionSet pset = _rf.dd();
  cout << "Max drawdown: ";
  if( pset.empty() ) {
    cout << 0 << endl;
    return;
  }

  PositionPtr pFirstPos = *(pset.get<last_exec_key>().begin());
  PositionPtr pLastPos = *(pset.get<last_exec_key>().rbegin());

  boost::gregorian::date_duration dur = pLastPos->last_exec().dt() - pFirstPos->first_exec().dt();

  ReturnFactors ddrf(pset, dur.days(), 12);
  cout << ddrf.roi()*100 << "% [" << pFirstPos->last_exec().dt() << '/' << pLastPos->last_exec().dt() << ']' << endl;
}
