PROJECT(Hudson)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SUBDIRS(lib)
INCLUDE_DIRECTORIES(/opt/local/include /opt/local/include/ta-lib lib)
LINK_DIRECTORIES(/opt/local/lib)

ADD_EXECUTABLE(aa app/AATrader/AATrader.cpp app/AATrader/aa.cpp)
ADD_EXECUTABLE(aamacd app/AATraderMACD/AATraderMACD.cpp app/AATraderMACD/aaMACD.cpp)
ADD_EXECUTABLE(eom app/EOMTrader/EOMTrader.cpp app/EOMTrader/eom.cpp)
ADD_EXECUTABLE(jan app/JanTrader/JanTrader.cpp app/JanTrader/jan.cpp)
ADD_EXECUTABLE(vix app/VIXTrader/VIXTrader.cpp app/VIXTrader/vix.cpp)

TARGET_LINK_LIBRARIES(aa boost_date_time boost_program_options ta_common ta_func gsl gslcblas HUDSON)
TARGET_LINK_LIBRARIES(aamacd boost_date_time boost_program_options ta_common ta_func gsl gslcblas HUDSON)
TARGET_LINK_LIBRARIES(eom boost_date_time boost_program_options ta_common ta_func gsl gslcblas HUDSON)
TARGET_LINK_LIBRARIES(jan boost_date_time boost_program_options ta_common ta_func gsl gslcblas HUDSON)
TARGET_LINK_LIBRARIES(vix boost_date_time boost_program_options ta_common ta_func gsl gslcblas HUDSON)

