
///////////////////////////////////////////////////////////////////////////
// This example shows how to build on top of the currrent framework by
// compiling everything together. Functions built by the user are all
// compiled together, and thus inclusion of other .c files is _expected_.
// This is not the best practice, but it is the quickest and dirtiest.
// IN 2016/06/29
// A modification and an addition were made to make this demonstration
// able to use an external "library" that renders widgets. In order to
// make the library's variables accessible we introduced an external
// function that accesses global variables.
// IN 2016/07/04
// A second external function was added. This function does a "configure"
// to the widgets that are being used.
// IN 2016/07/30
///////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "inxlib.h"

//
// Including this external file allows it to be compiled together with this
// example. This makes the function-pointer assignment really simple, because
// it can otherwise be a real burden for the user to achieve.
//
#include "dummy_draw.c"   // HACK TO TEST

#ifdef _DEBUG_FONT_
static int global_cursor_x=0, global_cursor_y=0, do_draw_cursor=0;
#endif

//
// Generic function to be called for drawing
// (The only reason this function is introduced is so that the appropriate
// arguments are passed to the actual drawing function when it is invoked from
// the function pointer in the main loop.)
//
int user_draw( struct my_xwin_vars *xvars, void *data )
{
#ifdef _CASE1_
   // DEMO 1
   // We ignore all user data for this demo (no use of the "void *data" pointer)

   // we call a function of our choosing, and in this case we have compiled the
   // function in this file (with all its dependencies above...)
   dummy_draw( 0, 0,
               xvars->win_width, xvars->win_height,
               xvars->font_base, xvars->xdisplay, xvars->xwindow );
#endif

#ifdef _CASE2_
   // DEMO 2
   // We use a C++ framework to do all of our drawing

   // we call the drawing frunction from the first case  to draw a background
   dummy_draw( 0, 0,
               xvars->win_width, xvars->win_height,
               xvars->font_base, xvars->xdisplay, xvars->xwindow );
   // we pass the pointers given to us and sit back and wait...
   (void) ingl_widgets_draw( xvars, data );
#endif

#ifdef _DEBUG_FONT_
   if( do_draw_cursor ) {
      glColor3f( 0.2, 1.0, 0.2 );
      glWindowPos2i( global_cursor_x, xvars->win_height - global_cursor_y );
      glCallLists( (GLsizei) strlen( "CURSOR" ), GL_UNSIGNED_BYTE, "CURSOR" );
      do_draw_cursor=0;
   }
#endif

   // swap the buffers to the frame we just rendered
   glXSwapBuffers( xvars->xdisplay, xvars->xwindow );
// printf("THIS SHOULD HAVE PLOTTED SOMETHING \n");

   return 0;
}


//
// Generic function to be called when the window is "configured"
// (The only reason this function is introduced is so that the appropriate
// arguments are passed to the actual configure function when it is invoked from
// the function pointer in the main loop.)
//
int user_configure( struct my_xwin_vars *xvars, XEvent *event )
{
#ifdef _CASE2_
   // we pass the pointers given to us and sit back and wait...
   (void) ingl_widgets_root_configure( xvars, (void *) event );
#endif

   return 0;
}

//
// Generic function to be called when we trap and handle a KeyPress event
// (The only reason this function is introduced is so that the appropriate
// arguments are passed to the actual C++ function when it is invoked from
// the function pointer in the main loop.)
//
int user_keypress( struct my_xwin_vars *xvars, XEvent *event )
{
#ifdef _CASE2_
   // we pass the pointers given to us and sit back and wait...
   (void) ingl_events_handle_keypress( xvars, (void *) event );
#endif

   return 0;
}

//
// Generic function to be called when we trap and handle a KeyRelease event
// (Works similarly to the keypress function.)
//
int user_keyrelease( struct my_xwin_vars *xvars, XEvent *event )
{
#ifdef _CASE2_
   // we pass the pointers given to us and sit back and wait...
   (void) ingl_events_handle_keyrelease( xvars, (void *) event );
#endif

   return 0;
}


//
// Generic function to be called when we trap and handle a ButtonPress event
//
int user_buttonpress( struct my_xwin_vars *xvars, XEvent *event )
{
#ifdef _CASE2_
   // we pass the pointers given to us and sit back and wait...
   (void) ingl_events_handle_buttonpress( xvars, (void *) event );
#endif

   return 0;
}


//
// Generic function to be called when we trap and handle a ButtonRelease event
//
int user_buttonrelease( struct my_xwin_vars *xvars, XEvent *event )
{
#ifdef _CASE2_
   // we pass the pointers given to us and sit back and wait...
   (void) ingl_events_handle_buttonrelease( xvars, (void *) event );
#endif

   return 0;
}


//
// Generic function to be called when we trap and handle a MotionNotify event
//
int user_motionnotify( struct my_xwin_vars *xvars, XEvent *event )
{
#ifdef _CASE2_
   // we pass the pointers given to us and sit back and wait...
   (void) ingl_events_handle_motionnotify( xvars, (void *) event );
#endif

#ifdef _DEBUG_FONT_
   XButtonEvent *ep = (XButtonEvent *) event;
   printf("X: %d   Y: %d \n", ep->x, ep->y );
   global_cursor_x = ep->x + 20;
   global_cursor_y = ep->y - 20;
   do_draw_cursor=1;
#endif

   return 0;
}


//
// Generic function to be called when we trap and handle a FocusChange event
//
int user_focuschange( struct my_xwin_vars *xvars, XEvent *event )
{
#ifdef _CASE2_
   (void) ingl_events_handle_focuschange( xvars, (void *) event );
#endif

   return 0;
}



//
// Function to specify callbacks (user must build this function)
//
int xwindow_user( struct my_xwin_vars *xvars )
{
   xvars->callback_Expose = NULL;
   xvars->callback_ConfigureNotify = NULL;
   xvars->callback_MapNotify = NULL;
   xvars->callback_KeyPress = NULL;
   xvars->callback_KeyRelease = NULL;
   xvars->callback_ButtonPress = NULL;
   xvars->callback_ButtonRelease = NULL;
   xvars->callback_MotionNotify = NULL;
   xvars->callback_EnterNotify = NULL;
   xvars->callback_FocusIn = NULL;
   xvars->callback_FocusOut = NULL;

   //
   // specify the drawing function
   //
   xvars->callback_DrawScreen = user_draw;
   //
   // specify the window-configure function
   //
   xvars->callback_ConfigureNotify = user_configure;
   //
   // specify a key-press handling function
   //
   xvars->callback_KeyPress = user_keypress;
   //
   // specify a key-release handling function
   //
   xvars->callback_KeyRelease = user_keyrelease;
   //
   // specify a mouse movement handling function
   //
   xvars->callback_MotionNotify = user_motionnotify;
   //
   // specify a focus-change handling function
   //
   xvars->callback_FocusIn = user_focuschange;
   xvars->callback_FocusOut = user_focuschange;
   //
   // specify a button-press handling function
   //
   xvars->callback_ButtonPress = user_buttonpress;
   //
   // specify a button-release handling function
   //
   xvars->callback_ButtonRelease = user_buttonrelease;


   return 0;
}

