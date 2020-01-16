COMPILER=gcc
OPTIMIZATION=-O3

all : compile run plot dynamicAnalysis doc

install :
	sudo apt-get install gnuplot

compile : main unitTests validation qualification

main : main2D main2DTime main3D main3DTime

main2D:
	cd 2D; make main; cd -

main2DTime:
	cd 2DTime; make main; cd -

main3D:
	cd 3D; make main; cd -

main3DTime:
	cd 3DTime; make main; cd -

unitTests : unitTests2D unitTests2DTime unitTests3D unitTests3DTime

unitTests2D:
	cd 2D; make unitTests; cd -

unitTests2DTime:
	cd 2DTime; make unitTests; cd -

unitTests3D:
	cd 3D; make unitTests; cd -

unitTests3DTime:
	cd 3DTime; make unitTests; cd -

validation : validation2D validation2DTime validation3D validation3DTime

validation2D:
	cd 2D; make validation; cd -

validation2DTime:
	cd 2DTime; make validation; cd -

validation3D:
	cd 3D; make validation; cd -

validation3DTime:
	cd 3DTime; make validation; cd -

qualification : qualification2D qualification2DTime qualification3D qualification3DTime

qualification2D:
	cd 2D; make qualification; cd -

qualification2DTime:
	cd 2DTime; make qualification; cd -

qualification3D:
	cd 3D; make qualification; cd -

qualification3DTime:
	cd 3DTime; make qualification; cd -

clean : clean2D clean2DTime clean3D clean3DTime

clean2D:
	cd 2D; make clean; cd -

clean2DTime:
	cd 2DTime; make clean; cd -

clean3D:
	cd 3D; make clean; cd -

clean3DTime:
	cd 3DTime; make clean; cd -

valgrind : valgrind2D valgrind2DTime valgrind3D valgrind3DTime

valgrind2D:
	cd 2D; make valgrind; cd -

valgrind2DTime:
	cd 2DTime; make valgrind; cd -

valgrind3D:
	cd 3D; make valgrind; cd -

valgrind3DTime:
	cd 3DTime; make valgrind; cd -

run : run2D run2DTime run3D run3DTime

run2D: 
	cd 2D; ./main > ../Results/main2D.txt; ./unitTests > ../Results/unitTests2D.txt; ./validation > ../Results/validation2D.txt; grep failed ../Results/validation2D.txt; ./qualification; grep failed ../Results/qualification2D.txt; cd - 

run3D: 
	cd 3D; ./main > ../Results/main3D.txt; ./unitTests > ../Results/unitTests3D.txt; ./validation > ../Results/validation3D.txt; grep failed ../Results/validation3D.txt; ./qualification > ../Results/qualification3D.txt; grep failed ../Results/qualification3D.txt; cd - 

run2DTime: 
	cd 2DTime; ./main > ../Results/main2DTime.txt; ./unitTests > ../Results/unitTests2DTime.txt; ./validation > ../Results/validation2DTime.txt; grep failed ../Results/validation2DTime.txt; ./qualification > ../Results/qualification2DTime.txt; grep failed ../Results/qualification2DTime.txt; cd - 

run3DTime: 
	cd 3DTime; ./main > ../Results/main3DTime.txt; ./unitTests > ../Results/unitTests3DTime.txt; ./validation > ../Results/validation3DTime.txt; grep failed ../Results/validation3DTime.txt; ./qualification > ../Results/qualification3DTime.txt; grep failed ../Results/qualification3DTime.txt; cd - 

plot: cleanPlot plot2D plot2DTime plot3D plot3DTime

cleanPlot:
	rm Results/*.png

plot2D:
	cd Results; gnuplot qualification2D.gnu; cd -

plot2DTime:
	cd Results; gnuplot qualification2DTime.gnu < qualification2DTime.txt; cd -

plot3D:
	cd Results; gnuplot qualification3D.gnu < qualification3D.txt; cd -

plot3DTime:
	cd Results; gnuplot qualification3DTime.gnu < qualification3DTime.txt; cd -

doc:
	cd Doc; make latex; cd -

getRuntimeEnvironment:
	echo "uname -v\n" > runtimeEnv.txt; uname -v >> runtimeEnv.txt; echo "\n=============\n" >> runtimeEnv.txt; echo "lshw -short\n" >> runtimeEnv.txt; sudo lshw -short >> runtimeEnv.txt; echo "\n=============\n" >> runtimeEnv.txt; echo "lscpu\n" >> runtimeEnv.txt; lscpu >> runtimeEnv.txt; echo "\n=============\n" >> runtimeEnv.txt; echo "$(COMPILER) -v\n" >> runtimeEnv.txt; $(COMPILER) -v 1>> runtimeEnv.txt 2>> runtimeEnv.txt 

dynamicAnalysis:
	make valgrind 1> dynamicAnalysis.txt 2> dynamicAnalysis.txt
