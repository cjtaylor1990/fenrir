machine="Loki"
lpDirPath="/Volumes/${machine}/fenrir_full_test/emissivity_files/"
lpParamFile="/Volumes/${machine}/fenrir_full_test/lp_param_file_test.txt"

numSpins=3
numHeights=11
numDiskThicknesses=11

paramCombinationIndex=0

spinIndex=0
while [ $spinIndex -lt $numSpins ]; do
    
    heightIndex=0
	while [ $heightIndex -lt $numHeights ]; do

        diskThicknessIndex=0
		while [ $diskThicknessIndex -lt $numDiskThicknesses ]; do
                
            mv "${lpDirPath}yorp19_em_test_hist${diskThicknessIndex}_${paramCombinationIndex}.npy" "${lpDirPath}em_hist_a${spinIndex}_h${heightIndex}_t${diskThicknessIndex}.npy"
            
        done

        paramCombinationIndex=$((paramCombinationIndex+1))
        heightIndex=$((heightIndex+1))
        
    done

    spinIndex=$((spinIndex+1))

done

