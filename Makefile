DEBUG = -D_NO_DEBUG_ -D_NO_EVENT_DEBUG_
DEBUG += -D   _DEBUG_FONT_

###### C and C++ ######
CC = gcc
COPTS = -g -Wall -fPIC -O0
#-- the following needs to be defined at compile-time; too much to explain...
COPTS += -D GL_GLEXT_PROTOTYPES
XLIBS = -lGL -lGLU -lX11
CXX = g++
CXXOPTS = -g -Wall -fPIC -O0
CXXOPTS += -D  _STRICT_CPP_

###### libraries ######
LIBS = $(XLIBS) -lm -lpthread


all: lib
	$(CC)    $(DEBUG) $(COPTS) test.c -ldl

lib:
	$(CC) -c $(DEBUG) $(COPTS) -Dno_OLDSTYLE_ inxlib.c
	$(CC) -c $(DEBUG) $(COPTS) -Dno_NO_GLX_WIN_ -D_CASE3_ inxlib_user.c
	$(CC) -c $(DEBUG) $(COPTS) inogl.c
	$(CC) -c $(DEBUG) $(COPTS) inxlib_gui.c
	$(CC) -shared -Wl,-soname,libINXlib.so -o libINXlib.so \
              inxlib_gui.o inxlib.o inxlib_user.o inogl.o $(LIBS)
	$(CC)    $(DEBUG) $(COPTS) test.c -ldl

doc:
	doxygen Doxyfile

clean:
	rm -f *.o *.a *.so a.out
	rm -Rf doxygen_doc


