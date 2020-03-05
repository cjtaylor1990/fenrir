lmod relxill ./
dummy 0.1 100.0 10000
cpd /xs
set inPath "/Users/cjtaylor/fenrir_full_tests/tests"
set outPath "/Users/cjtaylor/data"
set spinFilePath "${inPath}/full_lp_thickdisk_spin_params.txt"
set heightFilePath "${inPath}/full_lp_thickdisk_height_params.txt"
set inclinationFilePath "${inPath}/full_lp_thickdisk_inc_converted_params.txt"
set diskThicknessFilePath "${inPath}/full_lp_thickdisk_thickness_params.txt"
set numInclinations 3
set numHeights 11               
set spinFile [open ${spinFilePath} r]
set spinIndex 0
while {[gets ${spinFile} spin]>=0} {
    set inclinationFile [open ${inclinationFilePath} r]
    set inclinationIndex 0
    while {[gets ${inclinationFile} inclination]>=0} {
        set heightFile [open ${heightFilePath} r]
        set heightIndex 0
        while {[gets ${heightFile} height]>=0} {
            set diskCombinationIndex [expr ${inclinationIndex} + ${spinIndex}*${numInclinations}]
            set lpCombinationIndex [expr ${heightIndex} + ${spinIndex}*${numHeights}]
            set diskThicknessFile [open ${diskThicknessFilePath} r]
            set diskThicknessIndex 0
            while {[gets ${diskThicknessFile} diskThickness]>=0} {
                set outFile "${outPath}/lp_grid_disk${diskCombinationIndex}_lp${lpCombinationIndex}_thickness${diskThicknessIndex}"
                model fenconv_lp(xillver)
                ${height}
                ${spin}
                ${inclination}
                ${diskThickness}
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
                pl eem
                inc diskThicknessIndex
            }                          
            close ${diskThicknessFile}
            inc heightIndex
        }    
        close ${heightFile}
        inc inclinationIndex
    }
    close ${inclinationFile}
    inc spinIndex
}
close ${spinFile}
