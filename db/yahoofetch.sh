#!/bin/bash

#####################################################################################

function download {

    # Parameters
    local filename=$1
    local begin=$2
    local end=$3
    # End parameters

    local begin_year=${begin:0:4}
    local begin_month=${begin:4:2}
    local begin_day=${begin:6:2}

    local end_year=${end:0:4}
    local end_month=${end:4:2}
    local end_day=${end:6:2}

    local begin_month=$(( 10#$begin_month-1 ))
    local end_month=$(( 10#$end_month-1 ))

    url=`printf 'http://ichart.finance.yahoo.com/table.csv?s=%s&a=%02d&b=%d&c=%d&d=%02d&e=%d&f=%d&g=d&ignore=.csv' ${symbol} ${begin_month} ${begin_day} ${begin_year} ${end_month} ${end_day} ${end_year}`

    wget -q -O ${filename} "${url}"

    return $?
}

#####################################################################################

function dbimport
{

    # Parameters
    local db=$1
    local filename=$2
    # End parameters

    sqlite3 $db <<EOF
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
.import $filename myimport
INSERT OR REPLACE INTO eod(symbol, day_date, open_price, high_price, low_price, close_price, adjclose_price, volume) SELECT '$symbol', day_date, open_price, high_price, low_price, close_price, adjclose_price, volume FROM myimport;
DELETE FROM eod WHERE open_price='Open';
EOF

    return $?
}

#####################################################################################

USAGE="yahoofetch -f tickers_file -d database -b begin_date (YYYYMMDD) -e end_date (YYYYMMDD)"

tickers_file=
begin_date=
end_date=
database=

while getopts "f:b:e:d:" ARG
do
    case $ARG in
        f)
            tickers_file=$OPTARG
            ;;
        d)
            database=$OPTARG
            ;;
        b)
            begin_date=$OPTARG
            ;;
        e)
            end_date=$OPTARG
            ;;
    esac
done

if [[ -z "$tickers_file" || -z "$begin_date" || -z "$end_date" || -z "$database" ]]
then
    echo $USAGE
    exit -1
fi

cat $tickers_file | while read line
do
    symbol=$line
    filename=${symbol}.csv
    echo "Downloading $symbol..."
    download $filename $begin_date $end_date
    if [ $? -ne 0 ] ; then
        echo "$symbol download failed"
        continue
    fi

    echo "Importing $symbol..."
    dbimport $database $filename
    if [ $? -ne 0 ] ; then
        echo "$symbol import failed"
        continue
    fi

    rm $filename

    sleep 1
done

exit 0
