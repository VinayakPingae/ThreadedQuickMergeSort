TARGET=threaded_msort mygetenv.o
CFLAGS=-Wall -pthread
CC=gcc

all: $(TARGET)
	
threaded_msort: threaded_msort.c common.h utility.o
	$(CC) $(CFLAGS) threaded_msort.c utility.o -o threaded_msort
		
utility.o: utility.c common.h
	$(CC) $(CFLAGS) -c $*.c
	
mygetenv.o: mygetenv.c
	$(CC) $(CFLAGS) -c $*.c	
	
clean:
	rm -rf *.o *.a $(TARGET)
