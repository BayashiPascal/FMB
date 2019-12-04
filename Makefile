all : main unitTests validation qualification

BUILD_ARG=-O3
BUILD_ARG_=-ggdb

main : main.o fmb2d.o fmb3d.o fmb2dt.o fmb3dt.o sat.o frame.o Makefile
	gcc -o main main.o fmb2d.o fmb3d.o fmb2dt.o fmb3dt.o sat.o frame.o

main.o : main.c fmb2d.h fmb3d.h fmb2dt.h fmb3dt.h frame.h Makefile
	gcc -c main.c $(BUILD_ARG)

unitTests : unitTests.o fmb2d.o fmb3d.o fmb2dt.o fmb3dt.o frame.o Makefile
	gcc -o unitTests unitTests.o fmb2d.o fmb3d.o fmb2dt.o fmb3dt.o frame.o $(LINK_ARG)

unitTests.o : unitTests.c fmb2d.h fmb3d.h fmb2dt.h fmb3dt.h frame.h Makefile
	gcc -c unitTests.c $(BUILD_ARG)

validation : validation.o fmb2d.o fmb3d.o fmb2dt.o fmb3dt.o sat.o frame.o Makefile
	gcc -o validation validation.o fmb2d.o fmb3d.o fmb2dt.o fmb3dt.o sat.o frame.o $(LINK_ARG)

validation.o : validation.c fmb2d.h fmb3d.h fmb2dt.h fmb3dt.h sat.h frame.h Makefile
	gcc -c validation.c $(BUILD_ARG)

qualification : qualification.o fmb2d.o fmb3d.o fmb2dt.o fmb3dt.o sat.o frame.o Makefile
	gcc -o qualification qualification.o fmb2d.o fmb3d.o fmb2dt.o fmb3dt.o sat.o frame.o $(LINK_ARG)

qualification.o : qualification.c fmb2d.h fmb3d.h fmb2dt.h fmb3dt.h sat.h frame.h Makefile
	gcc -c qualification.c $(BUILD_ARG)

fmb2d.o : fmb2d.c fmb2d.h frame.h Makefile
	gcc -c fmb2d.c $(BUILD_ARG)

fmb3d.o : fmb3d.c fmb3d.h frame.h Makefile
	gcc -c fmb3d.c $(BUILD_ARG)

fmb2dt.o : fmb2dt.c fmb2dt.h frame.h Makefile
	gcc -c fmb2dt.c $(BUILD_ARG)

fmb3dt.o : fmb3dt.c fmb3dt.h frame.h Makefile
	gcc -c fmb3dt.c $(BUILD_ARG)

sat.o : sat.c sat.h frame.h Makefile
	gcc -c sat.c $(BUILD_ARG)

frame.o : frame.c frame.h Makefile
	gcc -c frame.c $(BUILD_ARG)

clean : 
	rm -f *.o main unitTests validation qualification

valgrind :
	valgrind -v --track-origins=yes --leak-check=full \
	--gen-suppressions=yes --show-leak-kinds=all ./main

