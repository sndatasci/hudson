/*
* SeriesFactorSet.hpp
*/

#ifndef _SERIESFACTORSET_HPP_
#define _SERIESFACTORSET_HPP_

#ifdef WIN32
#pragma warning (disable:4290)
#endif

// Boost
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/tag.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

// Hudson
#include "SeriesFactor.hpp"


struct from_key {};
struct to_key {};

typedef boost::multi_index::multi_index_container<

  SeriesFactor,

  boost::multi_index::indexed_by<
    boost::multi_index::ordered_unique<boost::multi_index::identity<SeriesFactor> >,
    boost::multi_index::ordered_non_unique<boost::multi_index::tag<from_key>, boost::multi_index::const_mem_fun<SeriesFactor, const boost::posix_time::ptime&, &SeriesFactor::from_tm> >,
    boost::multi_index::ordered_non_unique<boost::multi_index::tag<to_key>, boost::multi_index::const_mem_fun<SeriesFactor, const boost::posix_time::ptime&, &SeriesFactor::to_tm> >
  >
> __SeriesFactorSet;


class SeriesFactorSet: public __SeriesFactorSet
{
public:
  double factor(void) const;
};


typedef SeriesFactorSet::index<from_key>::type series_factor_by_begin_mark;
typedef SeriesFactorSet::index<to_key>::type series_factor_by_end_mark;

#endif // _SERIESFACTORSET_HPP_
