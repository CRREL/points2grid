CC=g++

# flags needed to compile sources
# ensure that your CPLUS_INCLUDE_PATH is set to pick up the required headers

# on Macs
# CFLAGS=-O3 -Wall -g -D_GNU_SOURCE -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -D_USE_LARGEFILE64 -Dfopen64=fopen

# on everything else
CFLAGS=-O3 -m64 -Wall -g -D_GNU_SOURCE -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -D_USE_LARGEFILE64

# ensure that your LIBRARY_PATH is set up to pick up these libraries
LIB=-llas -lm -lcurl

OBJS=main.o Interpolation.o InCoreInterp.o OutCoreInterp.o GridMap.o GridFile.o

all: points2grid

points2grid: $(OBJS)
	$(CC) $(CFLAGS) $(LIB) -o $@ $(OBJS)

.cpp.o:
	$(CC) $(CFLAGS) $(INC) -c $<

clean:
	rm -f points2grid *.o
