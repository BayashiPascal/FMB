set terminal pngcairo enhanced font "arial,10" fontscale 1.0 size 600, 400 
set style data lines 
set style fill solid border -1
set xlabel "Ratio NbInter/NbNoInter"
set ylabel "Ratio (timeFMB / timeSAT)"
set logscale y 10
set yrange [0.01:100]

set title "2D static, Total, intersection only"
set output 'qualification2Dinter.png'
plot for [i=4:6] '/dev/stdin' using i:xtic(1) title col
set title "2D static, Total, no intersection only"
set output 'qualification2Dnointer.png'
plot for [i=7:9] '/dev/stdin' using i:xtic(1) title col
set title "2D static, Total"
set output 'qualification2D.png'
plot for [i=10:12] '/dev/stdin' using i:xtic(1) title col

set title "2D static, Cuboid-Cuboid, intersection only"
set output 'qualification2DCCinter.png'
plot for [i=15:17] '/dev/stdin' using i:xtic(1) title col
set title "2D static, Cuboid-Cuboid, no intersection only"
set output 'qualification2DCCnointer.png'
plot for [i=18:20] '/dev/stdin' using i:xtic(1) title col
set title "2D static, Cuboid-Cuboid"
set output 'qualification2DCC.png'
plot for [i=21:23] '/dev/stdin' using i:xtic(1) title col

set title "2D static, Cuboid-Tetrahedron, intersection only"
set output 'qualification2DCTinter.png'
plot for [i=26:28] '/dev/stdin' using i:xtic(1) title col
set title "2D static, Cuboid-Tetrahedron, no intersection only"
set output 'qualification2DCTnointer.png'
plot for [i=29:31] '/dev/stdin' using i:xtic(1) title col
set title "2D static, Cuboid-Tetrahedron"
set output 'qualification2DCT.png'
plot for [i=32:34] '/dev/stdin' using i:xtic(1) title col

set title "2D static, Tetrahedron-Cuboid, intersection only"
set output 'qualification2DTCinter.png'
plot for [i=37:39] '/dev/stdin' using i:xtic(1) title col
set title "2D static, Tetrahedron-Cuboid, no intersection only"
set output 'qualification2DTCnointer.png'
plot for [i=40:42] '/dev/stdin' using i:xtic(1) title col
set title "2D static, Tetrahedron-Cuboid"
set output 'qualification2DTC.png'
plot for [i=43:45] '/dev/stdin' using i:xtic(1) title col

set title "2D static, Tetrahedron-Tetrahedron, intersection only"
set output 'qualification2DTTinter.png'
plot for [i=48:50] '/dev/stdin' using i:xtic(1) title col
set title "2D static, Tetrahedron-Tetrahedron, no intersection only"
set output 'qualification2DTTnointer.png'
plot for [i=51:53] '/dev/stdin' using i:xtic(1) title col
set title "2D static, Tetrahedron-Tetrahedron"
set output 'qualification2DTT.png'
plot for [i=54:56] '/dev/stdin' using i:xtic(1) title col
