CC=gcc
CFLAGS=-I.
DEPS = hellomake.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

diseaseMonitor: diseaseMonitor.o functions.o 
	$(CC) -o diseaseMonitor diseaseMonitor.o functions.o