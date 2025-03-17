// A driver that simply loads the GUI execution code; the idea is that this
// program does not have to be linked against any graphics libraries, but that
// it will load a shared object file explicitly and invoke its entry point,
// and that library upon loading is supposed to find all libraries.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dlfcn.h>

#include "inxlib.h"


int main( int argc, char *argv[] )
{
   static struct my_xwin_vars xvars;

   void* handle = dlopen( "./libINXlib.so", RTLD_NOW );
   if( handle == NULL ) {
      fprintf( stdout, "Could not open the shared object library!\n" );
      exit(1);
   }

   int (*my_inxlib_gui)( struct my_xwin_vars* xvars );

   my_inxlib_gui = dlsym( handle, "inxlib_gui" );
   if( my_inxlib_gui == NULL ) {
      fprintf( stdout, "Could not locate symbol \"inxlib_gui\"!\n" );
      dlclose( handle );
      exit(1);
   }

   (*my_inxlib_gui)( &xvars );

   dlclose( handle );

   return 0;
}

