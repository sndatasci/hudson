/*
 * ReturnFactors.hpp
 */

#ifndef _RETURNFACTORS_HPP_
#define _RETURNFACTORS_HPP_

// CSTD
#include <cmath>

// STL
#include <vector>
#include <numeric>
#include <functional>

class ReturnFactors
{
public:
  typedef std::vector<double> vdouble;

  ReturnFactors(const vdouble& vf, unsigned days, unsigned yf);
  ~ReturnFactors(void);

  unsigned days(void) const { return _days; }
  unsigned yperiods(void) const { return _yf; }

  double roi(void) const;
  double avg(void) const;
  double dd(void) const;
  double stddev(void) const;
  double skew(void) const;
  double cagr(void) const;
  double gsd(void) const;
  double best(void) const;
  double worst(void) const;
  size_t num(void) const;
  unsigned max_cons_pos(void) const; // max consecutive positive
  unsigned max_cons_neg(void) const; // max consecutive negative

  vdouble pos(void) const;
  vdouble neg(void) const;

private:
  struct variance1 : public std::binary_function<double, double, double> {
    variance1(double mean): _mean(mean) { }

    // accumulate() doesn't accumulate when using a custom binary_function...
    double operator()(double x, double y) { return x + ::pow(y - _mean, 2); }

  private:
    double _mean;
  };

  struct skew1: public std::binary_function<double, double, double> {
    skew1(double mean): _mean(mean) { }

    double operator()(double x, double y) { return x + ::pow(y - _mean, 3); }
  private:
    double _mean;
  };

  struct log10_1: public std::unary_function<double, double> {
    double operator()(double x) { return ::log10(x); }
  };

  double _dd(int i) const;

private:
  vdouble _vf;					// factors
  vdouble _vlf;					// log factors
  double* _f;					// C-Style array for GSL

  unsigned _days;				// time in days
  unsigned _yf;					// yearly factors (12 for monthly)
  double _years;				// time in years (fractionable)
  double _fv;					// future value
  double _mean;					// factors average
  double _stddev;				// factors standard deviation
};

#endif // _RETURNFACTORS_HPP_

