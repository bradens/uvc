#!/bin/sh
old='ls -l /etc/shadow'
new='ls -l /etc/shadow'

while [ "$old" = "$new" ]
do
new='ls -l /etc/shadow'
done
echo "STOP... The shadow ﬁle has been changed"