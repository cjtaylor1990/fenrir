param_file="/Users/cjtaylor/sparse_grid/param_file_converted.txt"
output_path="/Users/cjtaylor/sparse_grid/relxill_lines"

heainit

comb_index=1
while INF=' ' read -ra line; do
	spin_val=${line[0]}
	inc_val=${line[1]}

	xspec <<EOF
	lmod relxill ./
	dummy 0.002 2.002 1000 linear
	model relline
	1
	3
	3
	30
	${spin_val}
	${inc_val}
	-1
	80
	0
	0
	1
	cpd /xs
	pl mod
	ipl
	wdata ${output_path}/relxill_model_${comb_index}
	exit
	exit
	EOF
	
	tail -n +4 ${output_path}/relxill_model_${comb_index}.qdp > ${output_path}/relxill_model_${comb_index}.txt
	rm -f ${output_path}/relxill_model_${comb_index}.qdp
	python /Users/cjtaylor/sparse_grid/txt_to_npy_models.py ${output_path}/relxill_model_${comb_index}.txt ${output_path}/relxill_model_${comb_index}.npy
	rm -f ${output_path}/relxill_model_${comb_index}.txt
	comb_index=$((comb_index+1))
done < $param_file
