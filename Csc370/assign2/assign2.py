#!/usr/bin/python2.7
import psycopg2

conn = psycopg2.connect("dbname=postgres user=postgres password=something")

cur = conn.cursor()

cur.execute("Select * from productions limit 1;")
print cur.fetchone()
cur.close()
conn.close()
