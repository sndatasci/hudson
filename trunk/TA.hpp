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

// Hudson
#include "DayPrice.hpp"
#include "DaySeries.hpp"


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
  enum SERIES_TYPE
  {
    OPEN,
    CLOSE,
    HIGH,
    LOW,
    VOLUME
  };

  typedef std::vector<double> vDouble;
  typedef Series::DaySeries DB;

public:
  TA(const DB& db) throw(TAException);
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

  SMARes    SMA(SERIES_TYPE st, DB::const_iterator& iter, unsigned ma_period) const throw(TAException);
  EMARes    EMA(SERIES_TYPE st, DB::const_iterator& iter, unsigned ma_period) const throw(TAException);
  RSIRes    RSI(SERIES_TYPE st, DB::const_iterator& iter, unsigned rsi_period) const throw(TAException);
  ROCRes    ROC(SERIES_TYPE st, DB::const_iterator& iter, unsigned roc_period) const throw(TAException);
  ROCRRes   ROCR(SERIES_TYPE st, DB::const_iterator& iter, unsigned roc_period) const throw(TAException);
  ROCPRes   ROCP( SERIES_TYPE st, DB::const_iterator& iter, unsigned rocp_period ) const throw(TAException);
  STDDEVRes STDDEV(SERIES_TYPE st, DB::const_iterator& iter, unsigned stddev_period, double sd = 1) const throw(TAException);
  BBRes     BBANDS(SERIES_TYPE st, DB::const_iterator& iter, unsigned ma_period, double sd_up, double sd_down) const throw(TAException);

protected:
  vDouble getSeries(SERIES_TYPE st, DB::const_iterator& iter, unsigned len) const throw(TAException);
  std::string getError(TA_RetCode code) const;

protected:
  const DB& _db;
};

#endif // _TA_HPP_
