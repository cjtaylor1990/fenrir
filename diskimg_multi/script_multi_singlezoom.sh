#!/bin/bash
photons_per_side=500
num_procs=4
output_path="/Users/cjtaylor/Desktop/multi_test4"

spin_val=0.998
inc_val=0.9659
	
dummy_i=0
pids=()
while [ $dummy_i -lt $num_procs ];do
	./test.out $photons_per_side $num_procs $dummy_i $spin_val $inc_val 11 20 0.0 30 "${output_path}/test_multi_${dummy_i}" &
	pids+=($!)
	dummy_i=$((dummy_i+1))
done
wait ${pids[*]}
