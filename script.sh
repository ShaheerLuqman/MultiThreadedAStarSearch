#!/bin/bash

g++ main.cpp -o test1 > /dev/null
g++ threadPoolLib.cpp -o test2 > /dev/null

iteration=10

test1_t=0
test2_t=0

for (( i=1; i<=$iteration; i++ ))
do
    start_time=$(date +%s.%N)
    ./test1 > /dev/null
    end_time=$(date +%s.%N)
    runtime=$(echo "($end_time - $start_time) * 1000000 / 1" | bc)
    test1_t=$(echo "$test1_t + $runtime" | bc)
done

test1_t=$(echo "scale=2; $test1_t / $iteration" | bc)

for (( i=1; i<=$iteration; i++ ))
do
    start_time=$(date +%s.%N)
    ./test2 > /dev/null
    end_time=$(date +%s.%N)
    runtime=$(echo "($end_time - $start_time) * 1000000 / 1" | bc)
    test2_t=$(echo "$test2_t + $runtime" | bc)
done

test2_t=$(echo "scale=2; $test2_t / $iteration" | bc)

echo ""
echo "Test 1: $test1_t micro-seconds "
echo "Test 2: $test2_t micro-seconds "
