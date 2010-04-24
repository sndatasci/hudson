#!/bin/bash

USAGE="yahoofetch -s symbol -b begin_date (YYYYMMDD) -e end_date (YYYYMMDD)"

symbol=
begin_date=
end_date=

while getopts "s:b:e:" ARG
do
    case $ARG in
        s)
            symbol=$OPTARG
            ;;
        b)
            begin_date=$OPTARG
            ;;
        e)
            end_date=$OPTARG
            ;;
    esac
done

if [[ -z "$symbol" || -z "$begin_date" || -z "$end_date" ]]
then
    echo $USAGE
    exit -1
fi

url="http://ichart.finance.yahoo.com/table.csv?s=${symbol}&a=00&b=23&c=2009&d=03&e=24&f=2010&g=d&ignore=.csv"

begin_year=${begin_date:0:4}
begin_month=${begin_date:4:2}
begin_day=${begin_date:6:2}

end_year=${end_date:0:4}
end_month=${end_date:4:2}
end_day=${end_date:6:2}

begin_month=$(( 10#$begin_month-1 ))
end_month=$(( 10#$end_month-1 ))

ofile=${symbol}.csv

url=`printf 'http://ichart.finance.yahoo.com/table.csv?s=%s&a=%02d&b=%d&c=%d&d=%02d&e=%d&f=%d&g=d&ignore=.csv' ${symbol} ${begin_month} ${begin_day} ${begin_year} ${end_month} ${end_day} ${end_year}`

wget -q -O ${ofile} "${url}"

exit 0
