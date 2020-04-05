set terminal pngcairo enhanced font "arial,10" fontscale 1.0 size 600, 400 
set style data lines 
set style fill solid border -1
set xlabel "Ratio (nbPairInter/nbPair)"
set ylabel "Ratio (timeFMB / timeSAT)"
set logscale y 10
set yrange [0.01:100]
set datafile separator comma

set title "3D static (near case only), Total, intersection only"
set output 'qualification3DinterNearCaseOnly.png'
plot for [i=4:6] './qualification3Dnearcaseonly.txt' using i:xtic(1) title col
set title "3D static (near case only), Total, no intersection only"
set output 'qualification3DnointerNearCaseOnly.png'
plot for [i=7:9] './qualification3Dnearcaseonly.txt' using i:xtic(1) title col
set title "3D static (near case only), Total"
set output 'qualification3DNearCaseOnly.png'
plot for [i=10:12] './qualification3Dnearcaseonly.txt' using i:xtic(1) title col

set title "3D static (near case only), Cuboid-Cuboid, intersection only"
set output 'qualification3DCCinterNearCaseOnly.png'
plot for [i=4:6] 'qualification3DCCnearcaseonly.txt' using i:xtic(1) title col
set title "3D static (near case only), Cuboid-Cuboid, no intersection only"
set output 'qualification3DCCnointerNearCaseOnly.png'
plot for [i=7:9] 'qualification3DCCnearcaseonly.txt' using i:xtic(1) title col
set title "3D static (near case only), Cuboid-Cuboid"
set output 'qualification3DCCNearCaseOnly.png'
plot for [i=10:12] 'qualification3DCCnearcaseonly.txt' using i:xtic(1) title col

set title "3D static (near case only), Cuboid-Tetrahedron, intersection only"
set output 'qualification3DCTinterNearCaseOnly.png'
plot for [i=4:6] 'qualification3DCTnearcaseonly.txt' using i:xtic(1) title col
set title "3D static (near case only), Cuboid-Tetrahedron, no intersection only"
set output 'qualification3DCTnointerNearCaseOnly.png'
plot for [i=7:9] 'qualification3DCTnearcaseonly.txt' using i:xtic(1) title col
set title "3D static (near case only), Cuboid-Tetrahedron"
set output 'qualification3DCTNearCaseOnly.png'
plot for [i=10:12] 'qualification3DCTnearcaseonly.txt' using i:xtic(1) title col

set title "3D static (near case only), Tetrahedron-Cuboid, intersection only"
set output 'qualification3DTCinterNearCaseOnly.png'
plot for [i=4:6] 'qualification3DTCnearcaseonly.txt' using i:xtic(1) title col
set title "3D static (near case only), Tetrahedron-Cuboid, no intersection only"
set output 'qualification3DTCnointerNearCaseOnly.png'
plot for [i=7:9] 'qualification3DTCnearcaseonly.txt' using i:xtic(1) title col
set title "3D static (near case only), Tetrahedron-Cuboid"
set output 'qualification3DTCNearCaseOnly.png'
plot for [i=10:12] 'qualification3DTCnearcaseonly.txt' using i:xtic(1) title col

set title "3D static (near case only), Tetrahedron-Tetrahedron, intersection only"
set output 'qualification3DTTinterNearCaseOnly.png'
plot for [i=4:6] 'qualification3DTTnearcaseonly.txt' using i:xtic(1) title col
set title "3D static (near case only), Tetrahedron-Tetrahedron, no intersection only"
set output 'qualification3DTTnointerNearCaseOnly.png'
plot for [i=7:9] 'qualification3DTTnearcaseonly.txt' using i:xtic(1) title col
set title "3D static (near case only), Tetrahedron-Tetrahedron"
set output 'qualification3DTTNearCaseOnly.png'
plot for [i=10:12] 'qualification3DTTnearcaseonly.txt' using i:xtic(1) title col
