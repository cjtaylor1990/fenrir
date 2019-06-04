#!/bin/bash
rm -f parallel_test.txt
photons=1000000
num_procs=4
dummy_i=0
while [ $dummy_i -lt $num_procs ]
do
	./test.out $photons $num_procs $dummy_i "test_random_$dummy_i.txt" &
	dummy_i=$((dummy_i+1))
done
