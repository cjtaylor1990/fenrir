#!/bin/bash
photons_per_side=3000
num_procs1=15
num_procs2=15
num_procs3=30
param_file="./param_file_yorp2.txt"
ic_file="ic1_auto.h"
output_path="/yorp2a/cjtaylor"
combine_output="$output_path/test.txt"
combine_npy="$output_path/test.npy"
fits_output="$output_path/test.fits"

dR=0.01

while INF=' ' read -ra line; do
	spin_val=${line[0]}
	inc_val=${line[1]}
	echo ${line[0]} ${line[1]}
	
	dummy_i=0
	pids=()
	while [ $dummy_i -lt $num_procs1 ];do
		./test.out $photons_per_side $num_procs1 $dummy_i "$output_path/yorp2_layer1_$dummy_i.txt" $spin_val $inc_val 0.0 30 &
		pids+=($!)
		./test.out $photons_per_side $num_procs2 $dummy_i "$output_path/yorp2_layer2_$dummy_i.txt" $spin_val $inc_val 0.0 70 &
		pids+=($!)
		dummy_i=$((dummy_i+1))
	done
	wait ${pids[*]}
	cat $output_path/yorp2_layer1_*.txt > $output_path/yorp2_cat1.txt
	cat $output_path/yorp2_layer2_*.txt > $output_path/yorp2_cat2.txt
	rm -f $output_path/yorp2_layer1_*.txt
	rm -f $output_path/yorp2_layer2_*.txt
	
	dummy_i=0
	pids=()
	while [ $dummy_i -lt $num_procs3 ];do
		./test.out $photons_per_side $num_procs3 $dummy_i "$output_path/yorp2_layer3_$dummy_i.txt" $spin_val $inc_val 0.0 210 &
		pids+=($!)
		dummy_i=$((dummy_i+1))
	done
	wait ${pids[*]}
	cat $output_path/yorp2_layer3_*.txt > $output_path/yorp2_cat3.txt
	rm -f $output_path/yorp2_layer3_*.txt

	python txt_to_npy_diskimg.py $output_path/yorp2_cat1.txt $output_path/yorp2_cat1.npy
	python txt_to_npy_diskimg.py $output_path/yorp2_cat2.txt $output_path/yorp2_cat2.npy
	python txt_to_npy_diskimg.py $output_path/yorp2_cat3.txt $output_path/yorp2_cat3.npy
	rm -f $output_path/yorp2_cat1.txt
	rm -f $output_path/yorp2_cat2.txt
	rm -f $output_path/yorp2_cat3.txt
	python transfer_auto_zoomin_v4.py $spin_val $output_path/yorp2_dr0p01.fits 3 $dR $output_path/yorp2_cat1.npy 30 $output_path/yorp2_cat2.npy 70 $output_path/yorp2_cat3.npy 210
	rm -f $output_path/yorp2_cat1.npy
	rm -f $output_path/yorp2_cat2.npy
	rm -f $output_path/yorp2_cat3.npy
done < $param_file
