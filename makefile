CC=gcc
SRC=main.c
CFLAGS= -lm $$(pkg-config --cflags --libs libpipewire-0.3)
BIN=FFT_SINK

all:
	$(CC) $(SRC) -o $(BIN) $(CFLAGS)

clean:
	rm -f $(BIN) ./*.json ./pygraph/*.txt
