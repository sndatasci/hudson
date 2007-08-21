/*
* TA.cpp
*/

#include "StdAfx.h"

// STL
#include <vector>

// Hudson
#include "TA.hpp"

using namespace std;


TA::TA(void) throw(TAException)
{
  TA_RetCode ta_ret;

  if( (ta_ret = TA_Initialize()) != TA_SUCCESS )
    throw TAException(getError(ta_ret));
}


TA::~TA( void )
{
  TA_Shutdown();
}


TA::SMARes TA::SMA(vDouble vSeries, unsigned ma_period) const throw(TAException)
{
  if( vSeries.size() < ma_period )
    throw TAException("Not enough data");

  size_t results = vSeries.size() - (ma_period - 1);
  vDouble vOutReal(results);

  int outBegIdx;
  int outNbElement;

  TA_RetCode ta_ret = TA_MA(0, vSeries.size()-1, &vSeries[0], ma_period, TA_MAType_SMA,
                            &outBegIdx, &outNbElement, &vOutReal[0]);
  if( ta_ret != TA_SUCCESS )
    throw TAException(getError(ta_ret));

  if( outNbElement <= 0 )
    throw TAException("No output elements");

  if( outNbElement != results )
    throw TAException("Number of output elements doesn't match expected value");

  return vOutReal;
}


TA::EMARes TA::EMA( vDouble vSeries, unsigned ma_period ) const throw(TAException)
{
  if( vSeries.size() < ma_period )
    throw TAException("Not enough data");

  size_t results = vSeries.size() - (ma_period - 1);
  vDouble vOutReal(results);

  int outBegIdx;
  int outNbElement;

  TA_RetCode ta_ret = TA_MA(0, vSeries.size()-1, &vSeries[0], ma_period, TA_MAType_EMA,
                            &outBegIdx, &outNbElement, &vOutReal[0]);
  if( ta_ret != TA_SUCCESS )
    throw TAException(getError(ta_ret));

  if( outNbElement <= 0 )
    throw TAException("No out elements");

  if( outNbElement != results )
    throw TAException("Number of output elements doesn't match expected value");

  return vOutReal;
}


TA::RSIRes TA::RSI( vDouble vSeries, unsigned rsi_period ) const throw(TAException)
{
  if( vSeries.size() < rsi_period )
    throw TAException("Not enough data");

  size_t results = vSeries.size() - rsi_period; // TA_RSI() "eats" the first bar in series
  vDouble vOutReal(results);

  int outBegIdx;
  int outNbElement;

  TA_RetCode ta_ret = TA_RSI(0, vSeries.size()-1, &vSeries[0], rsi_period,
                             &outBegIdx, &outNbElement, &vOutReal[0]);
  if( ta_ret != TA_SUCCESS )
    throw TAException(getError(ta_ret));

  if( outNbElement <= 0 )
    throw TAException("No out elements");

  if( outNbElement != results )
    throw TAException("Number of output elements doesn't match expected value");

  return vOutReal;
}


TA::ROCRes TA::ROC(vDouble vSeries, unsigned roc_period) const throw(TAException)
{
  if( vSeries.size() < roc_period )
    throw TAException("Not enough data");

  size_t results = vSeries.size() - (roc_period - 1);
  vDouble vOutReal(results);

  int outBegIdx;
  int outNbElement;

  TA_RetCode ta_ret = TA_ROC(0, roc_period-1, &vSeries[0], roc_period,
                             &outBegIdx, &outNbElement, &vOutReal[0]);
  if( ta_ret != TA_SUCCESS )
    throw TAException(getError(ta_ret));

  if( outNbElement <= 0 )
    throw TAException("No out elements");

  if( outNbElement != results )
    throw TAException("Number of output elements doesn't match expected value");

  return vOutReal;
}


TA::ROCRRes TA::ROCR(vDouble vSeries, unsigned rocr_period ) const throw(TAException)
{
  if( vSeries.size() < rocr_period )
    throw TAException("Not enough data");

  size_t results = vSeries.size() - (rocr_period - 1);
  vDouble vOutReal(results);

  int outBegIdx;
  int outNbElement;

  TA_RetCode ta_ret = TA_ROCR(0, vSeries.size()-1, &vSeries[0], rocr_period,
                              &outBegIdx, &outNbElement, &vOutReal[0]);
  if( ta_ret != TA_SUCCESS )
    throw TAException(getError(ta_ret));

  if( outNbElement <= 0 )
    throw TAException("No out elements");

  if( outNbElement != results )
    throw TAException("Number of output elements doesn't match expected value");

  return vOutReal;
}


TA::ROCPRes TA::ROCP(vDouble vSeries, unsigned rocp_period ) const throw(TAException)
{
  if( vSeries.size() < rocp_period )
    throw TAException("Not enough data");

  size_t results = vSeries.size() - (rocp_period - 1);
  vDouble vOutReal(results);

  int outBegIdx;
  int outNbElement;

  TA_RetCode ta_ret = TA_ROCP(0, vSeries.size()-1, &vSeries[0], rocp_period,
                              &outBegIdx, &outNbElement, &vOutReal[0]);
  if( ta_ret != TA_SUCCESS )
    throw TAException(getError(ta_ret));

  if( outNbElement <= 0 )
    throw TAException("No out elements");

  if( outNbElement != results )
    throw TAException("Number of output elements doesn't match expected value");

  return vOutReal;
}


TA::STDDEVRes TA::STDDEV( vDouble vSeries, unsigned stddev_period, double sd ) const throw(TAException)
{
  if( vSeries.size() < stddev_period )
    throw TAException("Not enough data");

  size_t results = vSeries.size() - (stddev_period - 1);
  vDouble vOutReal(results);

  int outBegIdx;
  int outNbElement;

  TA_RetCode ta_ret = TA_STDDEV(0, vSeries.size()-1, &vSeries[0], stddev_period, sd,
                                &outBegIdx, &outNbElement, &vOutReal[0]);
  if( ta_ret != TA_SUCCESS )
    throw TAException(getError(ta_ret));

  if( outNbElement <= 0 )
    throw TAException("No out elements");

  if( outNbElement != results )
    throw TAException("Number of output elements doesn't match expected value");

  return vOutReal;
}


TA::BBRes TA::BBANDS( vDouble vSeries, unsigned ma_period, double sd_up, double sd_down ) const throw(TAException)
{
  if( vSeries.size() < ma_period )
    throw TAException("Not enough data");

  size_t results = vSeries.size() - (ma_period - 1);
  vDouble vOutMB(results);
  vDouble vOutUB(results);
  vDouble vOutLB(results);

  int outBegIdx;
  int outNbElement;

  TA_RetCode ta_ret = TA_BBANDS(0, vSeries.size()-1, &vSeries[0], ma_period, sd_up, sd_down, TA_MAType_SMA,
                                &outBegIdx, &outNbElement, &vOutUB[0], &vOutMB[0], &vOutLB[0]);
  if( ta_ret != TA_SUCCESS )
    throw TAException(getError(ta_ret));

  if( outNbElement <= 0 )
    throw TAException("No out elements");

  if( outNbElement != results )
    throw TAException("Number of output elements doesn't match expected value");

  BBRes res;
  res.upper_band  = vOutUB;
  res.middle_band = vOutMB;
  res.lower_band  = vOutLB;

  return res;
}


TA::FACTORRes TA::FACTORS( vDouble vSeries, unsigned period ) const throw(TAException)
{
  vDouble results;

  if( vSeries.empty() || (period + 1) > vSeries.size() )
    throw TAException("Not enough data");

  double last_element = vSeries[0];
  for( unsigned i = period; i < vSeries.size(); i += period ) {
    results.push_back(vSeries[i] / last_element);
    last_element = vSeries[i];
  }

  return results;
}


string TA::getError(TA_RetCode code) const
{
  TA_RetCodeInfo ta_retinfo;
  TA_SetRetCodeInfo(code, &ta_retinfo);
  return string(ta_retinfo.infoStr);
}
