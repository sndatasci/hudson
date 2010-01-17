PROJECT(Hudson)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SUBDIRS(lib)
INCLUDE_DIRECTORIES(/home/ag/include lib)
LINK_DIRECTORIES(/home/ag/lib /usr/lib lib)

ADD_EXECUTABLE(aa app/AATrader/AATrader.cpp app/AATrader/aa.cpp)
ADD_EXECUTABLE(aamacd app/AATraderMACD/AATraderMACD.cpp app/AATraderMACD/aaMACD.cpp)
ADD_EXECUTABLE(eom app/EOMTrader/EOMTrader.cpp app/EOMTrader/eom.cpp)
ADD_EXECUTABLE(jan app/JanTrader/JanTrader.cpp app/JanTrader/jan.cpp)
ADD_EXECUTABLE(vix app/VIXTrader/VIXTrader.cpp app/VIXTrader/vix.cpp)

TARGET_LINK_LIBRARIES(aa HUDSON boost_date_time-mt boost_program_options-mt ta_lib gsl gslcblas)
TARGET_LINK_LIBRARIES(aamacd HUDSON boost_date_time-mt boost_program_options-mt ta_lib gsl gslcblas)
TARGET_LINK_LIBRARIES(eom HUDSON boost_date_time-mt boost_program_options-mt ta_lib gsl gslcblas sqlite3)
TARGET_LINK_LIBRARIES(jan HUDSON boost_date_time-mt boost_program_options-mt ta_lib gsl gslcblas)
TARGET_LINK_LIBRARIES(vix HUDSON boost_date_time-mt boost_program_options-mt ta_lib gsl gslcblas)
