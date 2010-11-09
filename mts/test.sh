#!/bin/bash
#set current 0

numTrains = 6
numTests = 20
echo "If the field right of the test result is empty, the test case passed."
echo "$numTests"
for ((current = 0; current < numTests; current++))
do
	./mts numTrains > out.txt
	echo "Test case $current result: " >> results.txt
	diff out.txt ans.txt >> results.txt 
done
