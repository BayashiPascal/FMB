set terminal pngcairo enhanced font "arial,10" fontscale 1.0 size 600, 400 
set style data lines 
set style fill solid border -1
set xlabel "Ratio (nbPairInter/nbPair)"
set ylabel "Ratio (timeFMB / timeSAT)"
set logscale y 10
set yrange [0.01:100]
set datafile separator comma

set title "3D static, Total, intersection only"
set output 'qualification3Dinter.png'
plot for [i=4:6] './qualification3D.txt' using i:xtic(1) title col
set title "3D static, Total, no intersection only"
set output 'qualification3Dnointer.png'
plot for [i=7:9] './qualification3D.txt' using i:xtic(1) title col
set title "3D static, Total"
set output 'qualification3D.png'
plot for [i=10:12] './qualification3D.txt' using i:xtic(1) title col

set title "3D static, Cuboid-Cuboid, intersection only"
set output 'qualification3DCCinter.png'
plot for [i=4:6] 'qualification3DCC.txt' using i:xtic(1) title col
set title "3D static, Cuboid-Cuboid, no intersection only"
set output 'qualification3DCCnointer.png'
plot for [i=7:9] 'qualification3DCC.txt' using i:xtic(1) title col
set title "3D static, Cuboid-Cuboid"
set output 'qualification3DCC.png'
plot for [i=10:12] 'qualification3DCC.txt' using i:xtic(1) title col

set title "3D static, Cuboid-Tetrahedron, intersection only"
set output 'qualification3DCTinter.png'
plot for [i=4:6] 'qualification3DCT.txt' using i:xtic(1) title col
set title "3D static, Cuboid-Tetrahedron, no intersection only"
set output 'qualification3DCTnointer.png'
plot for [i=7:9] 'qualification3DCT.txt' using i:xtic(1) title col
set title "3D static, Cuboid-Tetrahedron"
set output 'qualification3DCT.png'
plot for [i=10:12] 'qualification3DCT.txt' using i:xtic(1) title col

set title "3D static, Tetrahedron-Cuboid, intersection only"
set output 'qualification3DTCinter.png'
plot for [i=4:6] 'qualification3DTC.txt' using i:xtic(1) title col
set title "3D static, Tetrahedron-Cuboid, no intersection only"
set output 'qualification3DTCnointer.png'
plot for [i=7:9] 'qualification3DTC.txt' using i:xtic(1) title col
set title "3D static, Tetrahedron-Cuboid"
set output 'qualification3DTC.png'
plot for [i=10:12] 'qualification3DTC.txt' using i:xtic(1) title col

set title "3D static, Tetrahedron-Tetrahedron, intersection only"
set output 'qualification3DTTinter.png'
plot for [i=4:6] 'qualification3DTT.txt' using i:xtic(1) title col
set title "3D static, Tetrahedron-Tetrahedron, no intersection only"
set output 'qualification3DTTnointer.png'
plot for [i=7:9] 'qualification3DTT.txt' using i:xtic(1) title col
set title "3D static, Tetrahedron-Tetrahedron"
set output 'qualification3DTT.png'
plot for [i=10:12] 'qualification3DTT.txt' using i:xtic(1) title col
