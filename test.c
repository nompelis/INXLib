#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "inxlib.h"


int main(int argc, char **argv) {
   static struct my_xwin_vars xvars;
   int iret;

   // initialize our callbacks
   xwindow_callbacks( &xvars );

   // initialize Xlib and bring up a window with an OpenGL context
   sprintf(xvars.window_name, "OpenGL X Window");
   iret = xwindow_setup( &xvars, -1, -1, -1, -1, 0, 1, 0 );
// iret = xwindow_setup_dualglx( &xvars, -1, -1, -1, -1, 0 ); // dual GL
   if(iret != 0) exit(1);

   // making a font list (this is EXTRA)
// iret = xwindow_setupDefaultFont( &xvars, (char *) "fixed" );
   iret = xwindow_setupLinuxFont( &xvars );
   if(iret != 0) exit(2);

   // perform all operations to setup any extra GL stuff before proceeding
   void init();   // this function is in "my_demo.c" and uses "inogl.*"
   init();        // some objects are create to be rendered with included code

   // the main event loop
   (void) xwindow_eventtrap( &xvars );

   xwindow_close( &xvars ) ;

   return(0);
}

