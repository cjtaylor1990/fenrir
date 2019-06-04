#!/bin/bash
num_cases=20
num_procs=20
num_cat=4

dummy_i=0
pids=()
while [ $dummy_i -lt $num_cases ]
do
	./a.out /Users/cjtaylor/slurm_tests/hello $dummy_i $num_procs
	pids+=($!)
	dummy_i=$((dummy_i+1))
done
wait ${pids[*]}

dummy_i=0
while [ $dummy_i -lt $num_cases ]
do
	dummy_j=0
	pids=()
	while [ $dummy_j -lt $num_cat ] && [ $dummy_i -lt $num_cases ]
	do
		cat /Users/cjtaylor/slurm_tests/hello_${dummy_i}_*.txt > /Users/cjtaylor/slurm_tests/output/hello_${dummy_i}.txt
		pids+=($!)
		dummy_i=$((dummy_i+1))
		dummy_j=$((dummy_j+1))
	done
	wait ${pids[*]}
	rm -f /Users/cjtaylor/slurm_tests/hello_${dummy_i}_*.txt
done
