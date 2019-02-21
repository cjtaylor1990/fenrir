#!/bin/bash
photons_per_side=1000
num_procs=25
param_file="./param_file.txt"
output_path="/n/galaxy/cjtaylor/transfer_auto"
combine_output="$output_path/test.txt"

while INF=' ' read -ra line ; do
	spin_val=${line[0]}
	inc_val=${line[1]}
	echo "$spin_val $inc_val" 
done < $param_file