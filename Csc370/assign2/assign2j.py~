#!/usr/bin/python2.7
import psycopg2
import subprocess

conn = psycopg2.connect("dbname=postgres user=postgres password=something")

cur = conn.cursor()

print "Running question 2. J)...."

cur.execute("select * from br_j()")

subprocess.call("clear")

print "Directors\n--------------------------------------------------------"
for record in cur:
    print record[0]

cur.close()
conn.close()
