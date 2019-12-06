machine="Loki"
diskDirPath="/Volumes/${machine}/multi_grid_test_data/ongrid_test/"

numSpins=3
numInclinations=3
numDiskThicknesses=11

spinIndex=0
while [ $spinIndex -lt $numSpins ]; do
    
    inclinationIndex=0
	while [ $inclinationIndex -lt $numInclinations ]; do

        diskThicknessIndex=0
		while [ $diskThicknessIndex -lt $numDiskThicknesses ]; do

            diskThicknessIndex=$((diskThicknessIndex+1))
        done

        inclinationIndex=$((inclinationIndex+1))

    done

    spinIndex=$((spinIndex+1))

done