### Makefile for a simple demo to demonstrate use of the INXLib library
### http://nobelware.com/RawCode/inxlib/inxlib.php
### Ioannis Nompelis 2017/05/25

CC = gcc
COPTS = -Wall -fPIC -O0
XLIBS = -lGL -lGLU -lX11

###### include paths (the INXLib files should live in here)
INCLUDE = -I ../

###### libraries
LIBS = $(XLIBS) -lm -ljpeg


all:
	$(CC) -c $(DEBUG) $(COPTS) $(INCLUDE) ../inxlib.c
	$(CC) -c $(DEBUG) $(COPTS) $(INCLUDE) ../inxfont.c
	$(CC) -c $(DEBUG) $(COPTS) $(INCLUDE) -D_CASE4_ inxlib_user.c
	$(CC) -c $(DEBUG) $(COPTS) $(INCLUDE) injpeg.c
	$(CC)    $(DEBUG) $(COPTS) $(INCLUDE) test.c injpeg.o \
           inxlib.o inxlib_user.o inxfont.o $(LIBS)

clean:
	rm -f *.o *.a a.out

