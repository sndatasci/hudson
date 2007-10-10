/*
* SeriesFactorSet.hpp
*/

#ifndef _SERIESFACTORSET_HPP_
#define _SERIESFACTORSET_HPP_

#ifdef WIN32
#pragma warning (disable:4290)
#endif

// STL
#include <stdexcept>
#include <string>

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


class SeriesFactorSetException: public std::exception
{
public:
  SeriesFactorSetException(const std::string& msg):
    _Str("SeriesFactorSetException: ")
  {
    _Str += msg;
  }

  virtual ~SeriesFactorSetException(void) throw() { }
  virtual const char *what() const throw() { return _Str.c_str(); }

protected:
  std::string _Str;
};


class SeriesFactorSet: private __SeriesFactorSet
{
public:
  // Export keys and iteration stuff
  typedef __SeriesFactorSet::iterator iterator;
  typedef __SeriesFactorSet::const_iterator const_iterator;
  typedef __SeriesFactorSet::index<from_key>::type from_key;
  typedef __SeriesFactorSet::index<to_key>::type to_key;

  using __SeriesFactorSet::get;
  using __SeriesFactorSet::size;
  using __SeriesFactorSet::empty;
  using __SeriesFactorSet::begin;
  using __SeriesFactorSet::end;

public:
  SeriesFactorSet(void);

  // Override insert() to cache factor value recalculation
  bool insert(const SeriesFactor& sf);

  double factor(void) const throw(SeriesFactorSetException);

private:
  double _factor;
};


typedef SeriesFactorSet::from_key series_factor_by_begin_mark;
typedef SeriesFactorSet::to_key series_factor_by_end_mark;

#endif // _SERIESFACTORSET_HPP_
