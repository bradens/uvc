<?php
$dbconn=pg_connect("host=localhost dbname=postgres user=postgres password=something")
        or die('Could not connect: ' . pg_last_error());

$drop = 'DROP TYPE IF EXISTS t_table_b CASCADE';
pg_query($drop);
$type = 'CREATE TYPE t_table_b AS (title character varying, lastname character varying, firstname character varying)';
pg_query($type);
$udf = 'CREATE OR REPLACE FUNCTION "discoverychannel_twoc" () RETURNS SETOF t_table_b AS ';
$udf .= '$$ select title, lastname, firstname ';
$udf .= 'from roles natural join productions natural join directors natural join persons where attr is NULL';
$udf .= '$$ LANGUAGE SQL';

pg_query($udf);
$result = pg_query('SELECT discoverychannel_twoc();');
echo "title\t\t|\t\tlastname\t\t|\t\tfirstname\n--------------------------------------------------------------------------------------\n";
while ($row = pg_fetch_row($result)) {
  echo $row[0]."\t|\t".$row[1]."\t|\t".$row[2];
}
pg_close($dbconn);
?>