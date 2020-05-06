all : main unitTests validation qualification

COMPILER?=gcc
OPTIMIZATION?=-O3
BUILD_ARG=$(OPTIMIZATION) -I../SAT -I../Frame
LINK_ARG=-lm

main : main.o fmb3dface.o frame.o sat.o Makefile
	$(COMPILER) -o main main.o fmb3dface.o frame.o sat.o

main.o : main.c fmb3dface.h ../Frame/frame.h Makefile
	$(COMPILER) -c main.c $(BUILD_ARG)

unitTests : unitTests.o fmb3dface.o frame.o sat.o Makefile
	$(COMPILER) -o unitTests unitTests.o fmb3dface.o frame.o sat.o $(LINK_ARG)

unitTests.o : unitTests.c fmb3dface.h ../Frame/frame.h Makefile
	$(COMPILER) -c unitTests.c $(BUILD_ARG)

validation : validation.o fmb3dface.o sat.o frame.o Makefile
	$(COMPILER) -o validation validation.o fmb3dface.o sat.o frame.o $(LINK_ARG)

validation.o : validation.c fmb3dface.h ../SAT/sat.h ../Frame/frame.h Makefile
	$(COMPILER) -c validation.c $(BUILD_ARG)

qualification : qualification.o fmb3dface.o sat.o frame.o Makefile
	$(COMPILER) -o qualification qualification.o fmb3dface.o sat.o frame.o $(LINK_ARG)

qualification.o : qualification.c fmb3dface.h ../SAT/sat.h ../Frame/frame.h Makefile
	$(COMPILER) -c qualification.c $(BUILD_ARG)

fmb3dface.o : fmb3dface.c fmb3dface.h ../Frame/frame.h Makefile
	$(COMPILER) -c fmb3dface.c $(BUILD_ARG)

sat.o : ../SAT/sat.c ../SAT/sat.h ../Frame/frame.h Makefile
	$(COMPILER) -c ../SAT/sat.c $(BUILD_ARG)

frame.o : ../Frame/frame.c ../Frame/frame.h Makefile
	$(COMPILER) -c ../Frame/frame.c $(BUILD_ARG)

clean : 
	rm -f *.o main unitTests validation qualification

valgrind :
	valgrind -v --track-origins=yes --leak-check=full \
	--gen-suppressions=yes --show-leak-kinds=all ./unitTests

cppcheck :
	cppcheck --enable=all ./

