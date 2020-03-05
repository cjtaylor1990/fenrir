set numHeights 11
set numSpins 3

set aIndex 0
set a [open full_lp_thickdisk_spin_params.txt r]
while {[gets $a line1]>=0} {
    set bIndex 0
    set b [open full_lp_thickdisk_height_params.txt r]
    while {[gets $b line2]>=0} {
        echo $line1 $line2 $aIndex $bIndex [expr $aIndex*$numHeights + $bIndex]
        inc bIndex
    }
    close $b
    inc aIndex
}
close $a