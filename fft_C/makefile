CC=gcc
SRCS=fft.c
CCFLAGS= -lfftw3 -lm -g
bin=Visualizer

all:
	$(CC) $(SRCS) -o  $(bin) $(CCFLAGS)

clean: 
	rm -f ./*.txt $(bin) ./*.json


