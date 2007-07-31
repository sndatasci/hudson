/*
* TA.cpp
*/

#include "StdAfx.h"

// TA-LIB
#include <ta_libc.h>

// Hudson
#include "TA.hpp"

using namespace std;


TA::TA( void ) throw(TAException)
{
  TA_RetCode ta_ret;

  if( (ta_ret = TA_Initialize()) != TA_SUCCESS ) {
    TA_RetCodeInfo ta_retinfo;
    TA_SetRetCodeInfo(ta_ret, &ta_retinfo);
    throw TAException(string(ta_retinfo.infoStr));
  }
}


TA::~TA( void )
{
  TA_Shutdown();
}
