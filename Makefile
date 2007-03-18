#
# Position Sever Makefile
#

CXX=/geek-gadgets/sparc-sun-solaris2.8/gcc-3.4/bin/g++

BIN=eom eom_matrix jan jan_matrix eow bow

SRC=\
	series.cpp \
	YahooDriver.cpp \
	ReturnFactors.cpp \
	Position.cpp \
	Report.cpp \
	Execution.cpp \
	ExecutionSet.cpp \
	LongPosition.cpp \
	ShortPosition.cpp \
	PositionSet.cpp \
	Trader.cpp \
	BnHTrader.cpp  \
	EOMTrader.cpp \
	EOWTrader.cpp \
	BOWTrader.cpp \
	JanTrader.cpp

HDR=\
	FileDriver.hpp \
	DayPrice.hpp \
	DaySeries.hpp \
	YahooDriver.hpp \
	ReturnFactors.hpp \
	Position.hpp \
	Report.hpp \
	Execution.hpp \
	ExecutionSet.hpp \
	LongPosition.hpp \
	ShortPosition.hpp \
	PositionSet.hpp \
	Trader.hpp \
	BnHTrader.hpp \
	EOMTrader.hpp \
	EOWTrader.hpp \
	BOWTrader.hpp \
	JanTrader.hpp



LIBS=-lboost_date_time-gcc -lboost_program_options -lgsl -lgslcblas

LIBPATH= \
	-L/mkv/extlib/sparc-sun-solaris2.8-gcc-3.4/boost-1_33_1/lib

RPATH= \
	-R/mkv/extlib/sparc-sun-solaris2.8-gcc-3.4/boost-1_33_1/lib

INCLUDES= \
	-I. \
	-I/mkv/extlib/sparc-sun-solaris2.8-gcc-3.4/boost-1_33_1/include

CFLAGS=-g
CPPFLASG=
LDFLAGS=
DEPFLAGS=-M


all: $(BIN)

%.d: %.cpp
	@$(CXX) $(INCLUDES) $(DEPFLAGS) $< > $@

%.o: %.cpp
	$(CXX) -c $(INCLUDES) $(DEFS) $(CPPFLAGS) $(CFLAGS) $<

eow: $(SRC:.cpp=.o) eow.o
	$(CXX) -o $@ $(LDFLAGS) $^ $(LIBPATH) $(LIBS)

bow: $(SRC:.cpp=.o) bow.o
	$(CXX) -o $@ $(LDFLAGS) $^ $(LIBPATH) $(LIBS)

eom: $(SRC:.cpp=.o) eom.o
	$(CXX) -o $@ $(LDFLAGS) $^ $(LIBPATH) $(LIBS)

eom_matrix: $(SRC:.cpp=.o) eom_matrix.o
	$(CXX) -o $@ $(LDFLAGS) $^ $(LIBPATH) $(LIBS)

jan: $(SRC:.cpp=.o) jan.o
	$(CXX) -o $@ $(LDFLAGS) $^ $(LIBPATH) $(LIBS)

jan_matrix: $(SRC:.cpp=.o) jan_matrix.o
	$(CXX) -o $@ $(LDFLAGS) $^ $(LIBPATH) $(LIBS)

clean:
	\rm -f *.o *.d core

clobber:
	\rm -f *.o *.d core $(BIN)

-include $(SRC:.cpp=.d)
