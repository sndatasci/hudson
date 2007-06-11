/*
 * ExecutionSet.hpp
 */

#ifndef _EXECUTIONSET_HPP_
#define _EXECUTIONSET_HPP_

// Boost
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/tag.hpp>

#include "Execution.hpp"


struct date_key { };
struct side_key { };

typedef boost::multi_index::multi_index_container<
  Execution*,
  boost::multi_index::indexed_by<
    boost::multi_index::ordered_unique<boost::multi_index::identity<Execution> >,
    boost::multi_index::ordered_non_unique<boost::multi_index::tag<date_key>, boost::multi_index::const_mem_fun<Execution, boost::gregorian::date, &Execution::dt> >,
    boost::multi_index::ordered_non_unique<boost::multi_index::tag<side_key>, boost::multi_index::const_mem_fun<Execution, Execution::Side, &Execution::side> >
  >
> __ExecutionSet;


class ExecutionSet
{
public:
  ExecutionSet(void);
  ~ExecutionSet(void);

  bool buy(boost::gregorian::date dt, const Price& price, unsigned size);
  bool sell(boost::gregorian::date dt, const Price& price, unsigned size);
  bool sell_short(boost::gregorian::date dt, const Price& price, unsigned size);
  bool cover(boost::gregorian::date dt, const Price& price, unsigned size);

  const Execution& first_by_date(void) const { return **(_es.get<date_key>().begin()); }
  const Execution& last_by_date(void) const { return **(_es.get<date_key>().rbegin()); }

  void print(void) const;

private:
  Execution::ID _eid;

  __ExecutionSet _es;

private:
  typedef __ExecutionSet::index<side_key>::type executions_by_side;
  typedef __ExecutionSet::index<date_key>::type executions_by_date;  
};

#endif // _EXECUTIONSET_HPP_
