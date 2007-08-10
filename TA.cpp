/*
* TA.cpp
*/

#include "StdAfx.h"

// STL
#include <vector>

// Hudson
#include "TA.hpp"

using namespace std;


TA::TA( const DB& db ) throw(TAException):
  _db(db)
{
  TA_RetCode ta_ret;

  if( (ta_ret = TA_Initialize()) != TA_SUCCESS )
    throw TAException(getError(ta_ret));
}


TA::~TA( void )
{
  TA_Shutdown();
}


double TA::SMA( SERIES_TYPE st, DB::const_iterator& iter, unsigned ma_period ) const throw(TAException)
{
  vDouble vSMA = getSeries(st, iter, ma_period);
  if( vSMA.size() < ma_period )
    throw TAException("Not enough data");

  vDouble vOutReal(vSMA.size());

  int outBegIdx;
  int outNbElement;

  TA_RetCode ta_ret = TA_MA(0, ma_period-1, &vSMA[0], ma_period, TA_MAType_SMA,
                         &outBegIdx, &outNbElement, &vOutReal[0]);
  if( ta_ret != TA_SUCCESS )
    throw TAException(getError(ta_ret));

  if( outNbElement <= 0 )
    throw TAException("No out elements");

  if( outNbElement > 1 )
    throw TAException("More than one element");

  return vOutReal[0];
}


TA::EMARes TA::EMA( SERIES_TYPE st, DB::const_iterator& iter, unsigned ma_period ) const throw(TAException)
{
  vDouble vEMA = getSeries(st, iter, ma_period);
  if( vEMA.size() < ma_period )
    throw TAException("Not enough data");

  vDouble vOutReal(vEMA.size());

  int outBegIdx;
  int outNbElement;

  TA_RetCode ta_ret = TA_MA(0, ma_period-1, &vEMA[0], ma_period, TA_MAType_EMA,
                            &outBegIdx, &outNbElement, &vOutReal[0]);
  if( ta_ret != TA_SUCCESS )
    throw TAException(getError(ta_ret));

  if( outNbElement <= 0 )
    throw TAException("No out elements");

  if( outNbElement > 1 )
    throw TAException("More than one element");

  return vOutReal[0];
}


TA::RSIRes TA::RSI( SERIES_TYPE st, DB::const_iterator& iter, unsigned rsi_period ) const throw(TAException)
{
  vDouble vRSI = getSeries(st, iter, rsi_period);
  if( vRSI.size() < rsi_period )
    throw TAException("Not enough data");

  vDouble vOutReal(vRSI.size());

  int outBegIdx;
  int outNbElement;

  TA_RetCode ta_ret = TA_RSI(0, rsi_period-1, &vRSI[0], rsi_period,
                            &outBegIdx, &outNbElement, &vOutReal[0]);
  if( ta_ret != TA_SUCCESS )
    throw TAException(getError(ta_ret));

  if( outNbElement <= 0 )
    throw TAException("No out elements");

  if( outNbElement > 1 )
    throw TAException("More than one element");

  return vOutReal[0];
}


TA::RSIRes TA::ROC( SERIES_TYPE st, DB::const_iterator& iter, unsigned roc_period ) const throw(TAException)
{
  vDouble vROC = getSeries(st, iter, roc_period);
  if( vROC.size() < roc_period )
    throw TAException("Not enough data");

  vDouble vOutReal(vROC.size());

  int outBegIdx;
  int outNbElement;

  TA_RetCode ta_ret = TA_ROC(0, roc_period-1, &vROC[0], roc_period,
                             &outBegIdx, &outNbElement, &vOutReal[0]);
  if( ta_ret != TA_SUCCESS )
    throw TAException(getError(ta_ret));

  if( outNbElement <= 0 )
    throw TAException("No out elements");

  if( outNbElement > 1 )
    throw TAException("More than one element");

  return vOutReal[0];
}


TA::RSIRes TA::ROCR( SERIES_TYPE st, DB::const_iterator& iter, unsigned rocr_period ) const throw(TAException)
{
  vDouble vROC = getSeries(st, iter, rocr_period);
  if( vROC.size() < rocr_period )
    throw TAException("Not enough data");

  vDouble vOutReal(vROC.size());

  int outBegIdx;
  int outNbElement;

  TA_RetCode ta_ret = TA_ROCR(0, rocr_period-1, &vROC[0], rocr_period,
                              &outBegIdx, &outNbElement, &vOutReal[0]);
  if( ta_ret != TA_SUCCESS )
    throw TAException(getError(ta_ret));

  if( outNbElement <= 0 )
    throw TAException("No out elements");

  if( outNbElement > 1 )
    throw TAException("More than one element");

  return vOutReal[0];
}


TA::ROCPRes TA::ROCP( SERIES_TYPE st, DB::const_iterator& iter, unsigned rocp_period ) const throw(TAException)
{
  vDouble vROCP = getSeries(st, iter, rocp_period);
  if( vROCP.size() < rocp_period )
    throw TAException("Not enough data");

  vDouble vOutReal(vROCP.size());

  int outBegIdx;
  int outNbElement;

  TA_RetCode ta_ret = TA_ROCP(0, rocp_period-1, &vROCP[0], rocp_period,
                              &outBegIdx, &outNbElement, &vOutReal[0]);
  if( ta_ret != TA_SUCCESS )
    throw TAException(getError(ta_ret));

  if( outNbElement <= 0 )
    throw TAException("No out elements");

  if( outNbElement > 1 )
    throw TAException("More than one element");

  return vOutReal[0];
}


TA::STDDEVRes TA::STDDEV( SERIES_TYPE st, DB::const_iterator& iter, unsigned stddev_period, double sd ) const throw(TAException)
{
  vDouble vSTDDEV = getSeries(st, iter, stddev_period);
  if( vSTDDEV.size() < stddev_period )
    throw TAException("Not enough data");

  vDouble vOutReal(vSTDDEV.size());

  int outBegIdx;
  int outNbElement;

  TA_RetCode ta_ret = TA_STDDEV(0, stddev_period-1, &vSTDDEV[0], stddev_period, sd,
                                &outBegIdx, &outNbElement, &vOutReal[0]);
  if( ta_ret != TA_SUCCESS )
    throw TAException(getError(ta_ret));

  if( outNbElement <= 0 )
    throw TAException("No out elements");

  if( outNbElement > 1 )
    throw TAException("More than one element");

  return vOutReal[0];
}


TA::BBRes TA::BBANDS( SERIES_TYPE st, DB::const_iterator& iter, unsigned ma_period, double sd_up, double sd_down ) const throw(TAException)
{
  vDouble vSMA = getSeries(st, iter, ma_period);
  if( vSMA.size() < ma_period )
    throw TAException("Not enough data");

  vDouble vOutMB(vSMA.size());
  vDouble vOutUB(vSMA.size());
  vDouble vOutLB(vSMA.size());

  int outBegIdx;
  int outNbElement;

  TA_RetCode ta_ret = TA_BBANDS(0, ma_period-1, &vSMA[0], ma_period, sd_up, sd_down, TA_MAType_SMA,
                                &outBegIdx, &outNbElement, &vOutUB[0], &vOutMB[0], &vOutLB[0]);
  if( ta_ret != TA_SUCCESS )
    throw TAException(getError(ta_ret));

  if( outNbElement <= 0 )
    throw TAException("No out elements");

  if( outNbElement > 1 )
    throw TAException("More than one element");

  BBRes res;
  res.upper_band = vOutUB[0];
  res.middle_band = vOutMB[0];
  res.lower_band = vOutLB[0];

  return res;
}


TA::vDouble TA::getSeries(SERIES_TYPE st, DB::const_iterator& iter, unsigned len) const throw(TAException)
{
  switch( st )
  {
    case OPEN:
      return _db.open(iter, len);

    case CLOSE:
      return _db.close(iter, len);

    case HIGH:
      return _db.high(iter, len);

    case LOW:
      return _db.low(iter, len);

    case VOLUME:
      return _db.volume(iter, len);

    default:
      throw TAException("Invalid series type");
  }
}


string TA::getError(TA_RetCode code) const
{
  TA_RetCodeInfo ta_retinfo;
  TA_SetRetCodeInfo(code, &ta_retinfo);
  return string(ta_retinfo.infoStr);
}

