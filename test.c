#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "inxlib.h"
#include "inxfont.h"


int main(int argc, char **argv) {
   static struct my_xwin_vars xvars;
   int iret;

   // initialize our callbacks
   xwindow_callbacks( &xvars );

   // initialize Xlib and bring up a window with an OpenGL context
   sprintf(xvars.window_name, "OpenGL X Window");
   iret = xwindow_setup( &xvars, -1, -1, -1, -1 );
   if(iret != 0) exit(1);

   // making a font list (this is EXTRA)
   iret = xwindow_setupDefaultFont( &xvars, (char *) "fixed" );
   if(iret != 0) exit(2);

   (void) xwindow_eventtrap( &xvars );

   xwindow_close( &xvars ) ;

   return(0);
}

