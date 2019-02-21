#!/bin/bash
photons_per_side=100
num_procs=1
param_file="./param_file.txt"
ic_file="ic1_auto.h"
output_path="."
combine_output="$output_path/test.txt"
combine_npy="$output_path/test.npy"
fits_output="$output_path/test.fits"

g++ -o test.out img_plane_parallel.cpp

python3 create_params.py 1 0 0 10 15 60 $param_file

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
	cat $output_path/test1_*.txt > $output_path/test1.txt
	cat $output_path/test2_*.txt > $output_path/test2.txt
	cat $output_path/test3_*.txt > $output_path/test3.txt
	rm -f $output_path/test1_*.txt
	rm -f $output_path/test2_*.txt
	rm -f $output_path/test3_*.txt
	python3 txt_to_npy_diskimg.py $output_path/test1.txt $output_path/test1.npy
	python3 txt_to_npy_diskimg.py $output_path/test2.txt $output_path/test2.npy
	python3 txt_to_npy_diskimg.py $output_path/test3.txt $output_path/test3.npy
	rm -f $output_path/test1.txt
	rm -f $output_path/test2.txt
	rm -f $output_path/test3.txt
	python3 transfer_auto_zoomin.py $spin_val $output_path/test.fits 3 $output_path/test1.npy 30 $output_path/test2.npy 70 $output_path/test3.npy 210
	rm -f $output_path/test1.npy
	rm -f $output_path/test2.npy
	rm -f $output_path/test3.npy
done < $param_file