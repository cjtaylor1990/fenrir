#!/bin/bash
num_photons=25000
num_threads=4

param_file="./corona_test_parameter_list.txt"
machine_name="tyr"
output_path="/Users/cjtaylor/corona_test"

initThickness=20.0
finalThickness=0.0
numThickness=25

numRadBins=150

while INF=' ' read -ra line
do
	spin_val=${line[0]}
	height_val=${line[1]}
	echo ${line[0]} ${line[1]}

	dummy_i=0
	pids=()
	while [ $dummy_i -lt $num_threads ]
	do
		./test.out $num_photons $num_threads $dummy_i $spin_val $height_val $numThickness $initThickness $finalThickness "${output_path}/${machine_name}_corona_test" &
		pids+=($!)
		dummy_i=$((dummy_i+1))
	done
	wait ${pids[*]}

    dummy_j=0
    while [ $dummy_j -lt $numThickness ]
    do
        dummy_k=0
        pids=()
        while [ $dummy_k -lt $num_threads ] && [ $dummy_j -lt $numThickness ]
        do
            cat ${output_path}/${machine_name}_corona_test_${dummy_j}_* > ${output_path}/${machine_name}_corona_test_cat${dummy_j}.txt &
            pids+=($!)
            dummy_j=$((dummy_j+1))
            dummy_k=$((dummy_k+1))
        done
        wait ${pids[*]}
    done

    rm -f ${output_path}/${machine_name}_corona_test_*_*.txt

    dummy_j=0
    while [ $dummy_j -lt $numThickness ]
    do
        dummy_k=0
        pids=()
        while [ $dummy_k -lt $num_threads ] && [ $dummy_j -lt $numThickness ]
        do
            python txt_to_npy_corona.py ${output_path}/${machine_name}_corona_test_cat${dummy_j}.txt ${output_path}/${machine_name}_corona_test_cat${dummy_j}.npy &
            pids+=($!)
            dummy_k=$((dummy_k+1))
            dummy_j=$((dummy_j+1))
        done
        wait ${pids[*]}
    done

    rm -f ${output_path}/${machine_name}_corona_test_cat*.txt

	dummy_j=0
	while [ $dummy_j -lt $numThickness ]
	do
		dummy_k=0
		pids=()
		while [ $dummy_k -lt $num_threads ] && [ $dummy_j -lt $numThickness ]
		do
			python emissivity_calc_auto.py ${output_path}/ ${machine_name}_corona_test_cat ${machine_name}_corona_test_hist ${dummy_j} ${spin_val} ${height_val} ${numRadBins} &
			pids+=($!)
			dummy_k=$((dummy_k+1))
			dummy_j=$((dummy_j+1))
		done
		wait ${pids[*]}
	done

done < $param_file
