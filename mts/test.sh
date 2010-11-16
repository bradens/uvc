#!/bin/bash
#set current 0

echo "Enter the number of trains in the input file."
read numTrains
echo "Enter the number of tests to run."
read numTests
echo "The tester outputs information of your tests to out.txt"
echo "It then diffs out.txt to ans.txt and records the result"
echo "in results.txt for each repetition of the test."
echo "In the results.txt file, if the field right of the test result is empty, the test case passed."
echo "You must stop the output from loading the trains, as this can be in any order and still be correct"
echo
echo "Starting Tester with parameters:"
echo "Trains: $numTrains        Tests: $numTests"
echo "====================================================================="

rm -f results.txt

for ((current = 0; current < numTests; current++))
do
        echo "Test case $current starting:"
        ./mts $numTrains > out.txt
        echo "Test case $current result: " >> results.txt
        diff out.txt ans.txt >> results.txt 
        diff out.txt ans.txt
done
