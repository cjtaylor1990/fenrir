#!/bin/bash
photons_per_side=3000
num_procs1=5
num_procs2=10
param_file="./param_file_yorp8.txt"
machine_name="yorp8"
output_path="/yorp8c/cjtaylor"

initMdot=20.0
finalMdot=0.0
numMdot=11

dR=0.01

echo "Meow!"

while INF=' ' read -ra line; do
	spin_val=${line[0]}
	inc_val=${line[1]}
	echo ${line[0]} ${line[1]}
	
	dummy_i=0
	pids=()
	while [ $dummy_i -lt $num_procs1 ];do
		./test.out $photons_per_side $num_procs1 $dummy_i $spin_val $inc_val $numMdot $initMdot $finalMdot 30 "${output_path}/${machine_name}_layer1_${dummy_i}" &
		pids+=($!)
		./test.out $photons_per_side $num_procs1 $dummy_i $spin_val $inc_val $numMdot $initMdot $finalMdot 70 "${output_path}/${machine_name}_layer2_${dummy_i}" &
		pids+=($!)
		dummy_i=$((dummy_i+1))
	done
	wait ${pids[*]}
	
	dummy_j=0
	while [ $dummy_j -lt $numMdot ];do
		cat ${output_path}/${machine_name}_layer1_*_${dummy_j}.txt > ${output_path}/${machine_name}_cat1_${dummy_j}.txt
		cat ${output_path}/${machine_name}_layer2_*_${dummy_j}.txt > ${output_path}/${machine_name}_cat2_${dummy_j}.txt
		rm -f ${output_path}/${machine_name}_layer1_*_${dummy_j}.txt
		rm -f ${output_path}/${machine_name}_layer2_*_${dummy_j}.txt
		python txt_to_npy_diskimg.py ${output_path}/${machine_name}_cat1_${dummy_j}.txt ${output_path}/${machine_name}_cat1_${dummy_j}.npy
		python txt_to_npy_diskimg.py ${output_path}/${machine_name}_cat2_${dummy_j}.txt ${output_path}/${machine_name}_cat2_${dummy_j}.npy
		rm -f ${output_path}/${machine_name}_cat1_${dummy_j}.txt
		rm -f ${output_path}/${machine_name}_cat2_${dummy_j}.txt
		dummy_j=$((dummy_j+1))
	done
	
	dummy_i=0
	pids=()
	while [ $dummy_i -lt $num_procs2 ];do
		./test.out $photons_per_side $num_procs2 $dummy_i $spin_val $inc_val $numMdot $initMdot $finalMdot 210 "${output_path}/${machine_name}_layer3_${dummy_i}" &
		pids+=($!)
		dummy_i=$((dummy_i+1))
	done
	wait ${pids[*]}
	
	dummy_j=0
	while [ $dummy_j -lt $numMdot ];do
		cat ${output_path}/${machine_name}_layer3_*_${dummy_j}.txt > ${output_path}/${machine_name}_cat3_${dummy_j}.txt
		rm -f ${output_path}/${machine_name}_layer3_*_${dummy_j}.txt
		python txt_to_npy_diskimg.py ${output_path}/${machine_name}_cat3_${dummy_j}.txt ${output_path}/${machine_name}_cat3_${dummy_j}.npy
		rm -f ${output_path}/${machine_name}_cat3_${dummy_j}.txt
		python transfer_auto_zoomin_v4.py $spin_val ${output_path}/${machine_name}_dr0p01_${dummy_j}.fits 3 $dR ${output_path}/${machine_name}_cat1_${dummy_j}.npy 30 ${output_path}/${machine_name}_cat2_${dummy_j}.npy 70 ${output_path}/${machine_name}_cat3_${dummy_j}.npy 210
		rm -f ${output_path}/${machine_name}_cat1_${dummy_j}.npy
		rm -f ${output_path}/${machine_name}_cat2_${dummy_j}.npy
		rm -f ${output_path}/${machine_name}_cat3_${dummy_j}.npy
		dummy_j=$((dummy_j+1))
	done
done < $param_file
