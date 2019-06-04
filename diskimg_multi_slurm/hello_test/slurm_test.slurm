#!/bin/bash

#SBATCH -n 20
#SBATCH -p debug
#SBATCH --mem-per-cpu=128
#SBATCH -t 0:5:0
#SBATCH --share
#SBATCH -o hello_world_test.out
#SBATCH -e hello_world_test.err
#SBATCH --mail-user=rabeundwolf1990@gmail.com
#SBATCH --mail-type=begin
#SBATCH --mail-type=end

echo BEGIN

num_cases=20
num_procs=20
num_cat=4

dummy_i=0
pids=()
while [ $dummy_i -lt $num_cases ]
do
        /lustre/cjtaylor/a.out /lustre/cjtaylor/hello $dummy_i $num_procs
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
                cat /lustre/cjtaylor/hello_${dummy_i}_*.txt > /lustre/cjtaylor/hello_${dummy_i}.txt
		pids+=($!)
		dummy_i=$((dummy_i+1))
		dummy_j=$((dummy_j+1))
	done
	wait ${pids[*]}
	rm -f /Users/cjtaylor/slurm_tests/hello_${dummy_i}_*.txt
done
echo END

