#!/bin/bash
photons_per_side=3000
num_procs1=15
num_procs2=30
num_procs_py=10
param_file="./yorp1_full_parameter_list.txt"
machine_name="yorp1"
output_path="/yorp1a/cjtaylor"

initMdot=20.0
finalMdot=0.0
numMdot=25

dR=0.01

while INF=' ' read -ra line
do
	spin_val=${line[0]}
	inc_val=${line[1]}
	echo ${line[0]} ${line[1]}

	dummy_i=0
	pids=()
	while [ $dummy_i -lt $num_procs1 ]
	do
		./test.out $photons_per_side $num_procs1 $dummy_i $spin_val $inc_val $numMdot $initMdot $finalMdot 30 "${output_path}/${machine_name}_layer1_${dummy_i}" &
		pids+=($!)
		./test.out $photons_per_side $num_procs1 $dummy_i $spin_val $inc_val $numMdot $initMdot $finalMdot 70 "${output_path}/${machine_name}_layer2_${dummy_i}" &
		pids+=($!)
		dummy_i=$((dummy_i+1))
	done
	wait ${pids[*]}

	dummy_j=0
	while [ $dummy_j -lt $numMdot ]
	do
		dummy_k=0
		pids=()
		while [ $dummy_k -lt $num_procs_py ] && [ $dummy_j -lt $numMdot ]
		do
			cat ${output_path}/${machine_name}_layer1_*_${dummy_j}.txt > ${output_path}/${machine_name}_cat1_${dummy_j}.txt &
			pids+=($!)
			cat ${output_path}/${machine_name}_layer2_*_${dummy_j}.txt > ${output_path}/${machine_name}_cat2_${dummy_j}.txt &
			pids+=($!)
			dummy_j=$((dummy_j+1))
			dummy_k=$((dummy_k+1))
		done
		wait ${pids[*]}
	done

	rm -f ${output_path}/${machine_name}_layer1_*_*.txt
	rm -f ${output_path}/${machine_name}_layer2_*_*.txt

	dummy_j=0
	while [ $dummy_j -lt $numMdot ]
	do
		dummy_k=0
		pids=()
		while [ $dummy_k -lt $num_procs_py ] && [ $dummy_j -lt $numMdot ]
		do
			python txt_to_npy_diskimg.py ${output_path}/${machine_name}_cat1_${dummy_j}.txt ${output_path}/${machine_name}_cat1_${dummy_j}.npy &
			pids+=($!)
			python txt_to_npy_diskimg.py ${output_path}/${machine_name}_cat2_${dummy_j}.txt ${output_path}/${machine_name}_cat2_${dummy_j}.npy &
			pids+=($!)
			dummy_k=$((dummy_k+1))
			dummy_j=$((dummy_j+1))
		done
		wait ${pids[*]}
	done

	rm -f ${output_path}/${machine_name}_cat1_*.txt
	rm -f ${output_path}/${machine_name}_cat2_*.txt

	dummy_i=0
	pids=()
	while [ $dummy_i -lt $num_procs2 ]
	do
		./test.out $photons_per_side $num_procs2 $dummy_i $spin_val $inc_val $numMdot $initMdot $finalMdot 210 "${output_path}/${machine_name}_layer3_${dummy_i}" &
		pids+=($!)
		dummy_i=$((dummy_i+1))
	done
	wait ${pids[*]}

	dummy_j=0
	while [ $dummy_j -lt $numMdot ]
	do
		dummy_k=0
		pids=()
		while [ $dummy_k -lt $num_procs_py ] && [ $dummy_j -lt $numMdot ]
		do
			cat ${output_path}/${machine_name}_layer3_*_${dummy_j}.txt > ${output_path}/${machine_name}_cat3_${dummy_j}.txt &
			pids+=($!)
			dummy_j=$((dummy_j+1))
			dummy_k=$((dummy_k+1))
		done
		wait ${pids[*]}
	done

	rm -f ${output_path}/${machine_name}_layer3_*_*.txt

	dummy_j=0
	while [ $dummy_j -lt $numMdot ]
	do
		dummy_k=0
		pids=()
		while [ $dummy_k -lt $num_procs_py ] && [ $dummy_j -lt $numMdot ]
		do
			python txt_to_npy_diskimg.py ${output_path}/${machine_name}_cat3_${dummy_j}.txt ${output_path}/${machine_name}_cat3_${dummy_j}.npy &
			pids+=($!)
			dummy_k=$((dummy_k+1))
			dummy_j=$((dummy_j+1))
		done
		wait ${pids[*]}
	done

	rm -f ${output_path}/${machine_name}_cat3_*.txt

	dummy_i=0
	pids=()
	while [ $dummy_i -lt $num_procs2 ]
	do
		./test.out $photons_per_side $num_procs2 $dummy_i $spin_val $inc_val $numMdot $initMdot $finalMdot 1010 "${output_path}/${machine_name}_layer4_${dummy_i}" &
		pids+=($!)
		dummy_i=$((dummy_i+1))
	done
	wait ${pids[*]}

	dummy_j=0
	while [ $dummy_j -lt $numMdot ]
	do
		dummy_k=0
		pids=()
		while [ $dummy_k -lt $num_procs_py ] && [ $dummy_j -lt $numMdot ]
		do
			cat ${output_path}/${machine_name}_layer4_*_${dummy_j}.txt > ${output_path}/${machine_name}_cat4_${dummy_j}.txt &
			pids+=($!)
			dummy_j=$((dummy_j+1))
			dummy_k=$((dummy_k+1))
		done
		wait ${pids[*]}
	done

	rm -f ${output_path}/${machine_name}_layer4_*_*.txt

	dummy_j=0
	while [ $dummy_j -lt $numMdot ]
	do
		dummy_k=0
		pids=()
		while [ $dummy_k -lt $num_procs_py ] && [ $dummy_j -lt $numMdot ]
		do
			python txt_to_npy_diskimg.py ${output_path}/${machine_name}_cat4_${dummy_j}.txt ${output_path}/${machine_name}_cat4_${dummy_j}.npy &
			pids+=($!)
			dummy_k=$((dummy_k+1))
			dummy_j=$((dummy_j+1))
		done
		wait ${pids[*]}
	done

	rm -f ${output_path}/${machine_name}_cat4_*.txt

	dummy_j=0
	while [ $dummy_j -lt $numMdot ]
	do
		dummy_k=0
		pids=()
		while [ $dummy_k -lt $num_procs_py ] && [ $dummy_j -lt $numMdot ]
		do
			python transfer_auto_zoomin_v5.py $spin_val ${output_path}/${machine_name}_dr0p01_${dummy_j}.fits 4 $dR ${output_path}/${machine_name}_cat1_${dummy_j}.npy 30 ${output_path}/${machine_name}_cat2_${dummy_j}.npy 70 ${output_path}/${machine_name}_cat3_${dummy_j}.npy 210 ${output_path}/${machine_name}_cat4_${dummy_j}.npy 1010
			pids+=($!)
			dummy_k=$((dummy_k+1))
			dummy_j=$((dummy_j+1))
		done
		wait ${pids[*]}
	done

	rm -f ${output_path}/${machine_name}_cat1_*.npy
	rm -f ${output_path}/${machine_name}_cat2_*.npy
	rm -f ${output_path}/${machine_name}_cat3_*.npy
	rm -f ${output_path}/${machine_name}_cat4_*.npy

done < $param_file
