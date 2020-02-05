COMPILER=gcc
OPTIMIZATION=-O3

all : compile run plot dynamicAnalysis doc

install :
	sudo apt-get install gnuplot

compile : main unitTests validation qualification

main : main2D main2DTime main3D main3DTime

main2D:
	cd 2D; make main OPTIMIZATION=$(OPTIMIZATION); cd -

main2DTime:
	cd 2DTime; make main OPTIMIZATION=$(OPTIMIZATION); cd -

main3D:
	cd 3D; make main OPTIMIZATION=$(OPTIMIZATION); cd -

main3DTime:
	cd 3DTime; make main OPTIMIZATION=$(OPTIMIZATION); cd -

unitTests : unitTests2D unitTests2DTime unitTests3D unitTests3DTime

unitTests2D:
	cd 2D; make unitTests OPTIMIZATION=$(OPTIMIZATION); cd -

unitTests2DTime:
	cd 2DTime; make unitTests OPTIMIZATION=$(OPTIMIZATION); cd -

unitTests3D:
	cd 3D; make unitTests OPTIMIZATION=$(OPTIMIZATION); cd -

unitTests3DTime:
	cd 3DTime; make unitTests OPTIMIZATION=$(OPTIMIZATION); cd -

validation : validation2D validation2DTime validation3D validation3DTime

validation2D:
	cd 2D; make validation OPTIMIZATION=$(OPTIMIZATION); cd -

validation2DTime:
	cd 2DTime; make validation OPTIMIZATION=$(OPTIMIZATION); cd -

validation3D:
	cd 3D; make validation OPTIMIZATION=$(OPTIMIZATION); cd -

validation3DTime:
	cd 3DTime; make validation OPTIMIZATION=$(OPTIMIZATION); cd -

qualification : qualification2D qualification2DTime qualification3D qualification3DTime

qualification2D:
	cd 2D; make qualification OPTIMIZATION=$(OPTIMIZATION); cd -

qualification2DTime:
	cd 2DTime; make qualification OPTIMIZATION=$(OPTIMIZATION); cd -

qualification3D:
	cd 3D; make qualification OPTIMIZATION=$(OPTIMIZATION); cd -

qualification3DTime:
	cd 3DTime; make qualification OPTIMIZATION=$(OPTIMIZATION); cd -

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

cppcheck : cppcheck2D cppcheck2DTime cppcheck3D cppcheck3DTime

cppcheck2D:
	cd 2D; make cppcheck; cd -

cppcheck2DTime:
	cd 2DTime; make cppcheck; cd -

cppcheck3D:
	cd 3D; make cppcheck; cd -

cppcheck3DTime:
	cd 3DTime; make cppcheck; cd -

run : run2D run2DTime run3D run3DTime

run2D: 
	cd 2D; ./main > ../Results/main2D.txt; ./unitTests > ../Results/unitTests2D.txt; ./validation > ../Results/validation2D.txt; ./qualification; cd - 

run3D: 
	cd 3D; ./main > ../Results/main3D.txt; ./unitTests > ../Results/unitTests3D.txt; ./validation > ../Results/validation3D.txt; ./qualification; cd - 

run2DTime: 
	cd 2DTime; ./main > ../Results/main2DTime.txt; ./unitTests > ../Results/unitTests2DTime.txt; ./validation > ../Results/validation2DTime.txt; ./qualification; cd - 

run3DTime: 
	cd 3DTime; ./main > ../Results/main3DTime.txt; ./unitTests > ../Results/unitTests3DTime.txt; ./validation > ../Results/validation3DTime.txt; ./qualification; cd - 

plot: cleanPlot plot2D plot2DTime plot3D plot3DTime

cleanPlot:
	rm -f Results/*.png

plot2D:
	cd Results; gnuplot qualification2D.gnu; cd -

plot2DTime:
	cd Results; gnuplot qualification2DTime.gnu; cd -

plot3D:
	cd Results; gnuplot qualification3D.gnu; cd -

plot3DTime:
	cd Results; gnuplot qualification3DTime.gnu; cd -

doc:
	cd Doc; make latex; cd -

getRuntimeEnvironment:
	echo "uname -v\n" > runtimeEnv.txt; uname -v >> runtimeEnv.txt; echo "\n=============\n" >> runtimeEnv.txt; echo "lshw -short\n" >> runtimeEnv.txt; sudo lshw -short >> runtimeEnv.txt; echo "\n=============\n" >> runtimeEnv.txt; echo "lscpu\n" >> runtimeEnv.txt; lscpu >> runtimeEnv.txt; echo "\n=============\n" >> runtimeEnv.txt; echo "$(COMPILER) -v\n" >> runtimeEnv.txt; $(COMPILER) -v 1>> runtimeEnv.txt 2>> runtimeEnv.txt 

dynamicAnalysis:
	make valgrind 1> dynamicAnalysis.txt 2> dynamicAnalysis.txt

staticAnalysis:
	make cppcheck 1> staticAnalysis.txt 2> staticAnalysis.txt
