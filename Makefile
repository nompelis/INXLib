DEBUG = -D_NO_DEBUG_ -D_NO_EVENT_DEBUG_

###### C
CC = gcc
COPTS = -Wall -fPIC -O0
XLIBS = -lGL -lGLU -lX11

###### libraries
LIBS = $(XLIBS) -lm 


all:
	$(CC) -c $(DEBUG) $(COPTS) inxlib.c
	$(CC) -c $(DEBUG) $(COPTS) -D_CASE1_ inxlib_user.c
	$(CC) -c $(DEBUG) $(COPTS) inxfont.c
	$(CC)    $(DEBUG) $(COPTS) test.c inxlib.o inxlib_user.o inxfont.o  $(LIBS)

doc:
	doxygen Doxyfile

clean:
	rm -f *.o *.a a.out
	rm -Rf doxygen_doc


