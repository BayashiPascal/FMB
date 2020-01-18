set terminal pngcairo enhanced font "arial,10" fontscale 1.0 size 600, 400 
set style data lines 
set style fill solid border -1
set xlabel "Ratio (nbPairInter/nbPair)"
set ylabel "Ratio (timeFMB / timeSAT)"
set logscale y 10
set yrange [0.01:100]
set datafile separator comma

set title "2D dynamic, Total, intersection only"
set output 'qualification2DTimeinter.png'
plot for [i=4:6] './qualification2DTime.txt' using i:xtic(1) title col
set title "2D dynamic, Total, no intersection only"
set output 'qualification2DTimenointer.png'
plot for [i=7:9] './qualification2DTime.txt' using i:xtic(1) title col
set title "2D dynamic, Total"
set output 'qualification2DTime.png'
plot for [i=10:12] './qualification2DTime.txt' using i:xtic(1) title col

set title "2D dynamic, Cuboid-Cuboid, intersection only"
set output 'qualification2DTimeCCinter.png'
plot for [i=4:6] 'qualification2DTimeCC.txt' using i:xtic(1) title col
set title "2D dynamic, Cuboid-Cuboid, no intersection only"
set output 'qualification2DTimeCCnointer.png'
plot for [i=7:9] 'qualification2DTimeCC.txt' using i:xtic(1) title col
set title "2D dynamic, Cuboid-Cuboid"
set output 'qualification2DTimeCC.png'
plot for [i=10:12] 'qualification2DTimeCC.txt' using i:xtic(1) title col

set title "2D dynamic, Cuboid-Tetrahedron, intersection only"
set output 'qualification2DTimeCTinter.png'
plot for [i=4:6] 'qualification2DTimeCT.txt' using i:xtic(1) title col
set title "2D dynamic, Cuboid-Tetrahedron, no intersection only"
set output 'qualification2DTimeCTnointer.png'
plot for [i=7:9] 'qualification2DTimeCT.txt' using i:xtic(1) title col
set title "2D dynamic, Cuboid-Tetrahedron"
set output 'qualification2DTimeCT.png'
plot for [i=10:12] 'qualification2DTimeCT.txt' using i:xtic(1) title col

set title "2D dynamic, Tetrahedron-Cuboid, intersection only"
set output 'qualification2DTimeTCinter.png'
plot for [i=4:6] 'qualification2DTimeTC.txt' using i:xtic(1) title col
set title "2D dynamic, Tetrahedron-Cuboid, no intersection only"
set output 'qualification2DTimeTCnointer.png'
plot for [i=7:9] 'qualification2DTimeTC.txt' using i:xtic(1) title col
set title "2D dynamic, Tetrahedron-Cuboid"
set output 'qualification2DTimeTC.png'
plot for [i=10:12] 'qualification2DTimeTC.txt' using i:xtic(1) title col

set title "2D dynamic, Tetrahedron-Tetrahedron, intersection only"
set output 'qualification2DTimeTTinter.png'
plot for [i=4:6] 'qualification2DTimeTT.txt' using i:xtic(1) title col
set title "2D dynamic, Tetrahedron-Tetrahedron, no intersection only"
set output 'qualification2DTimeTTnointer.png'
plot for [i=7:9] 'qualification2DTimeTT.txt' using i:xtic(1) title col
set title "2D dynamic, Tetrahedron-Tetrahedron"
set output 'qualification2DTimeTT.png'
plot for [i=10:12] 'qualification2DTimeTT.txt' using i:xtic(1) title col
