PROJECT(Hudson)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

#
#
#
FIND_PACKAGE(Boost 1.33 COMPONENTS date_time REQUIRED)
SET(Boost_USE_MULTITHREADED OFF)

INCLUDE_DIRECTORIES(/usr/include)
LINK_DIRECTORIES(/usr/lib)

ADD_LIBRARY(HUDSON SHARED
YahooDriver.cpp 
Database.cpp
DMYCloseDriver.cpp
EODSeries.cpp
EOMSeries.cpp
EOWSeries.cpp
EODDB.cpp
ExecutionNotifier.cpp
ReturnFactors.cpp
EOMReturnFactors.cpp
Position.cpp
Report.cpp
EOMReport.cpp
Execution.cpp
ExecutionSet.cpp
LongPosition.cpp
ShortPosition.cpp
PositionSet.cpp
PositionFactors.cpp
PositionFactorsSet.cpp
PositionsReport.cpp
SeriesFactor.cpp
SeriesFactorSet.cpp
StrategyPosition.cpp
StrategyTrader.cpp
Price.cpp
PortfolioReturns.cpp
PortfolioReport.cpp
Trader.cpp
BnHTrader.cpp
TA.cpp
)

TARGET_LINK_LIBRARIES(HUDSON boost_date_time-mt boost_program_options-mt ta_lib gslcblas)
