set terminal pngcairo enhanced font "arial,10" fontscale 1.0 size 600, 400 
set style data lines 
set style fill solid border -1
set xlabel "Ratio (nbPairInter/nbPair)"
set ylabel "Ratio (timeFMB / timeSAT)"
set logscale y 10
set yrange [0.01:100]

set title "3D dynamic, Total, intersection only"
set output 'qualification3DTimeinter.png'
plot for [i=4:6] '/dev/stdin' using i:xtic(1) title col
set title "3D dynamic, Total, no intersection only"
set output 'qualification3DTimenointer.png'
plot for [i=7:9] '/dev/stdin' using i:xtic(1) title col
set title "3D dynamic, Total"
set output 'qualification3DTime.png'
plot for [i=10:12] '/dev/stdin' using i:xtic(1) title col

set title "3D dynamic, Cuboid-Cuboid, intersection only"
set output 'qualification3DTimeCCinter.png'
plot for [i=15:17] '/dev/stdin' using i:xtic(1) title col
set title "3D dynamic, Cuboid-Cuboid, no intersection only"
set output 'qualification3DTimeCCnointer.png'
plot for [i=18:20] '/dev/stdin' using i:xtic(1) title col
set title "3D dynamic, Cuboid-Cuboid"
set output 'qualification3DTimeCC.png'
plot for [i=21:23] '/dev/stdin' using i:xtic(1) title col

set title "3D dynamic, Cuboid-Tetrahedron, intersection only"
set output 'qualification3DTimeCTinter.png'
plot for [i=26:28] '/dev/stdin' using i:xtic(1) title col
set title "3D dynamic, Cuboid-Tetrahedron, no intersection only"
set output 'qualification3DTimeCTnointer.png'
plot for [i=29:31] '/dev/stdin' using i:xtic(1) title col
set title "3D dynamic, Cuboid-Tetrahedron"
set output 'qualification3DTimeCT.png'
plot for [i=32:34] '/dev/stdin' using i:xtic(1) title col

set title "3D dynamic, Tetrahedron-Cuboid, intersection only"
set output 'qualification3DTimeTCinter.png'
plot for [i=37:39] '/dev/stdin' using i:xtic(1) title col
set title "3D dynamic, Tetrahedron-Cuboid, no intersection only"
set output 'qualification3DTimeTCnointer.png'
plot for [i=40:42] '/dev/stdin' using i:xtic(1) title col
set title "3D dynamic, Tetrahedron-Cuboid"
set output 'qualification3DTimeTC.png'
plot for [i=43:45] '/dev/stdin' using i:xtic(1) title col

set title "3D dynamic, Tetrahedron-Tetrahedron, intersection only"
set output 'qualification3DTimeTTinter.png'
plot for [i=48:50] '/dev/stdin' using i:xtic(1) title col
set title "3D dynamic, Tetrahedron-Tetrahedron, no intersection only"
set output 'qualification3DTimeTTnointer.png'
plot for [i=51:53] '/dev/stdin' using i:xtic(1) title col
set title "3D dynamic, Tetrahedron-Tetrahedron"
set output 'qualification3DTimeTT.png'
plot for [i=54:56] '/dev/stdin' using i:xtic(1) title col
