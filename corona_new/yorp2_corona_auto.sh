#!/bin/bash
num_photons=30000
num_threads=30

param_file="./yorp2_corona_full_params.txt"
machine_name="yorp2"
output_path="/yorp2a/cjtaylor"

initThickness=20.0
finalThickness=0.0
numThickness=25

numRadBins=150

numHeights=100

heightIndex=1

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
			python emissivity_calc_auto.py ${output_path}/ ${machine_name}_corona_test_cat ${machine_name}_corona_test_hist $dummy_j $spin_val $height_val $numRadBins &
			pids+=($!)
			dummy_k=$((dummy_k+1))
			dummy_j=$((dummy_j+1))
		done
		wait ${pids[*]}
	done

	rm -f ${output_path}/${machine_name}_corona_test_cat*.npy

	python lp_fits_auto_v1.py ${spin_val} ${height_val} ${numThickness} ${output_path}/ ${machine_name}_corona_test_hist test_ ${heightIndex}

	rm -f ${output_path}/${machine_name}_corona_test_hist*.npy

	heightIndex=$((heightIndex+1))

	if [ $heightIndex -gt $numHeights ]
	then
		heightIndex=1
	fi

	echo done
done < $param_file

python lp_fits_combine_auto_v1.py ${output_path}/ test_ ${numHeights} ${initThickness} ${finalThickness} ${numThickness} ${param_file} ${machine_name}_combine.fits
