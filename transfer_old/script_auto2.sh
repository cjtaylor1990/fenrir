#!/bin/bash
photons_per_side=100
num_procs=4
param_file="./param_file.txt"
ic_file="ic1_auto.h"
output_path="."
combine_output="$output_path/test.txt"
combine_npy="$output_path/test.npy"
fits_output="$output_path/test.fits"

python2 create_params.py 1 0 0 10 15 60 $param_file

while INF=' ' read -ra line; do
	spin_val=${line[0]}
	inc_val=${line[1]}
	echo ${line[0]} ${line[1]}
	rm -f $ic_file
	python2 create_ic.py $spin_val $inc_val $ic_file
	g++ -o test.out img_plane_parallel.cpp
	dummy_i=0
	pids=()
	while [ $dummy_i -lt $num_procs ];do
		./test.out $photons_per_side $num_procs $dummy_i "$output_path/test_$dummy_i.txt" $spin_val $inc_val &
		dummy_i=$((dummy_i+1))
		pids+=($!)
	done
	wait ${pids[*]}
	cat $output_path/test_*.txt > $combine_output
	rm -f $output_path/test_*.txt
	python2 txt_to_npy_diskimg.py $combine_output $combine_npy
	python2 transfer_auto_single2.py $spin_val $combine_npy $fits_output
	rm -f $combine_npy
done < $param_file