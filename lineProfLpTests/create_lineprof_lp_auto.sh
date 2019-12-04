machine="Loki"

diskFilePath="/Volumes/${machine}/multi_grid_test_data/ongrid_test/"
lpFilePath="/Volumes/${machine}/fenrir_full_test/emissivity_files/"

outDirectory="/Volumes/${machine}/fenrir_full_test/lineprof_lp_files/"

spinParamFile
heightParamFile
inclinationParamFile

numSpins = 3
numLpHeights = 11
numDiskInclinations = 3
numDiskThicknesses = 11

diskCombinationIndex=0
lpCombinationIndex=0
while INF=' ' read -ra spinLine; do
	spin=${spinLine[0]}

	while INF=' ' read -ra inclinationLine; do
		inclination=${inclinationLine[0]}

		while INF =' ' read -ra heightLine; do
			height=${heightLine[0]}
	
			diskThicknessIndex=0
			while [ $diskThicknessIndex -lt $numDiskThicknesses ]; do
		
				python3 create_lineprof_lp_auto.py $spin_val  $filePath $inDirectory $outDirectory $comb_index $disk_index
				echo $comb_index $disk_index
				diskThicknessIndex=$((diskThicknessIndex+1))
			
			done 

		done < $heightParamFile
		
	done < $inclinationParamFile

done < $spinParamFile
