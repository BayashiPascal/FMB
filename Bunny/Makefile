all : allFmb video allSat

COMPILER?=gcc
OPTIMIZATION?=-O3
BUILD_ARG=$(OPTIMIZATION) -I../SAT -I../Frame

main_fmb : main_fmb.o fmb3d.o frame.o Makefile
	$(COMPILER) -o main_fmb main_fmb.o fmb3d.o frame.o -lm

main_fmb.o : main.c fmb3d.h ../Frame/frame.h Makefile
	$(COMPILER) -DMODE=0 -c main.c $(BUILD_ARG) && mv main.o main_fmb.o

main_sat : main_sat.o sat.o frame.o Makefile
	$(COMPILER) -o main_sat main_sat.o sat.o frame.o -lm

main_sat.o : main.c fmb3d.h ../Frame/frame.h Makefile
	$(COMPILER) -DMODE=1 -c main.c $(BUILD_ARG) && mv main.o main_sat.o

fmb3d.o : fmb3d.c fmb3d.h ../Frame/frame.h Makefile
	$(COMPILER) -c fmb3d.c $(BUILD_ARG)

sat.o : ../SAT/sat.c ../SAT/sat.h ../Frame/frame.h Makefile
	$(COMPILER) -c ../SAT/sat.c $(BUILD_ARG)

frame.o : ../Frame/frame.c ../Frame/frame.h Makefile
	$(COMPILER) -c ../Frame/frame.c $(BUILD_ARG)

clean : 
	rm -f *.o fmb sat

pov:
	povray +H600 +W800 +A -d bunny.pov

allFmb:
	make clean main_fmb; rm -f timeFMB.txt; { time main_fmb >> timeFMB.txt; } 1>&2 2>> timeFMB.txt; povray +H600 +W800 +A -d bunny.pov; cp bunny.png bunnyFMB.png

allSat:
	make clean main_sat; rm -f timeSAT.txt; { time main_sat >> timeSAT.txt; } 1>&2 2>> timeSAT.txt; povray +H600 +W800 +A -d bunny.pov; cp bunny.png bunnySAT.png

video: cleanVideo
	povray bunny.ini; ffmpeg -framerate 25 -i bunny%03d.png -c:v libx264 -profile:v high -crf 20 -pix_fmt yuv420p bunny.mp4; rm -f bunny[0-9][0-9][0-9].png

cleanVideo:
	rm -f bunny.mp4
