#!/bin/bash
inPath="/Users/cjtaylor/fenrir_full_tests/tests"
outPath="/Users/cjtaylor/data"
linePath="${inPath}/fenrir_lp_direct_lines"


spinParamFile="${inPath}/full_lp_thickdisk_spin_params.txt"
heightParamFile="${inPath}/full_lp_thickdisk_height_params.txt"
inclinationParamFile="${inPath}/full_lp_thickdisk_inc_converted_params.txt"

numSpins=3
numInclinations=3
numHeights=11
numDiskThicknesses=11

HEADAS="/Users/cjtaylor/heasoft-6.21/x86_64-apple-darwin16.6.0"
. $HEADAS/headas-init.sh

spinIndex=0
while INF=' ' read -ra spinLine; do
	spin=${spinLine[0]}

	inclinationIndex=0
	while INF=' ' read -ra inclinationLine; do
		inclination=${inclinationLine[0]}

		heightIndex=0
		while INF=' ' read -ra heightLine; do
			height=${heightLine[0]}

			diskCombinationIndex=$((inclinationIndex + spinIndex*numInclinations))
			lpCombinationIndex=$((heightIndex + spinIndex*numHeights))

			diskThicknessIndex=0
			while [ $diskThicknessIndex -lt $numDiskThicknesses ]; do

				lineFile="${linePath}/line_disk${diskCombinationIndex}_lp${lpCombinationIndex}_thickness${diskThicknessIndex}.txt"
				cp ${lineFile} ./corbin.dat

				qdpFile="${outPath}/lp_direct_disk${diskCombinationIndex}_lp${lpCombinationIndex}_thickness${diskThicknessIndex}.qdp"
				txtFile="${outPath}/lp_direct_disk${diskCombinationIndex}_lp${lpCombinationIndex}_thickness${diskThicknessIndex}.txt"
				npyFile="${outPath}/lp_direct_disk${diskCombinationIndex}_lp${lpCombinationIndex}_thickness${diskThicknessIndex}.npy"
		
				xspec <<-EOF
				lmod relxill ./
				dummy 0.1 100.0 10000
				model relconv_lp(xillver)
				${height}
				${spin}
				${inclination}
				-1
				100
				0
				2
				2
				3.7
				300
				1.0
				0
				${inclination}
				-1
				1
				cpd /xs
				pl eem
				ipl
				wdata ${outPath}/lp_direct_disk${diskCombinationIndex}_lp${lpCombinationIndex}_thickness${diskThicknessIndex}
				exit
				exit
				EOF

				tail -n +4 ${qdpFile} > ${txtFile}
				rm -f ${qdpFile}
				python2 /Users/cjtaylor/fenrir_sparse_tests/txt_to_npy_models.py ${txtFile} ${npyFile}
				rm -f ${txtFile}
				
				diskThicknessIndex=$((diskThicknessIndex+1))
			
			done

			heightIndex=$((heightIndex+1))

		done < $heightParamFile

		inclinationIndex=$((inclinationIndex+1))
		
	done < $inclinationParamFile

	spinIndex=$((spinIndex+1))

done < $spinParamFile
