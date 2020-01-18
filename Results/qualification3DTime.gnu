set terminal pngcairo enhanced font "arial,10" fontscale 1.0 size 600, 400 
set style data lines 
set style fill solid border -1
set xlabel "Ratio (nbPairInter/nbPair)"
set ylabel "Ratio (timeFMB / timeSAT)"
set logscale y 10
set yrange [0.01:100]
set datafile separator comma

set title "3D static, Total, intersection only"
set output 'qualification3DTimeinter.png'
plot for [i=4:6] './qualification3DTime.txt' using i:xtic(1) title col
set title "3D static, Total, no intersection only"
set output 'qualification3DTimenointer.png'
plot for [i=7:9] './qualification3DTime.txt' using i:xtic(1) title col
set title "3D static, Total"
set output 'qualification3DTime.png'
plot for [i=10:12] './qualification3DTime.txt' using i:xtic(1) title col

set title "3D static, Cuboid-Cuboid, intersection only"
set output 'qualification3DTimeCCinter.png'
plot for [i=4:6] 'qualification3DTimeCC.txt' using i:xtic(1) title col
set title "3D static, Cuboid-Cuboid, no intersection only"
set output 'qualification3DTimeCCnointer.png'
plot for [i=7:9] 'qualification3DTimeCC.txt' using i:xtic(1) title col
set title "3D static, Cuboid-Cuboid"
set output 'qualification3DTimeCC.png'
plot for [i=10:12] 'qualification3DTimeCC.txt' using i:xtic(1) title col

set title "3D static, Cuboid-Tetrahedron, intersection only"
set output 'qualification3DTimeCTinter.png'
plot for [i=4:6] 'qualification3DTimeCT.txt' using i:xtic(1) title col
set title "3D static, Cuboid-Tetrahedron, no intersection only"
set output 'qualification3DTimeCTnointer.png'
plot for [i=7:9] 'qualification3DTimeCT.txt' using i:xtic(1) title col
set title "3D static, Cuboid-Tetrahedron"
set output 'qualification3DTimeCT.png'
plot for [i=10:12] 'qualification3DTimeCT.txt' using i:xtic(1) title col

set title "3D static, Tetrahedron-Cuboid, intersection only"
set output 'qualification3DTimeTCinter.png'
plot for [i=4:6] 'qualification3DTimeTC.txt' using i:xtic(1) title col
set title "3D static, Tetrahedron-Cuboid, no intersection only"
set output 'qualification3DTimeTCnointer.png'
plot for [i=7:9] 'qualification3DTimeTC.txt' using i:xtic(1) title col
set title "3D static, Tetrahedron-Cuboid"
set output 'qualification3DTimeTC.png'
plot for [i=10:12] 'qualification3DTimeTC.txt' using i:xtic(1) title col

set title "3D static, Tetrahedron-Tetrahedron, intersection only"
set output 'qualification3DTimeTTinter.png'
plot for [i=4:6] 'qualification3DTimeTT.txt' using i:xtic(1) title col
set title "3D static, Tetrahedron-Tetrahedron, no intersection only"
set output 'qualification3DTimeTTnointer.png'
plot for [i=7:9] 'qualification3DTimeTT.txt' using i:xtic(1) title col
set title "3D static, Tetrahedron-Tetrahedron"
set output 'qualification3DTimeTT.png'
plot for [i=10:12] 'qualification3DTimeTT.txt' using i:xtic(1) title col
