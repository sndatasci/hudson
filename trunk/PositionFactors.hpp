/*
 * PositionFactors.hpp
 */

#ifndef _POSITIONFACTORS_HPP_
#define _POSITIONFACTORS_HPP_

// Posix
#include <cmath>

// STL
#include <vector>
#include <numeric>

#include "PositionSet.hpp"


class PositionFactors
{
public:
  PositionFactors(const PositionSet& ps);				  
  ~PositionFactors(void);

  double roi(void) const;
  double avg(void) const;
  double stddev(void) const;
  /*
  double skew(void) const;
  unsigned num(void) const;

  const Position& best(void) const;
  const Position& worst(void) const;

  const PositionSet max_cons_pos(void) const; // max consecutive positive
  const PositionSet max_cons_neg(void) const; // max consecutive negative
  const PositionSet pos(void) const;
  const PositionSet neg(void) const;
  const PositionSet dd(void) const;
  */

private:
  struct variance1 : public std::binary_function<double, const Position*, double> {
	variance1(double mean): _mean(mean) { }

	double operator()(double x, const Position* p) { return x + ::pow(p->factor() - _mean, 2); }

  private:
	double _mean;
  };


  struct skew1: public std::binary_function<double, const Position*, double> {
	skew1(double mean): _mean(mean) { }

	double operator()(double x, const Position* p) { return x + ::pow(p->factor() - _mean, 3); }

  private:
	double _mean;
  };


  struct acc1: public std::binary_function<double, const Position*, double> {
	double operator()(double x, const Position* p) const { return x + p->factor(); }

  private:
	double _acc;
  };

  //double _dd(int i) const;

private:
  const PositionSet _ps;

  double _fv;					// future value
  double _mean;					// factors average
  double _stddev;				// factors standard deviation
};

#endif // _POSITIONFACTORS_HPP_
