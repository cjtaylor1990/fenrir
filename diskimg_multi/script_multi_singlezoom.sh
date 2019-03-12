#!/bin/bash
photons_per_side=1000
num_procs=4
output_path="/Users/cjtaylor/Desktop/single_test"

spin_val=0.9
inc_val=0.5
	
dummy_i=0
pids=()
while [ $dummy_i -lt $num_procs ];do
	./test.out $photons_per_side $num_procs $dummy_i $spin_val $inc_val 1 0.0 0.0 70 "${output_path}/test_multi_${dummy_i}" &
	pids+=($!)
	dummy_i=$((dummy_i+1))
done
wait ${pids[*]}
