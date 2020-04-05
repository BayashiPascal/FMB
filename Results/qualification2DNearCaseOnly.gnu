set terminal pngcairo enhanced font "arial,10" fontscale 1.0 size 600, 400 
set style data lines 
set style fill solid border -1
set xlabel "Ratio (nbPairInter/nbPair)"
set ylabel "Ratio (timeFMB / timeSAT)"
set logscale y 10
set yrange [0.01:100]
set datafile separator comma

set title "2D static (near case only), Total, intersection only"
set output 'qualification2DinterNearCaseOnly.png'
plot for [i=4:6] './qualification2Dnearcaseonly.txt' using i:xtic(1) title col
set title "2D static (near case only), Total, no intersection only"
set output 'qualification2DnointerNearCaseOnly.png'
plot for [i=7:9] './qualification2Dnearcaseonly.txt' using i:xtic(1) title col
set title "2D static (near case only), Total"
set output 'qualification2DNearCaseOnly.png'
plot for [i=10:12] './qualification2Dnearcaseonly.txt' using i:xtic(1) title col

set title "2D static (near case only), Cuboid-Cuboid, intersection only"
set output 'qualification2DCCinterNearCaseOnly.png'
plot for [i=4:6] 'qualification2DCCnearcaseonly.txt' using i:xtic(1) title col
set title "2D static (near case only), Cuboid-Cuboid, no intersection only"
set output 'qualification2DCCnointerNearCaseOnly.png'
plot for [i=7:9] 'qualification2DCCnearcaseonly.txt' using i:xtic(1) title col
set title "2D static (near case only), Cuboid-Cuboid"
set output 'qualification2DCCNearCaseOnly.png'
plot for [i=10:12] 'qualification2DCCnearcaseonly.txt' using i:xtic(1) title col

set title "2D static (near case only), Cuboid-Tetrahedron, intersection only"
set output 'qualification2DCTinterNearCaseOnly.png'
plot for [i=4:6] 'qualification2DCTnearcaseonly.txt' using i:xtic(1) title col
set title "2D static (near case only), Cuboid-Tetrahedron, no intersection only"
set output 'qualification2DCTnointerNearCaseOnly.png'
plot for [i=7:9] 'qualification2DCTnearcaseonly.txt' using i:xtic(1) title col
set title "2D static (near case only), Cuboid-Tetrahedron"
set output 'qualification2DCTNearCaseOnly.png'
plot for [i=10:12] 'qualification2DCTnearcaseonly.txt' using i:xtic(1) title col

set title "2D static (near case only), Tetrahedron-Cuboid, intersection only"
set output 'qualification2DTCinterNearCaseOnly.png'
plot for [i=4:6] 'qualification2DTCnearcaseonly.txt' using i:xtic(1) title col
set title "2D static (near case only), Tetrahedron-Cuboid, no intersection only"
set output 'qualification2DTCnointerNearCaseOnly.png'
plot for [i=7:9] 'qualification2DTCnearcaseonly.txt' using i:xtic(1) title col
set title "2D static (near case only), Tetrahedron-Cuboid"
set output 'qualification2DTCNearCaseOnly.png'
plot for [i=10:12] 'qualification2DTCnearcaseonly.txt' using i:xtic(1) title col

set title "2D static (near case only), Tetrahedron-Tetrahedron, intersection only"
set output 'qualification2DTTinterNearCaseOnly.png'
plot for [i=4:6] 'qualification2DTTnearcaseonly.txt' using i:xtic(1) title col
set title "2D static (near case only), Tetrahedron-Tetrahedron, no intersection only"
set output 'qualification2DTTnointerNearCaseOnly.png'
plot for [i=7:9] 'qualification2DTTnearcaseonly.txt' using i:xtic(1) title col
set title "2D static (near case only), Tetrahedron-Tetrahedron"
set output 'qualification2DTTNearCaseOnly.png'
plot for [i=10:12] 'qualification2DTTnearcaseonly.txt' using i:xtic(1) title col
