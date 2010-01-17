CREATE TABLE myimp(day_date VARCHAR(32), open_price REAL, high_price REAL, low_price REAL, close_price REAL, VOLUME INTEGER, adjclose_price REAL);

.import NDX.csv myimp

INSERT INTO eod (symbol, day_date, open_price, high_price, low_price, close_price, adjclose_price, volume) SELECT 'NDX', day_date, open_price, high_price, low_price, close_price, adjclose_price, volume FROM myimp;
