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

SRC=\
	YahooDriver.cpp \
	DMYCloseDriver.cpp \
	EODSeries.cpp \
	EOMSeries.cpp \
	EOWSeries.cpp \
	EODDB.cpp \
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
	WeekPrice.hpp \
	MonthPrice.hpp \
	EODSeries.hpp \
	EOWSeries.hpp \
	EOMSeries.hpp \
	EODDB.hpp \
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

INCLUDES= \
	-I. \
	-I/opt/local/include \
	-I/opt/local/include/ta-lib

CFLAGS=-g -O3
CPPFLASG=
LDFLAGS=
DEPFLAGS=-M


all: $(LIB)

lib: $(LIB)

%.d: %.cpp
	@$(CXX) $(INCLUDES) $(DEPFLAGS) $< > $@

%.o: %.cpp
	$(CXX) -c $(INCLUDES) $(DEFS) $(CPPFLAGS) $(CFLAGS) $< -o $@

libhudson.a: $(SRC:.cpp=.o)
	ar r $@ $^
	ranlib $@

clean:
	\rm -f *.o *.d core

clobber:
	\rm -f *.o *.d core $(BIN)

-include dep/$(SRC:.cpp=.d)

