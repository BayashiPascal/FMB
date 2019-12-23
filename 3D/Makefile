all : main unitTests validation qualification

BUILD_ARG=-O3 -I../SAT/ -I../Frame/

main : main.o fmb3d.o ../Frame/frame.o Makefile
	gcc -o main main.o fmb3d.o ../Frame/frame.o

main.o : main.c fmb3d.h ../Frame/frame.h Makefile
	gcc -c main.c $(BUILD_ARG)

unitTests : unitTests.o fmb3d.o ../Frame/frame.o Makefile
	gcc -o unitTests unitTests.o fmb3d.o ../Frame/frame.o $(LINK_ARG)

unitTests.o : unitTests.c fmb3d.h ../Frame/frame.h Makefile
	gcc -c unitTests.c $(BUILD_ARG)

validation : validation.o fmb3d.o ../SAT/sat.o ../Frame/frame.o Makefile
	gcc -o validation validation.o fmb3d.o ../SAT/sat.o ../Frame/frame.o

validation.o : validation.c fmb3d.h ../SAT/sat.h ../Frame/frame.h Makefile
	gcc -c validation.c $(BUILD_ARG)

qualification : qualification.o fmb3d.o ../SAT/sat.o ../Frame/frame.o Makefile
	gcc -o qualification qualification.o fmb3d.o ../SAT/sat.o ../Frame/frame.o

qualification.o : qualification.c fmb3d.h ../SAT/sat.h ../Frame/frame.h Makefile
	gcc -c qualification.c $(BUILD_ARG)

fmb3d.o : fmb3d.c fmb3d.h ../Frame/frame.h Makefile
	gcc -c fmb3d.c $(BUILD_ARG)

../SAT/sat.o : Makefile
	cd ../SAT/; make sat.o; cd -

../Frame/frame.o : Makefile
	cd ../Frame/; make frame.o; cd -

clean : 
	rm -f *.o main unitTests validation qualification

valgrind :
	valgrind -v --track-origins=yes --leak-check=full \
	--gen-suppressions=yes --show-leak-kinds=all ./main

