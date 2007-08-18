# Hudson Makefile

CXX=g++

LIB=libhudson.a
BIN=vix eom

SRC=\
	DaySeries.cpp \
	YahooDriver.cpp \
	ReturnFactors.cpp \
	Position.cpp \
	Report.cpp \
	Execution.cpp \
	ExecutionSet.cpp \
	LongPosition.cpp \
	ShortPosition.cpp \
	Position.cpp \
	PositionSet.cpp \
	PositionFactors.cpp \
	PositionFactorsSet.cpp \
	PositionsReport.cpp \
	SeriesFactor.cpp \
	SeriesFactorSet.cpp \
	Price.cpp \
	Trader.cpp \
	BnHTrader.cpp \
	StdAfx.cpp \
	TA.cpp

HDR=\
	FileDriver.hpp \
	DayPrice.hpp \
	DaySeries.hpp \
	YahooDriver.hpp \
	ReturnFactors.hpp \
	Position.hpp \
	PositionFactors.hpp \
	Report.hpp \
	Execution.hpp \
	ExecutionSet.hpp \
	LongPosition.hpp \
	ShortPosition.hpp \
	PositionSet.hpp \
	PositionFactors.hpp \
	PositionFactorsSet.hpp \
	PositionsReport.hpp \
	SeriesFactor.hpp \
	SeriesFactorSet.hpp \
	Trader.hpp \
	BnHTrader.hpp \
	Price.hpp \
	TA.hpp


LIBS=-lhudson -lboost_date_time -lboost_program_options -lta_common -lta_func -lgsl -lgslcblas

LIBPATH= \
	-L. \
	-L/opt/local/lib

RPATH= \
	-R. \
	-R/opt/local/lib

INCLUDES= \
	-I. \
	-I/home/users/agiannetti/include \
	-I/opt/local/include \
	-I/opt/local/include/ta-lib

CFLAGS=-g
CPPFLASG=
LDFLAGS=
DEPFLAGS=-M


all: $(LIB) $(BIN)

lib: $(LIB)

bin: $(BIN)

%.d: %.cpp
	@$(CXX) $(INCLUDES) $(DEPFLAGS) $< > $@

%.o: %.cpp
	$(CXX) -c $(INCLUDES) $(DEFS) $(CPPFLAGS) $(CFLAGS) $< -o $@

libhudson.a: $(SRC:.cpp=.o)
	ar r $@ $^
	ranlib $@

eow: $(LIB) eow.o EOWTrader.o
	$(CXX) -o $@ $(LDFLAGS) eow.o EOWTrader.o $(LIBPATH) $(LIBS)

bow: $(LIB) bow.o BOWTrader.o
	$(CXX) -o $@ $(LDFLAGS) bow.o BOWTrader.o $(LIBPATH) $(LIBS)

eom: $(LIB) eom.o EOMTrader.o
	$(CXX) -o $@ $(LDFLAGS) eom.o EOMTrader.o $(LIBPATH) $(LIBS)

vix: $(LIB) vix.o VIXTrader.o
	$(CXX) -o $@ $(LDFLAGS) vix.o VIXTrader.o $(LIBPATH) $(LIBS)

eom_matrix: $(LIB) eom_matrix.o EOMTrader.o
	$(CXX) -o $@ $(LDFLAGS) eom_matrix.o EOMTrader.o $(LIBPATH) $(LIBS)

jan: $(LIB) jan.o JanTrader.o
	$(CXX) -o $@ $(LDFLAGS) jan.o JanTrader.o $(LIBPATH) $(LIBS)

jan_matrix: $(LIB) jan_matrix.o JanTrader.o
	$(CXX) -o $@ $(LDFLAGS) jan_matrix.o JanTrader.o $(LIBPATH) $(LIBS)

clean:
	\rm -f *.o *.d core

clobber:
	\rm -f *.o *.d core $(BIN)

-include dep/$(SRC:.cpp=.d)
