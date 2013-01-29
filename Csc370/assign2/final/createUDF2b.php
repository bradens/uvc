<?php
$dbconn=pg_connect("host=localhost dbname=postgres user=postgres password=something")
        or die('Could not connect: ' . pg_last_error());

$drop = 'DROP TYPE IF EXISTS t_table CASCADE';
pg_query($drop);
$type = 'CREATE TYPE t_table as (id character varying)';
pg_query($type);
$udf = 'CREATE OR REPLACE FUNCTION discoverychannel_twob () RETURNS SETOF t_table AS ';
$udf .= '$$ select id from (select id from roles where pid = \'Eastwood, Clint\'';
$udf .= ' EXCEPT SELECT id FROM directors WHERE pid = \'Eastwood, Clint\' ) AS rip NATURAL JOIN productions ';
$udf .= 'WHERE attr IS NULL $$ LANGUAGE SQL';

pg_query($udf);
$result = pg_query('SELECT discoverychannel_twob();');
echo "Id\n----------------------------------\n";
while ($row = pg_fetch_row($result)) {
  echo $row[0]."\n";
}
pg_close($dbconn);
?>