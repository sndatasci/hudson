#!/usr/bin/nawk -f

#
# BEGIN
#
BEGIN {
  sqlite_prog = "/usr/bin/sqlite3";
  prog_name = "yahoo2hdb.awk";
  database_name = "eoddb";

  FS=",";			# Comma-separated file
  OFS="";			# No space between variable prints

  if( ARGC != 3 ) {		# Check usage
    print "Usage: ", prog_name, " symbol file";
    exit -1;
  }

  getline;			# Discard title line
}

#
# BODY
#
{ 
  cmd = sprintf("%s %s \"INSERT INTO eod (symbol, day_date, open_price, high_price, low_price, close_price, adjclose_price, volume) VALUES('%s', '%s', %f, %f, %f, %f, %f, %u)\"",
		sqlite_prog, database_name, ARGV[2], $1, $2, $3, $4, $5, $7, $6); 
  ret = system(cmd);
  if( ret != 0 ) {
    printf("Cannot insert '%s' record\n", $1);
    exit -2;
  }
}

#
# END
#
END {
  printf("Inserted %u record(s)\n", NR);
}
