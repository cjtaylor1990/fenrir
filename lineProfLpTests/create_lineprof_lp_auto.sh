machine="Loki"

diskFilePath="/Volumes/${machine}/multi_grid_test_data/ongrid_test/"
lpFilePath="/Volumes/${machine}/fenrir_full_test/emissivity_files/"

outDirPath="/Volumes/${machine}/fenrir_full_test/lineprof_lp_files/"

spinParamFile="/Volumes/${machine}/fenrir_full_test/full_spin_params.txt"
heightParamFile="/Volumes/${machine}/fenrir_full_test/full_height_params.txt"
inclinationParamFile="/Volumes/${machine}/fenrir_full_test/full_inc_params.txt"

numDiskThicknesses = 11

spinIndex=0
while INF=' ' read -ra spinLine; do
	spin=${spinLine[0]}

	while INF=' ' read -ra inclinationLine; do
		inclination=${inclinationLine[0]}

		while INF =' ' read -ra heightLine; do
			height=${heightLine[0]}
	
			diskThicknessIndex=0
			while [ $diskThicknessIndex -lt $numDiskThicknesses ]; do
		
				python3 create_lineprof_lp_auto.py $spin $spinIndex $inclinationIndex $heightIndex $diskThicknessIndex $diskFilePath $lpFilePath $outDirPath
				echo $spinIndex $inclinationIndex $heightIndex $diskThicknessIndex
				
				diskThicknessIndex=$((diskThicknessIndex+1))
			
			done

			heightIndex=$((heightIndex+1))

		done < $heightParamFile

		inclinationIndex=$((inclinationIndex+1))
		
	done < $inclinationParamFile

	spinIndex=$((spinIndex+1))

done < $spinParamFile
