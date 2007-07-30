/*
* PositionsReport.cpp
*/

#include <StdAfx.h>

// STL
#include <iostream>

// Hudson
#include "PositionsReport.hpp"
#include "SeriesFactorSet.hpp"
#include "SeriesFactor.hpp"

using namespace std;


PositionsReport::PositionsReport( const PositionFactorsSet& pf ):
  _pf(pf)
{

}


void PositionsReport::print( void ) const
{
  streamsize curr_precision = cout.precision();
  ios_base::fmtflags curr_flags = cout.flags();

  cout.precision(2);
  cout.setf(ios::fixed);

  avg_neg_excursion();
  avg_pos_excursion();
  max_cons_pos();
  max_cons_neg();
  worst_excursion();
  best_excursion();

  cout.precision(curr_precision);
  cout.flags(curr_flags);
}


void PositionsReport::max_cons_pos( void ) const
{
  const SeriesFactorSet sfs = _pf.max_cons_pos();
  cout << "Max cons pos: ";
  if( sfs.empty() ) {
    cout << 0 << endl;
    return;
  }

  const SeriesFactor& sf_begin = *(sfs.get<to_key>().begin());
  const SeriesFactor& sf_end   = *(sfs.get<to_key>().rbegin());

  cout << (int)sfs.size() << " [" << sf_begin.from_tm() << '/' << sf_end.to_tm() << ']' << endl;
}


void PositionsReport::max_cons_neg( void ) const
{
  const SeriesFactorSet sfs = _pf.max_cons_neg();
  cout << "Max cons neg: ";
  if( sfs.empty() ) {
    cout << 0 << endl;
    return;
  }

  const SeriesFactor& sf_begin = *(sfs.get<to_key>().begin());
  const SeriesFactor& sf_end   = *(sfs.get<to_key>().rbegin());

  cout << (int)sfs.size() << " [" << sf_begin.from_tm() << '/' << sf_end.to_tm() << ']' << endl;
}


void PositionsReport::best_excursion( void ) const
{
  const SeriesFactorSet& sfs = _pf.best_excursion();
  const SeriesFactor& sf_begin = *(sfs.get<from_key>().begin());
  const SeriesFactor& sf_end = *(sfs.get<to_key>().rbegin());

  std::cout << "Best excursion: " << (sfs.factor()-1)*100 << '%';
  std::cout << " [" << sf_begin.from_tm() << '/' << sf_end.to_tm() << ']' << std::endl;
}


void PositionsReport::worst_excursion( void ) const
{
  const SeriesFactorSet& sfs = _pf.worst_excursion();
  const SeriesFactor& sf_begin = *(sfs.get<from_key>().begin());
  const SeriesFactor& sf_end = *(sfs.get<to_key>().rbegin());

  std::cout << "Worst excursion: " << (sfs.factor()-1)*100 << '%';
  std::cout << " [" << sf_begin.from_tm() << '/' << sf_end.to_tm() << ']' << std::endl;
}
