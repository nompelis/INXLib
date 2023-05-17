### Makefile for a simple game demo to demonstrate use of the INXLib library
### http://nobelware.com/RawCode/inxlib/inxlib.php
### Ioannis Nompelis 2016/09/10

CC = gcc
COPTS = -Wall -fPIC -O0
XLIBS = -lGL -lGLU -lX11

###### include paths (the INXLib files should live in here)
INCLUDE = -I ../

###### libraries
LIBS = $(XLIBS) -lm 


all:
	$(CC) -c $(DEBUG) $(COPTS) $(INCLUDE) ../inxlib.c
	$(CC) -c $(DEBUG) $(COPTS) $(INCLUDE) -D_CASE3_ inxlib_user.c
	$(CC) -c $(DEBUG) $(COPTS) $(INCLUDE) axissphere.c
	$(CC)    $(DEBUG) $(COPTS) $(INCLUDE) test.c axissphere.o \
           inxlib.o inxlib_user.o $(LIBS)

clean:
	rm -f *.o *.a a.out

