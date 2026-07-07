CC=gcc
SRC=main.c
CFLAGS= -lm $$(pkg-config --cflags --libs libpipewire-0.3)

all:
	$(CC) $(SRC) -o out $(CFLAGS)

clean:
	rm out
