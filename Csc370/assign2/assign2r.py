#!/usr/bin/python2.7
import psycopg2
import subprocess

conn = psycopg2.connect("dbname=postgres user=postgres password=something")

cur = conn.cursor()

print "Running question 2. r)...."

cur.execute("select * from discoverychannel_r()")

subprocess.call("clear")

print "title    |   count\n--------------------------------------------------------"
for record in cur:
    print record[0] + " | " + str(record[1])

cur.close()
conn.close()
