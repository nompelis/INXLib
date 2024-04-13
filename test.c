// This file is a driver of a user-built application. It performs all the
// X11 and GLX operations to: open a window, bring up an OpenGL rendering
// context, build some renderable fonts (using the "fixed pipeline"), and
// perform some setting up operations of the user's application code.
//
// The idea is for this file to remain lean so as to clearly show the order
// of operations at the high level, while it remains unaware of all the code
// that the user needs to build on their own. The function "xwindow_callbacks()"
// should be found in "inxlib_user.c" and it should (1) assign all callbacks
// that this framework needs to render and trap user inputs, and (2) do all
// the setting up of the user's application.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "inxlib.h"


int main(int argc, char **argv) {
   static struct my_xwin_vars xvars;
   int iret;

   //
   // initialize our callbacks and everything else the user needs
   //
   xwindow_callbacks( &xvars );

   //
   // initialize Xlib and bring up a window with an OpenGL context
   //
   sprintf(xvars.window_name, "OpenGL X Window");
   // single GL context
// iret = xwindow_setup( &xvars, -1, -1, -1, -1, 0, 1, 0 );
   // two GL contexts that share resources
   iret = xwindow_setup_dualglx( &xvars, -1, -1, -1, -1, 0 );    // dual GL
   if(iret != 0) exit(1);

   //
   // making a font list to render text with the fixed pipeline
   //
// iret = xwindow_setupDefaultFont( &xvars, (char *) "fixed" );
   iret = xwindow_setupLinuxFont( &xvars );
   if(iret != 0) exit(2);
   (void) xwindow_query_glxversion( &xvars );

   //
   // perform all operations to setup any extra GL stuff before proceeding
   // (The OpenGL contexts are ready at this point.)
   // You can make this driver aware of functions in your application as in
   // the example below.
   //
   void prog_init( void* );   // this function is inside the app's source code
   prog_init( (void*) &xvars );

   //
   // the main event loop
   //
   (void) xwindow_eventtrap( &xvars );

   xwindow_close( &xvars ) ;

   return 0;
}

