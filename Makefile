all : main

BUILD_ARG=-Wall -O3
LINK_ARG=

main : main.o fmb.o frame.o
	gcc -o main main.o fmb.o frame.o $(LINK_ARG)

main.o : main.c fmb.h frame.h
	gcc -c main.c $(BUILD_ARG)

fmb.o : fmb.c fmb.h frame.h
	gcc -c fmb.c $(BUILD_ARG)

frame.o : frame.c frame.h
	gcc -c frame.c $(BUILD_ARG)

clean : 
	rm -f *.o main
