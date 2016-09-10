/*****************************************************************************
 INXlib v0.1
 A simple skeleton framework for building X11 windowed applications with XLib.
 It includes an OpenGL context for 3D graphics.
 Copyright 2016 Ioannis Nompelis
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "inxlib.h"

/**
// @details
//
// Function to initialize all callbacks to NULL first, and to invoke a user
// function that set the callbacks based on custom (user-defined) functions.
// @author Ioannis Nompelis <nompelis@nobelware.com>
*/
int xwindow_callbacks( struct my_xwin_vars *xvars )
{
   int iret;

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
   // call the function that the user provides
   //
   iret = xwindow_user( xvars );

   return( iret );
}

/**
// @details
//
// Function to create and bring up an X window with an associated OpenGL
// rendering context
//
// @author Ioannis Nompelis <nompelis@nobelware.com>
*/
int xwindow_setup( struct my_xwin_vars *xvars,
                   int width, int height, int xpos, int ypos )
{
   int ierr;
   XVisualInfo *visinfo;
   XSetWindowAttributes win_attr;
   unsigned long attr_mask;

   // the following array will be used for initialization
   int glx_attr[] ={ GLX_RGBA,
                     GLX_RED_SIZE, 8,       // number signifies bits
                     GLX_GREEN_SIZE, 8,
                     GLX_BLUE_SIZE, 8,
                     GLX_DOUBLEBUFFER,
                     GLX_DEPTH_SIZE, 24,
                     None };   // this line terminates the list


   //
   // Initialize Xlib in multi-thread mode
   // (this seems to be necessary if mutliple threads will use Xlib)
   //
   ierr = XInitThreads();
   if(ierr == 0) {
      fprintf(stderr," e  Could not get Xlib to work in multi-thread mode\n");
      return(1);
   }

   //
   // make a connection to the X server
   // (we use the user's default display from the environment variable)
   //
   xvars->xdisplay = XOpenDisplay( getenv("DISPLAY") );
   if(xvars->xdisplay == NULL) {
      fprintf(stderr," e  Failed to open display.\n");
      return(2);
   }

   //
   // echo the number of screens on this X display
   //
   fprintf(stderr," i  The display has %d available screen(s)\n",
           ScreenCount( xvars->xdisplay ));

   //
   // get the default screen number (we will open our window in it) and get
   // the root window
   //
   xvars->xscreen = DefaultScreen( xvars->xdisplay );
   xvars->xroot   = RootWindow( xvars->xdisplay, xvars->xscreen );


   //
   // allocate a structure to store the X display's visual info
   // from the attributes we requested
   //
   visinfo = glXChooseVisual( xvars->xdisplay, xvars->xscreen, glx_attr );
   if(!visinfo) {
      fprintf(stderr," e Unable to find RGB, double-buffered visual\n");
      // close connection to the Xserver
      XCloseDisplay( xvars->xdisplay );
      xvars->xdisplay = NULL;
      return(3);
   }

   //
   // set position and size of the X window (use sizes from the screen as guide)
   //
   if( width == -1 ) {   // user has no preference
      xvars->win_width  = WidthOfScreen(ScreenOfDisplay( xvars->xdisplay,
                                                       xvars->xscreen )) * 3/4;
      xvars->win_height = HeightOfScreen(ScreenOfDisplay( xvars->xdisplay,
                                                       xvars->xscreen )) * 3/4;
      xvars->win_xpos = xvars->win_width / 4;
      xvars->win_ypos = xvars->win_height / 4;
   } else {
      xvars->win_width  = (unsigned int) width;
      xvars->win_height =  (unsigned int) height;
      xvars->win_xpos = xpos;
      xvars->win_ypos = ypos;
   }

   //
   // Set attributes of window and the attribute mask
   // Attributes include the choice of a movable window or frameless/static
   // one, and X events to be trapped by the window (mouse clicks, motion,
   // etc.) The attribute mask lets the API know of which of the attributes
   // we provided.
   //
   win_attr.background_pixel = 0;
   win_attr.border_pixel = 2;
   win_attr.colormap =
            XCreateColormap( xvars->xdisplay, xvars->xroot, visinfo->visual,
                             AllocNone);
   win_attr.cursor = None;  // May point to an allocated special cursor
   win_attr.override_redirect = True;    // this makes the window solid!
   win_attr.override_redirect = False;
   win_attr.event_mask = StructureNotifyMask |
                         ExposureMask |
                         ButtonPressMask |
                         ButtonReleaseMask |
                         KeyPressMask |
                         KeyReleaseMask;
   attr_mask = CWBackPixel |
               CWBorderPixel |
               CWColormap |
               CWEventMask |
               CWOverrideRedirect;

   //
   // create the X window
   //
   xvars->xwindow = XCreateWindow( xvars->xdisplay, xvars->xroot,
                        xvars->win_xpos,  xvars->win_ypos,
                        xvars->win_width, xvars->win_height, 0,
                        visinfo->depth, InputOutput,
                        visinfo->visual, attr_mask, &win_attr);
   if( xvars->xwindow <= 0 ) {
      fprintf(stderr," e Unable to create window \n");
      // drop visual info
      XFree( visinfo );
      // close connection to the Xserver
      XCloseDisplay( xvars->xdisplay );
      xvars->xdisplay = NULL;
      return(4);
   }

   XStoreName( xvars->xdisplay, xvars->xwindow, xvars->window_name );

   //
   // Specify events to be sent to the program from the window
   //
   XSelectInput( xvars->xdisplay, xvars->xwindow,
                ExposureMask |
                StructureNotifyMask |
                PointerMotionMask |
                ButtonPressMask |
                ButtonReleaseMask |
                KeyPressMask |
                KeyReleaseMask);

   //
   // Create the window and bring it up
   // (An X window can be created and not be displayed until needed.)
   //
   XMapWindow( xvars->xdisplay, xvars->xwindow );
   XMapRaised( xvars->xdisplay, xvars->xwindow );
   XFlush( xvars->xdisplay );

   //
   // Associate this window with an OpenGL context (create the context)
   // Here we make the selection of whether we want direct rendering (with
   // hardware acceleration) or indirect rendering (with software).
   // We will let this be hard-wired to indirect for now.
   //
// xvars->glx_context = glXCreateContext( xvars->xdisplay, visinfo, NULL, True);
   xvars->glx_context = glXCreateContext( xvars->xdisplay, visinfo, NULL,False);
   if(xvars->glx_context == NULL) {
      fprintf(stderr," e  Could not create GLX context!\n");
      XFree( visinfo );
      XDestroyWindow( xvars->xdisplay, xvars->xwindow );
      xvars->xwindow = 0;
      XCloseDisplay( xvars->xdisplay );
      xvars->xdisplay = NULL;
      return(5);
   }

   //
   // Drop structures we no longer need
   //
   XFree( visinfo );

   return(0);
}


/**
// @details
//
// Function to close an X window with an OpenGL context and close the connection
// to the X server
//
// @author Ioannis Nompelis <nompelis@nobelware.com>
*/
int xwindow_close( struct my_xwin_vars *xvars )
{

   //
   // Destroy the OpenGL context
   //
   glXDestroyContext( xvars->xdisplay, xvars->glx_context );
   fprintf(stderr, " i  Released GLX context \n");
   xvars->glx_context = NULL;

   //
   // Destroy window
   //
   XDestroyWindow( xvars->xdisplay, xvars->xwindow );
   fprintf(stderr, " i  Destroyed X window \n");
   xvars->xwindow = 0;

   //
   // Close connection to X server
   //
   XCloseDisplay( xvars->xdisplay );
   fprintf(stderr, " i  Closed connection to the X server \n");
   xvars->xdisplay = NULL;

   return(0);
}


/**
// @details
//
// Function that is up to the user to write. It decides termination in this case
//
// @author Ioannis Nompelis <nompelis@nobelware.com>
*/
int react_to_key_press(int ikey, int ishift, int ictrl, int ialt ) {
   // return a termination flag if CTRL-Escape is pressed
   if(ikey == XK_Escape && ictrl > 0) {
      fprintf(stderr," i  The \"Escape\" key is pressed.\n");
      return(-1);
   } else {
      return(0);
   }
}


/**
// @details
//
// Function to handle X events that are trapped by an appropriately setup
// X window. It contains a mechanism for graceful exit.
//
// @author Ioannis Nompelis <nompelis@nobelware.com>
*/
int xwindow_eventtrap( struct my_xwin_vars *xvars )
{
   Display *xdisplay = xvars->xdisplay;;
   int iend = 0;
   int ishift_key = 0, ialt_key = 0, ictrl_key = 0;
   int ileft_button = 0, iright_button = 0, imiddle_button = 0;

   while(iend == 0) {
      // time to sleep if there are no events
      int isleep = 25000;    // about 1/40 of a second
      int iresult;

      // We need to start a timer here to use a time-interval to maintain a
      // constant framerate. (We do not do that at present.)
      // [start timer call goes here]

   while(XPending(xdisplay) > 0) {
      XEvent event;
      KeySym ikey;

      XNextEvent(xdisplay, &event);

      // decision making based on X event retreived
      switch(event.type) {

        case Expose:
         fprintf(stderr," i  Got \"Expose\" event.\n");

         if(xvars->callback_Expose != NULL) xvars->callback_Expose();
         break;

        case ConfigureNotify:
         fprintf(stderr," i  Got \"ConfigureNotify\" event.\n");
         // variables to track are:
         // event.xconfigure.width;
         // event.xconfigure.height;

         if(xvars->callback_ConfigureNotify != NULL)
            xvars->callback_ConfigureNotify( xvars, NULL );
         break;

        case MapNotify:
         fprintf(stderr," i  Got \"MapNotify\" event.\n");

         if(xvars->callback_MapNotify != NULL) xvars->callback_MapNotify();
         break;

        case KeyPress:
         fprintf(stderr," i  Got \"KeyPress\" event.\n");
         ikey = XLookupKeysym(&event.xkey, 0);

         // first trap control / alt / shift
         if(ikey == XK_Shift_L) {
            fprintf(stderr," i  The \"SHIFT L\" key is pressed.\n");
            ishift_key = 1;
         } else
         if(ikey == XK_Shift_R) {
            fprintf(stderr," i  The \"SHIFT R\" key is pressed.\n");
            ishift_key = 2;
         } else
         if(ikey == XK_Control_L) {
            fprintf(stderr," i  The \"CTRL L\" key is pressed.\n");
            ictrl_key = 1;
         } else
         if(ikey == XK_Control_R) {
            fprintf(stderr," i  The \"CTRL R\" key is pressed.\n");
            ictrl_key = 2;
         } else
         if(ikey == XK_Alt_L) {
            fprintf(stderr," i  The \"ALT L\" key is pressed.\n");
            ialt_key = 1;
         } else
         if(ikey == XK_Alt_R) {
            fprintf(stderr," i  The \"ALT R\" key is pressed.\n");
            ialt_key = 2;
         }


         if(ikey == XK_Return) {
            fprintf(stderr," i  The \"ENTER\" key is pressed.\n");
         } else

         if(ikey == XK_Up) {
            fprintf(stderr," i  The \"Up Arrow\" key is pressed.\n");
         } else
         if(ikey == XK_Down) {
            fprintf(stderr," i  The \"Down Arrow\" key is pressed.\n");
         } else
         if(ikey == XK_Left) {
            fprintf(stderr," i  The \"Left Arrow\" key is pressed.\n");
         } else
         if(ikey == XK_Right) {
            fprintf(stderr," i  The \"Right Arrow\" key is pressed.\n");
         } else {
            // this will trap all other keys
            iresult = react_to_key_press(ikey, ishift_key, ictrl_key, ialt_key);
            if(iresult == -1) {
               iend = 1;     // exit mechanism
            } else {
               // possibly do other things...
            }
         }

         if(xvars->callback_KeyPress != NULL) {
            xvars->callback_KeyPress( xvars, &event );
         }
         break;

        case KeyRelease:
         fprintf(stderr," i  Got \"KeyRelease\" event.\n");
         ikey = XLookupKeysym(&event.xkey, 0);

         // we only care about releases of helper keys (shift, alt, ctrl)

         if(ikey == XK_Shift_L || ikey == XK_Shift_R) {
            fprintf(stderr," i  The \"SHIFT\" key was released.\n");
            ishift_key = 0;
         } else
         if(ikey == XK_Control_L || ikey == XK_Control_R) {
            fprintf(stderr," i  The \"CTRL\" key was released.\n");
            ictrl_key = 0;
         } else
         if(ikey == XK_Alt_L || ikey == XK_Alt_R) {
            fprintf(stderr," i  The \"ALT\" key was released.\n");
            ialt_key = 0;
         }

         if(xvars->callback_KeyRelease != NULL) {
            xvars->callback_KeyRelease( xvars, &event );
         }
         break;

        case ButtonPress:
         fprintf(stderr," i  Got \"ButtonPress\" event.\n");
         if(event.xbutton.button == 1) { ileft_button = 1; }
         if(event.xbutton.button == 2) { imiddle_button = 1; }
         if(event.xbutton.button == 3) { iright_button = 1; }

         if(xvars->callback_ButtonPress != NULL) {
            xvars->callback_ButtonPress( xvars, &event );
         }
         break;

        case ButtonRelease:
         fprintf(stderr," i  Got \"ButtonRelease\" event.\n");
         if(event.xbutton.button == 1) { ileft_button = 0; }
         if(event.xbutton.button == 2) { imiddle_button = 0; }
         if(event.xbutton.button == 3) { iright_button = 0; }

         if(xvars->callback_ButtonRelease != NULL) {
            xvars->callback_ButtonRelease( xvars, &event );
         }
         break;

        case MotionNotify:
         fprintf(stderr," i  Got \"MotionNotify\" event.\n");

         if(xvars->callback_MotionNotify != NULL) {
            xvars->callback_MotionNotify( xvars, &event );
         }
         break;

        case EnterNotify:
         fprintf(stderr," i  Got \"EnterNotify\" event.\n");

         if(xvars->callback_EnterNotify != NULL) {
            xvars->callback_EnterNotify( xvars, &event );
         }
         break;

        case FocusIn:
         fprintf(stderr," i  Got \"FocusIn\" event.\n");

         if(xvars->callback_FocusIn != NULL) xvars->callback_FocusIn();
         break;

        case FocusOut:
         fprintf(stderr," i  Got \"FocusOut\" event.\n");

         if(xvars->callback_FocusOut != NULL) xvars->callback_FocusOut();
         break;

      }

      // when any event is trapped, make update very quick
      // (I have found that this is up to the user's taste)
      // isleep =  25000;
   }

      // We click at time again to measure a time-interval here, subtract the
      // interval from the interval corresponding to a constant frame-rate and
      // sleep the remainder of time.
      // [stop timer call goes here]

      // we impose an appropriate delay
      usleep(isleep);

      // call the function to draw the screen
      xvars->callback_DrawScreen( xvars, NULL );
   }

   return(0);
}

