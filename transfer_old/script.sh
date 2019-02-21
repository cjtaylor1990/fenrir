#!/bin/bash
rm -f parallel_test.txt
photons_per_side=100
num_procs=4
dummy_i=0
while [ $dummy_i -lt $num_procs ]
do
	./test.out $photons_per_side $num_procs $dummy_i "test_$dummy_i.txt" 0.0 15
	dummy_i=$((dummy_i+1))
done