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
  TAException(const std::string& msg):
    _Str("TAException: ")
  {
    _Str += msg;
  }

  virtual ~TAException(void) throw() { }
  virtual const char *what(void) const throw() { return _Str.c_str(); }

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

  typedef struct  
  {
    vDouble ma;
    int begIdx;
    int nbElement;
  } SMARes;
  
  typedef struct
  {
    vDouble ema;
    int begIdx;
    int nbElement;
  } EMARes;

  typedef struct  
  {
    vDouble rsi;
    int begIdx;
    int nbElement;
  } RSIRes;

  typedef struct  
  {
    vDouble roc;
    int begIdx;
    int nbElement;
  } ROCRes;

  typedef struct 
  {
    vDouble rocr;
    int begIdx;
    int nbElement;
  } ROCRRes;

  typedef struct  
  {
    vDouble rocp;
    int begIdx;
    int nbElement;
  } ROCPRes;

  typedef struct
  {
    vDouble stddev;
    int begIdx;
    int nbElement;
  } STDDEVRes;

  typedef struct
  {
    vDouble factors;
    int begIdx;
    int nbElement;
  } FACTORRes;

  typedef struct
  {
    vDouble upper_band;
    vDouble middle_band;
    vDouble lower_band;
    int begIdx;
    int nbElement;
  } BBRes;

  typedef struct 
  {
    vDouble macd;
    vDouble macd_signal;
    vDouble macd_hist;
    int begIdx;
    int nbElement;
  } MACDRes;

  SMARes    SMA(vDouble vSeries, unsigned ma_period) const throw(TAException);
  EMARes    EMA(vDouble vSeries, unsigned ema_period) const throw(TAException);
  RSIRes    RSI(vDouble vSeries, unsigned rsi_period) const throw(TAException);
  MACDRes   MACD(vDouble vSeries, unsigned macd_fast_period, unsigned macd_slow_period, unsigned macd_signal_period) throw(TAException);
  ROCRes    ROC(vDouble vSeries, unsigned roc_period) const throw(TAException);
  ROCRRes   ROCR(vDouble vSeries, unsigned roc_period) const throw(TAException);
  ROCPRes   ROCP(vDouble vSeries, unsigned rocp_period) const throw(TAException);
  STDDEVRes STDDEV(vDouble vSeries, unsigned stddev_period, double sd) const throw(TAException);
  BBRes     BBANDS(vDouble vSeries, unsigned ma_period, double sd_up, double sd_down) const throw(TAException);
  FACTORRes FACTORS(vDouble vSeries, unsigned period) const throw(TAException);

protected:
  std::string getError(TA_RetCode code) const;
};

#endif // _TA_HPP_
