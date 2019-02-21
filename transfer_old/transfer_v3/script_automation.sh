#!/bin/bash
photons_per_side=400
num_procs=4
param_file="./param_file.txt"
output_path="."
combine_output="$output_path/test.txt"
combine_npy="$output_path/test.npy"
fits_output="$output_path/test.fits"

while INF=' ' read -ra line; do
	echo $combine_output
	spin_val=${line[0]}
	inc_val=${line[1]}
	echo ${line[0]} ${line[1]}
	dummy_i=0
	pids=()
	while [ $dummy_i -lt $num_procs ];do
		./test.out $photons_per_side $num_procs $dummy_i "$output_path/test_$dummy_i.txt" $spin_val $inc_val &
		dummy_i=$((dummy_i+1))
		pids+=($!)
	done
	wait $pids
	cat $output_path/test_*.txt > $combine_output
	rm -f $output_path/test_*.txt
	python txt_to_npy_diskimg.py $combine_output $combine_npy
	python transfer_auto_single2.py $spin_val $combine_npy $fits_output
	rm -f $combine_npy
done < $param_file