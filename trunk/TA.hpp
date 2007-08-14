/*
* TA.hpp
*/

#ifndef _TA_HPP_ 
#define _TA_HPP_

#ifdef WIN32
#pragma warning (disable:4290)
#endif

// STL
#include <stdexcept>
#include <string>
#include <vector>

// TA
#include <ta_libc.h>


class TAException: public std::exception
{
  public:
    TAException(const std::string msg):
      _Str("TAException: ")
    {
      _Str += msg;
    }

    virtual ~TAException(void) throw() { }
    virtual const char *what() const throw() { return _Str.c_str(); }

  protected:
    std::string _Str;
};


class TA
{
public:
  typedef std::vector<double> vDouble;

public:
  TA(void) throw(TAException);
  ~TA(void);

  typedef double SMARes;
  typedef double EMARes;
  typedef double RSIRes;
  typedef double ROCRes;
  typedef double ROCRRes;
  typedef double ROCPRes;
  typedef double STDDEVRes;
  typedef struct
  {
    double upper_band;
    double middle_band;
    double lower_band;
  } BBRes;

  SMARes    SMA(vDouble vSeries, unsigned ma_period) const throw(TAException);
  EMARes    EMA(vDouble vSeries, unsigned ma_period) const throw(TAException);
  RSIRes    RSI(vDouble vSeries, unsigned rsi_period) const throw(TAException);
  ROCRes    ROC(vDouble vSeries, unsigned roc_period) const throw(TAException);
  ROCRRes   ROCR(vDouble vSeries, unsigned roc_period) const throw(TAException);
  ROCPRes   ROCP(vDouble vSeries, unsigned rocp_period) const throw(TAException);
  STDDEVRes STDDEV(vDouble vSeries, unsigned stddev_period, double sd = 1) const throw(TAException);
  BBRes     BBANDS(vDouble vSeries, unsigned ma_period, double sd_up, double sd_down) const throw(TAException);

protected:
  std::string getError(TA_RetCode code) const;
};

#endif // _TA_HPP_
