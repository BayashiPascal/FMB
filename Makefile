all : main unitTests validation qualification

BUILD_ARG=-O3

main : main.o fmb.o sat.o frame.o Makefile
	gcc -o main main.o fmb.o sat.o frame.o

main.o : main.c fmb.h frame.h Makefile
	gcc -c main.c $(BUILD_ARG)

unitTests : unitTests.o fmb.o frame.o Makefile
	gcc -o unitTests unitTests.o fmb.o frame.o $(LINK_ARG)

unitTests.o : unitTests.c fmb.h frame.h Makefile
	gcc -c unitTests.c $(BUILD_ARG)

validation : validation.o fmb.o sat.o frame.o Makefile
	gcc -o validation validation.o fmb.o sat.o frame.o $(LINK_ARG)

validation.o : validation.c fmb.h sat.h frame.h Makefile
	gcc -c validation.c $(BUILD_ARG)

qualification : qualification.o fmb.o sat.o frame.o Makefile
	gcc -o qualification qualification.o fmb.o sat.o frame.o $(LINK_ARG)

qualification.o : qualification.c fmb.h sat.h frame.h Makefile
	gcc -c qualification.c $(BUILD_ARG)

fmb.o : fmb.c fmb.h frame.h Makefile
	gcc -c fmb.c $(BUILD_ARG)

sat.o : sat.c sat.h frame.h Makefile
	gcc -c sat.c $(BUILD_ARG)

frame.o : frame.c frame.h Makefile
	gcc -c frame.c $(BUILD_ARG)

clean : 
	rm -f *.o main unitTests validation qualification

latex:
	pdflatex -synctex=1 -interaction=nonstopmode -shell-escape  fmb.tex

valgrind :
	valgrind -v --track-origins=yes --leak-check=full \
	--gen-suppressions=yes --show-leak-kinds=all ./main

