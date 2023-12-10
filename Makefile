DEBUG = -D_NO_DEBUG_ -D_NO_EVENT_DEBUG_
DEBUG += -D   _DEBUG_FONT_

###### C and C++ ######
CC = gcc
COPTS = -g -Wall -fPIC -O0
#-- the following needs to be defined at compile-time; too much to explain...
COPTS += -D GL_GLEXT_PROTOTYPES
XLIBS = -lGL -lGLU -lX11 -lstdc++ -ltiff -ljpeg
CXX = g++
CXXOPTS = -g -Wall -fPIC -O0
CXXOPTS += -D  _STRICT_CPP_

###### libraries ######
LIBS = $(XLIBS) -lm 


all:
	$(CC) -c $(DEBUG) $(COPTS) -Dno_OLDSTYLE_ inxlib.c
	$(CC) -c $(DEBUG) $(COPTS) -D_CASE1_ inxlib_user.c
	$(CC) -c $(DEBUG) $(COPTS) inogl.c
	$(CC)    $(DEBUG) $(COPTS) test.c inxlib.o inxlib_user.o inogl.o $(LIBS)

doc:
	doxygen Doxyfile

clean:
	rm -f *.o *.a a.out
	rm -Rf doxygen_doc


