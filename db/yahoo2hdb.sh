#!/bin/bash

#
# Import Yahoo historical prices to Hudson DB. Command line invokation must indicate
# the name of the Yahoo .csv file and symbol name chosen for this data import.
#
# Example:
#
# yahoo2hdb.sh NDX.csv NDX
#

if(( $# != 2 )); then
    printf "Usage: %s file ticker\n" $0
    exit -1
fi

FILENAME=$1
TICKER=$2

#
# Run sqlite import commands. Import table column order must mirror raw file columns.
#
sqlite3 eoddb <<EOF
CREATE TEMPORARY TABLE myimport(
  day_date VARCHAR(16),
  open_price REAL,
  high_price REAL,
  low_price REAL,
  close_price REAL,
  volume INTEGER,
  adjclose_price REAL
);
.separator ","
.import $FILENAME myimport
INSERT INTO eod(symbol, day_date, open_price, high_price, low_price, close_price, adjclose_price, volume) SELECT '$TICKER', day_date, open_price, high_price, low_price, close_price, adjclose_price, volume FROM myimport;
DELETE FROM eod WHERE open_price='Open';
EOF

if(( $? != 0 )); then
    exit -4
fi

exit 0
