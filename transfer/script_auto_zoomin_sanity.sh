#!/bin/bash
photons_per_side=4000
num_procs=4
param_file="./param_file_sanity_all.txt"
ic_file="ic1_auto.h"
output_path="/n/galaxy/cjtaylor/transfer_auto/sanity"
combine_output="$output_path/test.txt"
combine_npy="$output_path/test.npy"
fits_output="$output_path/test.fits"

comb_num=1
while INF=' ' read -ra line; do
	spin_val=${line[0]}
	inc_val=${line[1]}
	echo ${line[0]} ${line[1]}
	dummy_i=0
	pids=()
	while [ $dummy_i -lt $num_procs ];do
		./test.out $photons_per_side $num_procs $dummy_i "$output_path/test1_$dummy_i.txt" $spin_val $inc_val 0.0 30 &
		pids+=($!)
		./test.out $photons_per_side $num_procs $dummy_i "$output_path/test2_$dummy_i.txt" $spin_val $inc_val 0.0 70 &
		pids+=($!)
		./test.out $photons_per_side $num_procs $dummy_i "$output_path/test3_$dummy_i.txt" $spin_val $inc_val 0.0 210 &
		pids+=($!)
		dummy_i=$((dummy_i+1))
	done
	wait ${pids[*]}
	cat $output_path/test1_*.txt > $output_path/cat1_$comb_num.txt
	cat $output_path/test2_*.txt > $output_path/cat2_$comb_num.txt
	cat $output_path/test3_*.txt > $output_path/cat3_$comb_num.txt
	rm -f $output_path/test1_*.txt
	rm -f $output_path/test2_*.txt
	rm -f $output_path/test3_*.txt
	comb_num=$((comb_num+1))
done < $param_file
