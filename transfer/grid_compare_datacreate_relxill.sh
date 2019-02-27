param_file="/Users/cjtaylor/sparse_grid/param_file_converted.txt"
output_path="/Users/cjtaylor/sparse_grid/relxill_models"

heainit

comb_index=1
while INF=' ' read -ra line; do
	spin_val=${line[0]}
	inc_val=${line[1]}

	xspec <<EOF
	lmod relxill ./
	dummy 0.1 100 1000
	model relconv(xillver)
	3
	3
	30
	${spin_val}
	${inc_val}
	-1
	80
	0
	2
	3.7
	300
	0
	0
	${inc_val}
	-1
	1.0
	cpd /xs
	pl eemod
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
