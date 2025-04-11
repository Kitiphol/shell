CC=gcc
CFLAGS=-Wall -g 
BINARY=icsh

all: icsh

icsh: src/icsh.cpp
	$(CC) -o $(BINARY) $(CFLAGS) $<

.PHONY: clean

clean:
	rm -f $(BINARY)
