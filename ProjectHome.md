Hudson is a free, open-source trading simulator based on EOD price historical data. It is designed as a C++ library providing simulation and statistics tools for integration with other trading strategy applications. A set of strategy examples is included in the source code distribution to illustrate some of the features provided.
This software is open source. You are required to redistribute any change you make to the original source code following the GPLv3 license agreement. Please read the COPYING file for more information on GPL licensing.

Hudson calculates various statistics, including compound annualized growth rate, % winners/losers, realized drawdown, position excursion analysis, month-to-month returns, Sharpe ratio and geometric standard deviation of monthly returns.
The report statistics are easily extensible by inheriting from the Report class and adding your own calculation based on recorded transactions and historical data.

The trader API supports backtesting of any custom long/short trading strategy, such as portfolio backtesting of multiple symbols, pairs-trading and spread-trading strategies. For a spread trading example, check the January trader (JanTrader). This class implements a trading strategy analyzing the seasonal microcap effect that occurs around the end of the year.

The Asset Allocator example (AATrader) evaluates the performance of 5 asset classes (World Equities, SP500, Commodities, US Bonds and REIT) on a monthly basis and make allocation decisions based on each index SMA.

Please note that the goal of this project is to provide a set of open source tools to create and improve trading strategies and NOT to offer trading advice. Basic support is provided by the author through the TA-DOC forums at http://www.tadoc.org/forum.

Hudson requires the GNU Scientific Library (GSL), TA-Lib, and the Boost libraries.

The GSL is a free, open source library developed by the FSF offering over 1000 numerical functions including statistics, least-square fitting, random numbers and Monte Carlo integration: http://www.gnu.org/software/gsl/.

Boost provides a set of free, open source portable C++ libraries implementing a wide range of functionalities, from a complete set of date and time functions to multi-index collections and objects serialization: http://www.boost.org/libs/libraries.htm.

TA-LIB is a free, open-source financial series technical analysis library maintained by Mario Fortier. It offers support to more than 150 technical analysis indicators in C++, Java, Excel, .NET and other languages: http://ta-lib.org.