#
# Copyright (C) 2007, Alberto Giannetti
#
# This file is part of Hudson.
#
# Hudson is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Hudson is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Hudson.  If not, see <http://www.gnu.org/licenses/>.
#

CXX=g++

LIB=libhudson.a
BIN=vix eom eow bow jan aa

SRC=\
	YahooDriver.cpp \
	DMYCloseDriver.cpp \
	EODSeries.cpp \
	ReturnFactors.cpp \
	EOMReturnFactors.cpp \
	Position.cpp \
	Report.cpp \
	EOMReport.cpp \
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
	PortfolioReturns.cpp \
	PortfolioReport.cpp \
	Trader.cpp \
	BnHTrader.cpp \
	StdAfx.cpp \
	TA.cpp

HDR=\
	FileDriver.hpp \
	DayPrice.hpp \
	EODSeries.hpp \
	DMYCloseDriver.hpp \
	YahooDriver.hpp \
	ReturnFactors.hpp \
	EOMReturnFactors.hpp \
	Report.hpp \
	EOMReport.hpp \
	Execution.hpp \
	ExecutionSet.hpp \
	LongPosition.hpp \
	ShortPosition.hpp \
	Position.hpp \
	PositionSet.hpp \
	PositionFactors.hpp \
	PositionFactorsSet.hpp \
	PositionsReport.hpp \
	PortfolioReturns.hpp \
	PortfolioReport.hpp \
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

aa: $(LIB) aa.o AATrader.o
	$(CXX) -o $@ $(LDFLAGS) aa.o AATrader.o $(LIBPATH) $(LIBS)

jan_matrix: $(LIB) jan_matrix.o JanTrader.o
	$(CXX) -o $@ $(LDFLAGS) jan_matrix.o JanTrader.o $(LIBPATH) $(LIBS)

clean:
	\rm -f *.o *.d core

clobber:
	\rm -f *.o *.d core $(BIN)

-include dep/$(SRC:.cpp=.d)
