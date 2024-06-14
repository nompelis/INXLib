/*****************************************************************************
 INXlib v0.6
 A simple skeleton framework for building X11 windowed applications with XLib.
 It includes an OpenGL context for 3D graphics.
 Copyright 2016-2024 Ioannis Nompelis
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
                   int width, int height, int xpos, int ypos,
                   int ithread, int irender, int iframe )
{
   int ierr;
   XVisualInfo *visinfo;
#ifndef _OLDSTYLE_
   GLXFBConfig *fbconfig;
#endif
   XSetWindowAttributes win_attr;
   unsigned long attr_mask;

   // the following array will be used for initialization
#ifdef _OLDSTYLE_
   int glx_attr[] ={ GLX_RGBA,
                     GLX_RED_SIZE, 8,       // number signifies bits
                     GLX_GREEN_SIZE, 8,
                     GLX_BLUE_SIZE, 8,
                     GLX_ALPHA_SIZE, 8,
                     GLX_DEPTH_SIZE, 24,
                     GLX_STENCIL_SIZE, 8,
                     GLX_DOUBLEBUFFER, True,
                     None };   // this line terminates the list
#else
   int glx_attr[] ={ GLX_RGBA,
                     GLX_X_RENDERABLE, True,
                     GLX_DRAWABLE_TYPE,
                     GLX_WINDOW_BIT,
                     GLX_RENDER_TYPE,
                     GLX_RGBA_BIT,
                     GLX_X_VISUAL_TYPE,
                     GLX_TRUE_COLOR,
                     GLX_RED_SIZE, 8,
                     GLX_GREEN_SIZE, 8,
                     GLX_BLUE_SIZE, 8,
                     GLX_ALPHA_SIZE, 8,
                     GLX_DEPTH_SIZE, 24,
                     GLX_STENCIL_SIZE, 8,
                     GLX_DOUBLEBUFFER, True,
                     // GLX_SAMPLE_BUFFERS  , 1,
                     // GLX_SAMPLES         , 4,
                     None };   // this line terminates the list
#endif

   //
   // Initialize Xlib in multi-thread mode if requested
   // (this seems to be necessary if mutliple threads will use Xlib, but if
   // the library is used by an application that has potentially made this
   // call before, we cannot make this call. We give a choice to use users.)
   //
   if( ithread == 0 ) {
      ierr = XInitThreads();
      if( ierr == 0 ) {
         fprintf( stderr, " [Error]  Could not start in multi-thread mode\n" );
         return 1;
      }
   }

   //
   // make a connection to the X server
   // (we use the user's default display from the environment variable)
   //
   xvars->xdisplay = XOpenDisplay( getenv("DISPLAY") );
   if( xvars->xdisplay == NULL ) {
      fprintf( stderr, " [Error]  Failed to open display.\n" );
      return 2;
   }

   //
   // echo the number of screens on this X display
   //
   fprintf( stderr, " [INFO]  The display has %d available screen(s)\n",
            ScreenCount( xvars->xdisplay ) );

   //
   // get the default screen number (we will open our window in it) and get
   // the root window
   //
   xvars->xscreen = DefaultScreen( xvars->xdisplay );
   xvars->xroot   = RootWindow( xvars->xdisplay, xvars->xscreen );

#ifndef _OLDSTYLE_
   //
   // retrieve a framebuffer configuration
   //
   int fbcount;
   fbconfig = glXChooseFBConfig( xvars->xdisplay, xvars->xscreen, glx_attr,
                                 &fbcount );
   if( !fbconfig ) {
      fprintf( stderr, " [Error]  Failed to retrieve a framebuffer config\n" );
      XCloseDisplay( xvars->xdisplay );
      xvars->xdisplay = NULL;
      return 3;
   } else {
      fprintf( stderr, " [INFO]  Number of FB config: %d \n", fbcount );
   }
#endif

   //
   // allocate a structure to store the X display's visual info
   // from the attributes we requested
   //
#ifdef _OLDSTYLE_
   visinfo = glXChooseVisual( xvars->xdisplay, xvars->xscreen, glx_attr );
#else
   visinfo = glXGetVisualFromFBConfig( xvars->xdisplay, fbconfig[0] );
#endif
   if( !visinfo ) {
      fprintf( stderr, " [Error]  Unable to find RGB, double-buffer visual\n" );
      // close connection to the Xserver
      XCloseDisplay( xvars->xdisplay );
      xvars->xdisplay = NULL;
      return 4;
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
      xvars->win_height = (unsigned int) height;
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
   win_attr.background_pixmap = None;
   win_attr.background_pixel = 0;
   win_attr.border_pixel = 2;
   win_attr.colormap =
            XCreateColormap( xvars->xdisplay, xvars->xroot, visinfo->visual,
                             AllocNone );
   win_attr.cursor = None;  // May point to an allocated special cursor
   win_attr.override_redirect = False;
   if( iframe == 1 )  {
      win_attr.override_redirect = True;    // this makes the window solid!
   }
   win_attr.event_mask = StructureNotifyMask |
                         ExposureMask |
                         FocusChangeMask |
                         PointerMotionMask |
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
                        visinfo->visual, attr_mask, &win_attr );
   if( xvars->xwindow <= 0 ) {
      fprintf( stderr, " [Error]  Unable to create window \n" );
#ifndef _OLDSTYLE_
      // drop FB configuration object(s)
      XFree( fbconfig );
#endif
      // drop visual info
      XFree( visinfo );
      // close connection to the Xserver
      XCloseDisplay( xvars->xdisplay );
      xvars->xdisplay = NULL;
      return 5;
   }

   XStoreName( xvars->xdisplay, xvars->xwindow, xvars->window_name );

   //
   // Specify events to be sent to the program from the window
   //
   XSelectInput( xvars->xdisplay, xvars->xwindow,
                 ExposureMask |
                 StructureNotifyMask |
                 FocusChangeMask |
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
   //
#ifdef _OLDSTYLE_
   if( irender == 1 ) {
      xvars->glxc = glXCreateContext( xvars->xdisplay, visinfo, NULL, True );
   } else {
      xvars->glxc = glXCreateContext( xvars->xdisplay, visinfo, NULL, False );
   }
#else
   //
   // Create a modern OpenGL context
   //
   typedef GLXContext (*my_func)( Display*, GLXFBConfig, GLXContext,
                                  Bool, const int* );
   my_func glXCreateContextAttribsARB = NULL;
   glXCreateContextAttribsARB = (my_func)
           glXGetProcAddressARB( (const GLubyte*)"glXCreateContextAttribsARB" );

   // This library does _not_ use the latest versions of the "core" profile
   // because it uses display lists. This compromise allows for the modern
   // programmable pipeline to be used _and_ the fixed pipeline rendering!
   int context_attribs[] = {
   // GLX_CONTEXT_MAJOR_VERSION_ARB, 4,   // Core profile (DO NOT USE)
   // GLX_CONTEXT_MINOR_VERSION_ARB, 5,
   // GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
      GLX_CONTEXT_MAJOR_VERSION_ARB, 3,   // Compatibility profile
      GLX_CONTEXT_MINOR_VERSION_ARB, 0,
      GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
      None
   };

   xvars->glxc = glXCreateContextAttribsARB( xvars->xdisplay,
                                             fbconfig[0], NULL, True,
                                             context_attribs );
#endif
   if( xvars->glxc == NULL ) {
      fprintf( stderr, " [Error]  Could not create GLX context!\n" );
#ifndef _OLDSTYLE_
      XFree( fbconfig );
#endif
      XFree( visinfo );
      XDestroyWindow( xvars->xdisplay, xvars->xwindow );
      xvars->xwindow = 0;
      XCloseDisplay( xvars->xdisplay );
      xvars->xdisplay = NULL;
      return 6;
   }

   // make sure the second GLX context is unuseable
   xvars->glxc2 = NULL;

#ifndef _OLDSTYLE_
   //
   // make a GLX window (this is for high-performance applications)
   //
   xvars->glxwin = glXCreateWindow( xvars->xdisplay, fbconfig[0],
                                    xvars->xwindow, NULL );
   if( !(xvars->glxwin) ) {
      fprintf( stderr, " [Error]  Could not create a GLX area! \n" );
      XFree( fbconfig );
      XFree( visinfo );
      glXDestroyContext( xvars->xdisplay, xvars->glxc );
      glXDestroyContext( xvars->xdisplay, xvars->glxc2 );
      XDestroyWindow( xvars->xdisplay, xvars->xwindow );
      xvars->xwindow = 0;
      XCloseDisplay( xvars->xdisplay );
      xvars->xdisplay = NULL;
      return 8;
   }
#endif

   //
   // make this the current OpenGL context (does not harm)
   //
#ifndef _OLDSTYLE_
   glXMakeContextCurrent( xvars->xdisplay, xvars->glxwin, xvars->glxwin,
                          xvars->glxc );
#else
   glXMakeCurrent( xvars->xdisplay, xvars->xwindow, xvars->glxc );
#endif

   //
   // Drop structures we no longer need
   //
#ifndef _OLDSTYLE_
   XFree( fbconfig );
#endif
   XFree( visinfo );

   //
   // Set the internal variable for user-guided termination of the library
   //
   xvars->iterm_loop = 0;

   return 0;
}


/**
// @details
//
// Function to create and bring up an X window with two associated OpenGL
// rendering contexts that share resources
//
// @author Ioannis Nompelis <nompelis@nobelware.com>
*/
int xwindow_setup_dualglx( struct my_xwin_vars *xvars,
                           int width, int height, int xpos, int ypos,
                           int iframe )
{
   int ierr;
   XVisualInfo *visinfo;
#ifndef _OLDSTYLE_
   GLXFBConfig *fbconfig;
#endif
   XSetWindowAttributes win_attr;
   unsigned long attr_mask;

   // the following array will be used for initialization
#ifdef _OLDSTYLE_
   int glx_attr[] ={ GLX_RGBA,
                     GLX_RED_SIZE, 8,       // number signifies bits
                     GLX_GREEN_SIZE, 8,
                     GLX_BLUE_SIZE, 8,
                     GLX_ALPHA_SIZE, 8,
                     GLX_DEPTH_SIZE, 24,
                     GLX_STENCIL_SIZE, 8,
                     GLX_DOUBLEBUFFER, True,
                     None };   // this line terminates the list
#else
   int glx_attr[] ={
                     GLX_X_RENDERABLE, True,
                     GLX_DRAWABLE_TYPE,
                     GLX_WINDOW_BIT,
                     GLX_RENDER_TYPE,
                     GLX_RGBA_BIT,
                     GLX_X_VISUAL_TYPE,
                     GLX_TRUE_COLOR,
                     GLX_RED_SIZE, 8,
                     GLX_GREEN_SIZE, 8,
                     GLX_BLUE_SIZE, 8,
                     GLX_ALPHA_SIZE, 8,
                     GLX_DEPTH_SIZE, 24,
                     GLX_STENCIL_SIZE, 8,
                     GLX_DOUBLEBUFFER, True,
                     // GLX_SAMPLE_BUFFERS  , 1,
                     // GLX_SAMPLES         , 4,
                     None };   // this line terminates the list
#endif

   //
   // Initialize Xlib in multi-thread mode
   // (This is absolutely needed if the user is to use this as intended.)
   //
   ierr = XInitThreads();
   if( ierr == 0 ) {
      fprintf( stderr, " [Error]  Could not start in multi-thread mode\n" );
      return 1;
   }

   //
   // make a connection to the X server
   // (we use the user's default display from the environment variable)
   //
   xvars->xdisplay = XOpenDisplay( getenv("DISPLAY") );
   if( xvars->xdisplay == NULL ) {
      fprintf( stderr, " [Error]  Failed to open display.\n" );
      return 2;
   }

   //
   // echo the number of screens on this X display
   //
   fprintf( stderr, " [INFO]  The display has %d available screen(s)\n",
            ScreenCount( xvars->xdisplay ) );

   //
   // get the default screen number (we will open our window in it) and get
   // the root window
   //
   xvars->xscreen = DefaultScreen( xvars->xdisplay );
   xvars->xroot   = RootWindow( xvars->xdisplay, xvars->xscreen );

#ifndef _OLDSTYLE_
   //
   // retrieve a framebuffer configuration
   //
   int fbcount;
   fbconfig = glXChooseFBConfig( xvars->xdisplay, xvars->xscreen, glx_attr,
                                 &fbcount );
   if( !fbconfig ) {
      fprintf( stderr, " [Error]  Failed to retrieve a framebuffer config\n" );
      XCloseDisplay( xvars->xdisplay );
      xvars->xdisplay = NULL;
      return 3;
   } else {
      fprintf( stderr, " [INFO]  Number of FB config: %d \n", fbcount );
   }
#endif

   //
   // allocate a structure to store the X display's visual info
   // from the attributes we requested
   //
#ifdef _OLDSTYLE_
   visinfo = glXChooseVisual( xvars->xdisplay, xvars->xscreen, glx_attr );
#else
   visinfo = glXGetVisualFromFBConfig( xvars->xdisplay, fbconfig[0] );
#endif
   if( !visinfo ) {
      fprintf( stderr, " [Error]  Unable to find RGB, double-buffer visual\n" );
      // close connection to the Xserver
      XCloseDisplay( xvars->xdisplay );
      xvars->xdisplay = NULL;
      return 4;
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
      xvars->win_height = (unsigned int) height;
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
   win_attr.background_pixmap = None;
   win_attr.background_pixel = 0;
   win_attr.border_pixel = 2;
   win_attr.colormap =
            XCreateColormap( xvars->xdisplay, xvars->xroot, visinfo->visual,
                             AllocNone );
   win_attr.cursor = None;  // May point to an allocated special cursor
   win_attr.override_redirect = False;
   if( iframe == 1 )  {
      win_attr.override_redirect = True;    // this makes the window solid!
   }
   win_attr.event_mask = StructureNotifyMask |
                         ExposureMask |
                         FocusChangeMask |
                         PointerMotionMask |
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
                        visinfo->visual, attr_mask, &win_attr );
   if( xvars->xwindow <= 0 ) {
      fprintf( stderr, " [Error]  Unable to create window \n" );
#ifndef _OLDSTYLE_
      // drop FB configuration object(s)
      XFree( fbconfig );
#endif
      // drop visual info
      XFree( visinfo );
      // close connection to the Xserver
      XCloseDisplay( xvars->xdisplay );
      xvars->xdisplay = NULL;
      return 5;
   }

   XStoreName( xvars->xdisplay, xvars->xwindow, xvars->window_name );

   //
   // Specify events to be sent to the program from the window
   //
   XSelectInput( xvars->xdisplay, xvars->xwindow,
                 ExposureMask |
                 StructureNotifyMask |
                 FocusChangeMask |
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
   //
#ifdef _OLDSTYLE_
   xvars->glxc = glXCreateContext( xvars->xdisplay, visinfo, NULL, True );
#else
   //
   // Create a modern OpenGL context
   //
   typedef GLXContext (*my_func)( Display*, GLXFBConfig, GLXContext,
                                  Bool, const int* );
   my_func glXCreateContextAttribsARB = NULL;
   glXCreateContextAttribsARB = (my_func)
           glXGetProcAddressARB( (const GLubyte*)"glXCreateContextAttribsARB" );

   // This library does _not_ use the latest versions of the "core" profile
   // because it uses display lists. This compromise allows for the modern
   // programmable pipeline to be used _and_ the fixed pipeline rendering!
   int context_attribs[] = {
   // GLX_CONTEXT_MAJOR_VERSION_ARB, 4,   // Core profile (DO NOT USE)
   // GLX_CONTEXT_MINOR_VERSION_ARB, 5,
   // GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
      GLX_CONTEXT_MAJOR_VERSION_ARB, 3,   // Compatibility profile
      GLX_CONTEXT_MINOR_VERSION_ARB, 0,
      GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
      None
   };

   xvars->glxc = glXCreateContextAttribsARB( xvars->xdisplay,
                                             fbconfig[0], NULL, True,
                                             context_attribs );
#endif
   if( xvars->glxc == NULL ) {
      fprintf( stderr, " [Error]  Could not create GLX context!\n" );
#ifndef _OLDSTYLE_
      XFree( fbconfig );
#endif
      XFree( visinfo );
      XDestroyWindow( xvars->xdisplay, xvars->xwindow );
      xvars->xwindow = 0;
      XCloseDisplay( xvars->xdisplay );
      xvars->xdisplay = NULL;
      return 6;
   }

#ifdef _OLDSTYLE_
   xvars->glxc2 = glXCreateContext( xvars->xdisplay, visinfo, xvars->glxc,
                                                                        True );
#else
   xvars->glxc2 = glXCreateContextAttribsARB( xvars->xdisplay,
                                              fbconfig[0], xvars->glxc, True,
                                              context_attribs );
#endif
   if( xvars->glxc2 == NULL ) {
      fprintf( stderr, " [Error]  Could not create 2nd GLX context!\n" );
#ifndef _OLDSTYLE_
      XFree( fbconfig );
#endif
      XFree( visinfo );
      glXDestroyContext( xvars->xdisplay, xvars->glxc );
      XDestroyWindow( xvars->xdisplay, xvars->xwindow );
      xvars->xwindow = 0;
      XCloseDisplay( xvars->xdisplay );
      xvars->xdisplay = NULL;
      return 7;
   }

#ifndef _OLDSTYLE_
   //
   // make a GLX window (this is for high-performance applications)
   //
   xvars->glxwin = glXCreateWindow( xvars->xdisplay, fbconfig[0],
                                    xvars->xwindow, NULL );
   if( !(xvars->glxwin) ) {
      fprintf( stderr, " [Error]  Could not create a GLX area! \n" );
      XFree( fbconfig );
      XFree( visinfo );
      glXDestroyContext( xvars->xdisplay, xvars->glxc );
      glXDestroyContext( xvars->xdisplay, xvars->glxc2 );
      XDestroyWindow( xvars->xdisplay, xvars->xwindow );
      xvars->xwindow = 0;
      XCloseDisplay( xvars->xdisplay );
      xvars->xdisplay = NULL;
      return 8;
   }
#endif

   //
   // make this the current OpenGL context (does not harm)
   //
#ifndef _OLDSTYLE_
   glXMakeContextCurrent( xvars->xdisplay, xvars->glxwin, xvars->glxwin,
                          xvars->glxc );
#else
   glXMakeCurrent( xvars->xdisplay, xvars->xwindow, xvars->glxc );
#endif

   //
   // Drop structures we no longer need
   //
#ifndef _OLDSTYLE_
   XFree( fbconfig );
#endif
   XFree( visinfo );

   //
   // Set the internal variable for user-guided termination of the library
   //
   xvars->iterm_loop = 0;

   return 0;
}


void xwindow_query_glxversion( struct my_xwin_vars *xvars )
{
   int glxMajor, glxMinor;
   if( glXQueryVersion( xvars->xdisplay, &glxMajor, &glxMinor) ) {
      fprintf( stdout, " [INFO]  GLX Version: %d.%d\n", glxMajor, glxMinor );
   } else {
      fprintf( stdout, " [Error]  Failed to query GLX version\n" );
   }
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
   glXMakeCurrent( xvars->xdisplay, None, NULL );
   if( xvars->glxc2 != NULL ) {
      glXDestroyContext( xvars->xdisplay, xvars->glxc2 );
      fprintf( stderr, " [INFO]  Released 2nd GLX context \n" );
   }
   glXDestroyContext( xvars->xdisplay, xvars->glxc );
   fprintf( stderr, " [INFO]  Released GLX context \n" );
   xvars->glxc = NULL;

#ifndef _OLDSTYLE_
   //
   // Destroy GLX area
   //
   glXDestroyWindow( xvars->xdisplay, xvars->glxwin );
   fprintf( stderr, " [INFO]  Destroyed GLX area \n" );
   xvars->glxwin = 0;
#endif

   //
   // Destroy window
   //
   XDestroyWindow( xvars->xdisplay, xvars->xwindow );
   fprintf( stderr, " [INFO]  Destroyed X window \n" );
   xvars->xwindow = 0;

   //
   // Close connection to X server
   //
   XCloseDisplay( xvars->xdisplay );
   fprintf( stderr, " [INFO]  Closed connection to the X server \n" );
   xvars->xdisplay = NULL;

   return 0;
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
   if( ikey == XK_Escape && ictrl > 0 ) {
      fprintf( stderr, " [INFO]  The \"Escape\" key is pressed.\n" );
      return -1;
   } else {
      return 0;
   }
}


/**
// @details
//
// Function to handle X events that are trapped by an appropriately setup
// X window. It contains a mechanism for graceful exit. This function is meant
// to trap events, pass them to handler functions, and assume that they will be
// handled; handling is done by the software that uses this library as a front
// end. This function can act on its own on top of "sending" all events for
// handling, and this is the case here for illustration purposes. The function
// keeps track of key-presses, etc, as needed, but all events pass to the
// functions that have been assigned to the function-pointers by the user.
// This function can terminate the loop on its own, or through a variable that
// can be set in the back-end by the underlying software; the latter way should
// be the proper behaviour.
//
// @author Ioannis Nompelis <nompelis@nobelware.com>
*/
int xwindow_eventtrap( struct my_xwin_vars *xvars )
{
   Display *xdisplay = xvars->xdisplay;
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

      // perform pre-event, pre-drawing operations
      if( xvars->callback_FrameEntry != NULL ) {
         xvars->callback_FrameEntry( xvars );
      }

   while(XPending(xdisplay) > 0) {
      XEvent event;
      KeySym ikey;

      XNextEvent(xdisplay, &event);

      // decision making based on X event retreived
      switch(event.type) {

        case Expose:
         fprintf( stderr, " [INFO]  Got \"Expose\" event.\n" );

         // This function can act on its own here...
         // ...and/or dispatch the event to user-assigned functions for handling
         if(xvars->callback_Expose != NULL) {
            xvars->callback_Expose( xvars, &event );
         }
         break;

        case ConfigureNotify:
         fprintf( stderr, " [INFO]  Got \"ConfigureNotify\" event.\n" );
         // variables to track are:
         // event.xconfigure.width;
         // event.xconfigure.height;

         // This function can act on its own here...
         // ...and/or dispatch the event to user-assigned functions for handling
         if(xvars->callback_ConfigureNotify != NULL)
            xvars->callback_ConfigureNotify( xvars, &event );
         break;

        case MapNotify:
         fprintf( stderr, " [INFO]  Got \"MapNotify\" event.\n" );

         // This function can act on its own here...
         // ...and/or dispatch the event to user-assigned functions for handling
         if(xvars->callback_MapNotify != NULL) {
            xvars->callback_MapNotify( xvars, &event );
         }
         break;

        case KeyPress:
         fprintf( stderr, " [INFO]  Got \"KeyPress\" event.\n" );
         ikey = XLookupKeysym(&event.xkey, 0);

         // This function can act on its own here...
         // ...and/or dispatch the event to user-assigned functions for handling

         // first trap control / alt / shift
         if(ikey == XK_Shift_L) {
            fprintf( stderr, " [INFO]  The \"SHIFT L\" key is pressed.\n" );
            ishift_key = 1;
         } else
         if(ikey == XK_Shift_R) {
            fprintf( stderr, " [INFO]  The \"SHIFT R\" key is pressed.\n" );
            ishift_key = 2;
         } else
         if(ikey == XK_Control_L) {
            fprintf( stderr, " [INFO]  The \"CTRL L\" key is pressed.\n" );
            ictrl_key = 1;
         } else
         if(ikey == XK_Control_R) {
            fprintf( stderr, " [INFO]  The \"CTRL R\" key is pressed.\n" );
            ictrl_key = 2;
         } else
         if(ikey == XK_Alt_L) {
            fprintf( stderr, " [INFO]  The \"ALT L\" key is pressed.\n" );
            ialt_key = 1;
         } else
         if(ikey == XK_Alt_R) {
            fprintf( stderr, " [INFO]  The \"ALT R\" key is pressed.\n" );
            ialt_key = 2;
         }


         if(ikey == XK_Return) {
            fprintf( stderr, " [INFO]  The \"ENTER\" key is pressed.\n" );
         } else

         if(ikey == XK_Up) {
            fprintf( stderr, " [INFO]  The \"Up Arrow\" key is pressed.\n" );
         } else
         if(ikey == XK_Down) {
            fprintf( stderr, " [INFO]  The \"Down Arrow\" key is pressed.\n" );
         } else
         if(ikey == XK_Left) {
            fprintf( stderr, " [INFO]  The \"Left Arrow\" key is pressed.\n" );
         } else
         if(ikey == XK_Right) {
            fprintf( stderr, " [INFO]  The \"Right Arrow\" key is pressed.\n" );
         } else {
            // this will trap all other keys
            iresult = react_to_key_press(ikey, ishift_key, ictrl_key, ialt_key);
            if(iresult == -1) {
               iend = 1;     // exit mechanism
               printf("Exiting From the inxlib.c exit mechanism \n");
            } else {
               // possibly do other things...
            }
         }

         if(xvars->callback_KeyPress != NULL) {
            xvars->callback_KeyPress( xvars, &event );
         }
         break;

        case KeyRelease:
         fprintf( stderr, " [INFO]  Got \"KeyRelease\" event.\n" );
         ikey = XLookupKeysym(&event.xkey, 0);

         // we only care about releases of helper keys (shift, alt, ctrl)

         if(ikey == XK_Shift_L || ikey == XK_Shift_R) {
            fprintf( stderr, " [INFO]  The \"SHIFT\" key was released.\n" );
            ishift_key = 0;
         } else
         if(ikey == XK_Control_L || ikey == XK_Control_R) {
            fprintf( stderr, " [INFO]  The \"CTRL\" key was released.\n" );
            ictrl_key = 0;
         } else
         if(ikey == XK_Alt_L || ikey == XK_Alt_R) {
            fprintf( stderr, " [INFO]  The \"ALT\" key was released.\n" );
            ialt_key = 0;
         }

         if(xvars->callback_KeyRelease != NULL) {
            xvars->callback_KeyRelease( xvars, &event );
         }
         break;

        case ButtonPress:
         fprintf( stderr, " [INFO]  Got \"ButtonPress\" event.\n" );
         if(event.xbutton.button == 1) { ileft_button = 1; }
         if(event.xbutton.button == 2) { imiddle_button = 1; }
         if(event.xbutton.button == 3) { iright_button = 1; }

         if(xvars->callback_ButtonPress != NULL) {
            xvars->callback_ButtonPress( xvars, &event );
         }
         break;

        case ButtonRelease:
         fprintf( stderr, " [INFO]  Got \"ButtonRelease\" event.\n" );
         if(event.xbutton.button == 1) { ileft_button = 0; }
         if(event.xbutton.button == 2) { imiddle_button = 0; }
         if(event.xbutton.button == 3) { iright_button = 0; }

         if(xvars->callback_ButtonRelease != NULL) {
            xvars->callback_ButtonRelease( xvars, &event );
         }
         break;

        case MotionNotify:
         fprintf( stderr, " [INFO]  Got \"MotionNotify\" event.\n" );

         if(xvars->callback_MotionNotify != NULL) {
            xvars->callback_MotionNotify( xvars, &event );
         }
         break;

        case EnterNotify:
         fprintf( stderr, " [INFO]  Got \"EnterNotify\" event.\n" );

         if(xvars->callback_EnterNotify != NULL) {
            xvars->callback_EnterNotify( xvars, &event );
         }
         break;

        case FocusIn:
         fprintf( stderr, " [INFO]  Got \"FocusIn\" event.\n" );

         if(xvars->callback_FocusIn != NULL) {
            xvars->callback_FocusIn( xvars, &event );
         }
         break;

        case FocusOut:
         fprintf( stderr, " [INFO]  Got \"FocusOut\" event.\n" );

         if(xvars->callback_FocusOut != NULL) {
            xvars->callback_FocusOut( xvars, &event );
         }
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

      // we impose an appropriate delay to maintain constant frame-rate
      usleep( isleep );

      // call the function to draw the screen
      if( xvars->callback_DrawScreen != NULL ) {
         xvars->callback_DrawScreen( xvars, NULL );
      }

      // perform post-event, post-drawing operations
      if( xvars->callback_FrameExit != NULL ) {
          xvars->callback_FrameExit( xvars );
      }

      // allow for a user termination condition to exit the loop
      if( xvars->iterm_loop != 0 ) iend = 1;
   }

   return 0;
}


/**
// @details
//
// Function to create a font's display lists given the font name.
// It uses GLX's functionality to load a font by name, assign attributes
// of the font to the appropriate X-variables structure members, and to
// allocate in the GL context the display lists for all letters.
//
// @author Ioannis Nompelis <nompelis@nobelware.com>
*/
int xwindow_setupDefaultFont( struct my_xwin_vars *xvars, char *font_name )
{
   XFontStruct *fontInfo;
   Font fid;

   // first make the current GL context active so we can access it
   glXMakeCurrent( xvars->xdisplay, xvars->xwindow, xvars->glxc );

   fontInfo = XLoadQueryFont( xvars->xdisplay, font_name );
   if( fontInfo == NULL ) {
      fprintf( stderr, " [Error]  Could not find font! This is surprising.\n" );
      return 1;
   }

   // get font id and first and last character counts and height
   fid = fontInfo->fid;
   xvars->font_min_char = fontInfo->min_char_or_byte2;
   xvars->font_max_char = fontInfo->max_char_or_byte2;
   xvars->font_height =
               fontInfo->max_bounds.ascent + fontInfo->max_bounds.descent;
   xvars->font_width =
               fontInfo->max_bounds.rbearing + fontInfo->max_bounds.lbearing;

   // generate the GL list-set for the particular number of lists
   xvars->font_base = glGenLists((GLsizei) (xvars->font_max_char + 1));
   if( xvars->font_base == 0 ) {
      fprintf( stderr, " [Error]  Could not make default font display list.\n");
      XFreeFont( xvars->xdisplay, fontInfo );
      return 2;
   }

   //
   // Fill in the lists with of the ll-fl+1 characters starting from
   // character fl and using as count the number FontBase+fl
   // (this is smart, because now we can use the character's value to
   // call the list that represents that character).
   //
   glXUseXFont( fid, xvars->font_min_char,
                     xvars->font_max_char - xvars->font_min_char + 1,
                     xvars->font_base + xvars->font_min_char );

   // free the font struct
   XFreeFont( xvars->xdisplay, fontInfo );

   return 0;
}


GLubyte font_Cp850_8X8_8x8_flip[2048] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x81, 0x99, 0xbd, 0x81, 0xa5, 0x81, 0x7e,
  0x7e, 0xff, 0xe7, 0xc3, 0xff, 0xdb, 0xff, 0x7e, 0x00, 0x10, 0x38, 0x7c, 0xfe, 0xfe, 0xfe, 0x6c,
  0x00, 0x10, 0x38, 0x7c, 0xfe, 0x7c, 0x38, 0x10, 0x38, 0x10, 0xd6, 0xfe, 0xfe, 0x38, 0x7c, 0x38,
  0x38, 0x10, 0x7c, 0xfe, 0xfe, 0x7c, 0x38, 0x10, 0x00, 0x00, 0x18, 0x3c, 0x3c, 0x18, 0x00, 0x00,
  0xff, 0xff, 0xe7, 0xc3, 0xc3, 0xe7, 0xff, 0xff, 0x00, 0x3c, 0x66, 0x42, 0x42, 0x66, 0x3c, 0x00,
  0xff, 0xc3, 0x99, 0xbd, 0xbd, 0x99, 0xc3, 0xff, 0x78, 0xcc, 0xcc, 0xcc, 0x7d, 0x0f, 0x07, 0x0f,
  0x18, 0x7e, 0x18, 0x3c, 0x66, 0x66, 0x66, 0x3c, 0xe0, 0xf0, 0x70, 0x30, 0x30, 0x3f, 0x33, 0x3f,
  0xc0, 0xe6, 0x67, 0x63, 0x63, 0x7f, 0x63, 0x7f, 0x18, 0xdb, 0x3c, 0xe7, 0xe7, 0x3c, 0xdb, 0x18,
  0x00, 0x80, 0xe0, 0xf8, 0xfe, 0xf8, 0xe0, 0x80, 0x00, 0x02, 0x0e, 0x3e, 0xfe, 0x3e, 0x0e, 0x02,
  0x18, 0x3c, 0x7e, 0x18, 0x18, 0x7e, 0x3c, 0x18, 0x00, 0x66, 0x00, 0x66, 0x66, 0x66, 0x66, 0x66,
  0x00, 0x1b, 0x1b, 0x1b, 0x7b, 0xdb, 0xdb, 0x7f, 0x7c, 0x86, 0x3c, 0x66, 0x66, 0x3c, 0x61, 0x3e,
  0x00, 0x7e, 0x7e, 0x7e, 0x00, 0x00, 0x00, 0x00, 0xff, 0x18, 0x3c, 0x7e, 0x18, 0x7e, 0x3c, 0x18,
  0x00, 0x18, 0x18, 0x18, 0x18, 0x7e, 0x3c, 0x18, 0x00, 0x18, 0x3c, 0x7e, 0x18, 0x18, 0x18, 0x18,
  0x00, 0x00, 0x18, 0x0c, 0xfe, 0x0c, 0x18, 0x00, 0x00, 0x00, 0x30, 0x60, 0xfe, 0x60, 0x30, 0x00,
  0x00, 0x00, 0xfe, 0xc0, 0xc0, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x24, 0x66, 0xff, 0x66, 0x24, 0x00,
  0x00, 0x00, 0xff, 0xff, 0x7e, 0x3c, 0x18, 0x00, 0x00, 0x00, 0x18, 0x3c, 0x7e, 0xff, 0xff, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x18, 0x18, 0x3c, 0x3c, 0x18,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x66, 0x66, 0x00, 0x6c, 0x6c, 0xfe, 0x6c, 0xfe, 0x6c, 0x6c,
  0x00, 0x18, 0x7c, 0x06, 0x3c, 0x60, 0x3e, 0x18, 0x00, 0xc6, 0x66, 0x30, 0x18, 0xcc, 0xc6, 0x00,
  0x00, 0x76, 0xcc, 0xdc, 0x76, 0x38, 0x6c, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x18, 0x18,
  0x00, 0x0c, 0x18, 0x30, 0x30, 0x30, 0x18, 0x0c, 0x00, 0x30, 0x18, 0x0c, 0x0c, 0x0c, 0x18, 0x30,
  0x00, 0x00, 0x66, 0x3c, 0xff, 0x3c, 0x66, 0x00, 0x00, 0x00, 0x18, 0x18, 0x7e, 0x18, 0x18, 0x00,
  0x30, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x00,
  0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xc0, 0x60, 0x30, 0x18, 0x0c, 0x06,
  0x00, 0x38, 0x6c, 0xc6, 0xd6, 0xc6, 0x6c, 0x38, 0x00, 0x7e, 0x18, 0x18, 0x18, 0x18, 0x38, 0x18,
  0x00, 0xfe, 0x66, 0x30, 0x1c, 0x06, 0xc6, 0x7c, 0x00, 0x7c, 0xc6, 0x06, 0x3c, 0x06, 0xc6, 0x7c,
  0x00, 0x1e, 0x0c, 0xfe, 0xcc, 0x6c, 0x3c, 0x1c, 0x00, 0x7c, 0xc6, 0x06, 0xfc, 0xc0, 0xc0, 0xfe,
  0x00, 0x7c, 0xc6, 0xc6, 0xfc, 0xc0, 0x60, 0x38, 0x00, 0x30, 0x30, 0x30, 0x18, 0x0c, 0xc6, 0xfe,
  0x00, 0x7c, 0xc6, 0xc6, 0x7c, 0xc6, 0xc6, 0x7c, 0x00, 0x78, 0x0c, 0x06, 0x7e, 0xc6, 0xc6, 0x7c,
  0x00, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x00, 0x30, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x00,
  0x00, 0x06, 0x0c, 0x18, 0x30, 0x18, 0x0c, 0x06, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x7e, 0x00, 0x00,
  0x00, 0x60, 0x30, 0x18, 0x0c, 0x18, 0x30, 0x60, 0x00, 0x18, 0x00, 0x18, 0x18, 0x0c, 0xc6, 0x7c,
  0x00, 0x78, 0xc0, 0xde, 0xde, 0xde, 0xc6, 0x7c, 0x00, 0xc6, 0xc6, 0xc6, 0xfe, 0xc6, 0x6c, 0x38,
  0x00, 0xfc, 0x66, 0x66, 0x7c, 0x66, 0x66, 0xfc, 0x00, 0x3c, 0x66, 0xc0, 0xc0, 0xc0, 0x66, 0x3c,
  0x00, 0xf8, 0x6c, 0x66, 0x66, 0x66, 0x6c, 0xf8, 0x00, 0xfe, 0x62, 0x68, 0x78, 0x68, 0x62, 0xfe,
  0x00, 0xf0, 0x60, 0x68, 0x78, 0x68, 0x62, 0xfe, 0x00, 0x3a, 0x66, 0xce, 0xc0, 0xc0, 0x66, 0x3c,
  0x00, 0xc6, 0xc6, 0xc6, 0xfe, 0xc6, 0xc6, 0xc6, 0x00, 0x3c, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3c,
  0x00, 0x78, 0xcc, 0xcc, 0x0c, 0x0c, 0x0c, 0x1e, 0x00, 0xe6, 0x66, 0x6c, 0x78, 0x6c, 0x66, 0xe6,
  0x00, 0xfe, 0x66, 0x62, 0x60, 0x60, 0x60, 0xf0, 0x00, 0xc6, 0xc6, 0xd6, 0xfe, 0xfe, 0xee, 0xc6,
  0x00, 0xc6, 0xc6, 0xce, 0xde, 0xf6, 0xe6, 0xc6, 0x00, 0x7c, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0x7c,
  0x00, 0xf0, 0x60, 0x60, 0x7c, 0x66, 0x66, 0xfc, 0x0e, 0x7c, 0xce, 0xc6, 0xc6, 0xc6, 0xc6, 0x7c,
  0x00, 0xe6, 0x66, 0x6c, 0x7c, 0x66, 0x66, 0xfc, 0x00, 0x3c, 0x66, 0x0c, 0x18, 0x30, 0x66, 0x3c,
  0x00, 0x3c, 0x18, 0x18, 0x18, 0x5a, 0x7e, 0x7e, 0x00, 0x7c, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6,
  0x00, 0x38, 0x6c, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0x00, 0x6c, 0xfe, 0xd6, 0xd6, 0xc6, 0xc6, 0xc6,
  0x00, 0xc6, 0xc6, 0x6c, 0x38, 0x6c, 0xc6, 0xc6, 0x00, 0x3c, 0x18, 0x18, 0x3c, 0x66, 0x66, 0x66,
  0x00, 0xfe, 0x66, 0x32, 0x18, 0x8c, 0xc6, 0xfe, 0x00, 0x3c, 0x30, 0x30, 0x30, 0x30, 0x30, 0x3c,
  0x00, 0x02, 0x06, 0x0c, 0x18, 0x30, 0x60, 0xc0, 0x00, 0x3c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x3c,
  0x00, 0x00, 0x00, 0x00, 0xc6, 0x6c, 0x38, 0x10, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x18, 0x30, 0x00, 0x76, 0xcc, 0x7c, 0x0c, 0x78, 0x00, 0x00,
  0x00, 0xdc, 0x66, 0x66, 0x66, 0x7c, 0x60, 0xe0, 0x00, 0x7c, 0xc6, 0xc0, 0xc6, 0x7c, 0x00, 0x00,
  0x00, 0x76, 0xcc, 0xcc, 0xcc, 0x7c, 0x0c, 0x1c, 0x00, 0x7c, 0xc0, 0xfe, 0xc6, 0x7c, 0x00, 0x00,
  0x00, 0xf0, 0x60, 0x60, 0xf8, 0x60, 0x66, 0x3c, 0xf8, 0x0c, 0x7c, 0xcc, 0xcc, 0x76, 0x00, 0x00,
  0x00, 0xe6, 0x66, 0x66, 0x76, 0x6c, 0x60, 0xe0, 0x00, 0x3c, 0x18, 0x18, 0x18, 0x38, 0x00, 0x18,
  0x3c, 0x66, 0x66, 0x06, 0x06, 0x06, 0x00, 0x06, 0x00, 0xe6, 0x6c, 0x78, 0x6c, 0x66, 0x60, 0xe0,
  0x00, 0x3c, 0x18, 0x18, 0x18, 0x18, 0x18, 0x38, 0x00, 0xd6, 0xd6, 0xd6, 0xfe, 0xec, 0x00, 0x00,
  0x00, 0x66, 0x66, 0x66, 0x66, 0xdc, 0x00, 0x00, 0x00, 0x7c, 0xc6, 0xc6, 0xc6, 0x7c, 0x00, 0x00,
  0xf0, 0x60, 0x7c, 0x66, 0x66, 0xdc, 0x00, 0x00, 0x1e, 0x0c, 0x7c, 0xcc, 0xcc, 0x76, 0x00, 0x00,
  0x00, 0xf0, 0x60, 0x60, 0x76, 0xdc, 0x00, 0x00, 0x00, 0xfc, 0x06, 0x7c, 0xc0, 0x7e, 0x00, 0x00,
  0x00, 0x1c, 0x36, 0x30, 0x30, 0xfc, 0x30, 0x30, 0x00, 0x76, 0xcc, 0xcc, 0xcc, 0xcc, 0x00, 0x00,
  0x00, 0x38, 0x6c, 0xc6, 0xc6, 0xc6, 0x00, 0x00, 0x00, 0x6c, 0xfe, 0xd6, 0xd6, 0xc6, 0x00, 0x00,
  0x00, 0xc6, 0x6c, 0x38, 0x6c, 0xc6, 0x00, 0x00, 0xfc, 0x06, 0x7e, 0xc6, 0xc6, 0xc6, 0x00, 0x00,
  0x00, 0x7e, 0x32, 0x18, 0x4c, 0x7e, 0x00, 0x00, 0x00, 0x0e, 0x18, 0x18, 0x70, 0x18, 0x18, 0x0e,
  0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x70, 0x18, 0x18, 0x0e, 0x18, 0x18, 0x70,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xdc, 0x76, 0x00, 0xfe, 0xc6, 0xc6, 0x6c, 0x38, 0x10, 0x00,
  0x78, 0x0c, 0x7c, 0xc6, 0xc0, 0xc0, 0xc6, 0x7c, 0x00, 0x76, 0xcc, 0xcc, 0xcc, 0xcc, 0x00, 0xcc,
  0x00, 0x7c, 0xc0, 0xfe, 0xc6, 0x7c, 0x18, 0x0c, 0x00, 0x76, 0xcc, 0x7c, 0x0c, 0x78, 0x82, 0x7c,
  0x00, 0x76, 0xcc, 0x7c, 0x0c, 0x78, 0x00, 0xc6, 0x00, 0x76, 0xcc, 0x7c, 0x0c, 0x78, 0x18, 0x30,
  0x00, 0x76, 0xcc, 0x7c, 0x0c, 0x78, 0x30, 0x30, 0x38, 0x0c, 0x7e, 0xc0, 0xc0, 0x7e, 0x00, 0x00,
  0x00, 0x7c, 0xc0, 0xfe, 0xc6, 0x7c, 0x82, 0x7c, 0x00, 0x7c, 0xc0, 0xfe, 0xc6, 0x7c, 0x00, 0xc6,
  0x00, 0x7c, 0xc0, 0xfe, 0xc6, 0x7c, 0x18, 0x30, 0x00, 0x3c, 0x18, 0x18, 0x18, 0x38, 0x00, 0x66,
  0x00, 0x3c, 0x18, 0x18, 0x18, 0x38, 0x82, 0x7c, 0x00, 0x3c, 0x18, 0x18, 0x38, 0x00, 0x18, 0x30,
  0x00, 0xc6, 0xc6, 0xfe, 0xc6, 0x6c, 0x38, 0xc6, 0x00, 0xc6, 0xc6, 0xfe, 0xc6, 0x7c, 0x6c, 0x38,
  0x00, 0xfe, 0xc0, 0xf8, 0xc0, 0xfe, 0x30, 0x18, 0x00, 0xfe, 0x90, 0xfe, 0x12, 0x7e, 0x00, 0x00,
  0x00, 0xce, 0xcc, 0xcc, 0xfe, 0xcc, 0x6c, 0x3e, 0x00, 0x7c, 0xc6, 0xc6, 0xc6, 0x7c, 0x82, 0x7c,
  0x00, 0x7c, 0xc6, 0xc6, 0xc6, 0x7c, 0x00, 0xc6, 0x00, 0x7c, 0xc6, 0xc6, 0xc6, 0x7c, 0x18, 0x30,
  0x00, 0x76, 0xcc, 0xcc, 0xcc, 0x00, 0x84, 0x78, 0x00, 0x76, 0xcc, 0xcc, 0xcc, 0xcc, 0x30, 0x60,
  0xfc, 0x06, 0x7e, 0xc6, 0xc6, 0xc6, 0x00, 0xc6, 0x00, 0x38, 0x6c, 0xc6, 0xc6, 0x6c, 0x38, 0xc6,
  0x00, 0x7c, 0xc6, 0xc6, 0xc6, 0xc6, 0x00, 0xc6, 0x80, 0x7c, 0xe6, 0xd6, 0xce, 0x7c, 0x02, 0x00,
  0x00, 0xfc, 0x66, 0x60, 0xf0, 0x64, 0x6c, 0x38, 0x00, 0xb8, 0x6c, 0xe6, 0xd6, 0xce, 0x6c, 0x3a,
  0x00, 0x00, 0xc6, 0x6c, 0x38, 0x6c, 0xc6, 0x00, 0x00, 0x70, 0xd8, 0x18, 0x3c, 0x18, 0x1b, 0x0e,
  0x00, 0x76, 0xcc, 0x7c, 0x0c, 0x78, 0x30, 0x18, 0x00, 0x3c, 0x18, 0x18, 0x38, 0x00, 0x18, 0x0c,
  0x00, 0x7c, 0xc6, 0xc6, 0xc6, 0x7c, 0x18, 0x0c, 0x00, 0x76, 0xcc, 0xcc, 0xcc, 0xcc, 0x30, 0x18,
  0x00, 0x66, 0x66, 0x66, 0xdc, 0x00, 0xdc, 0x76, 0x00, 0xce, 0xde, 0xf6, 0xe6, 0x00, 0xdc, 0x76,
  0x00, 0x00, 0x7e, 0x00, 0x3e, 0x6c, 0x6c, 0x3c, 0x00, 0x00, 0x7c, 0x00, 0x38, 0x6c, 0x6c, 0x38,
  0x00, 0x3e, 0x63, 0x30, 0x18, 0x18, 0x00, 0x18, 0x7e, 0x81, 0xa5, 0xb9, 0xa5, 0xb9, 0x81, 0x7e,
  0x00, 0x00, 0x06, 0x06, 0xfe, 0x00, 0x00, 0x00, 0x0f, 0xcc, 0x66, 0x33, 0x7e, 0x6c, 0xe6, 0x63,
  0x06, 0xdf, 0x6a, 0x36, 0x7a, 0x6c, 0xe6, 0x63, 0x00, 0x18, 0x3c, 0x3c, 0x18, 0x18, 0x00, 0x18,
  0x00, 0x00, 0x33, 0x66, 0xcc, 0x66, 0x33, 0x00, 0x00, 0x00, 0xcc, 0x66, 0x33, 0x66, 0xcc, 0x00,
  0x88, 0x22, 0x88, 0x22, 0x88, 0x22, 0x88, 0x22, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55,
  0xdd, 0x77, 0xdd, 0x77, 0xdd, 0x77, 0xdd, 0x77, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
  0x18, 0x18, 0x18, 0xf8, 0x18, 0x18, 0x18, 0x18, 0x00, 0xc6, 0xfe, 0xc6, 0x6c, 0x38, 0x60, 0x30,
  0x00, 0xc6, 0xfe, 0xc6, 0x6c, 0x38, 0x82, 0x7c, 0x00, 0xc6, 0xfe, 0xc6, 0x6c, 0x38, 0x0c, 0x18,
  0x7e, 0x81, 0x9d, 0xa1, 0xa1, 0x9d, 0x81, 0x7e, 0x36, 0x36, 0x36, 0xf6, 0x06, 0xf6, 0x36, 0x36,
  0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0xf6, 0x06, 0xfe, 0x00, 0x00,
  0x00, 0x00, 0x00, 0xfe, 0x06, 0xf6, 0x36, 0x36, 0x18, 0x18, 0x7e, 0xc0, 0xc0, 0x7e, 0x18, 0x18,
  0x18, 0x18, 0x7e, 0x18, 0x7e, 0x3c, 0x66, 0x66, 0x18, 0x18, 0x18, 0xf8, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x1f, 0x18, 0x18, 0x18, 0x18, 0x00, 0x00, 0x00, 0xff, 0x18, 0x18, 0x18, 0x18,
  0x18, 0x18, 0x18, 0xff, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0x1f, 0x18, 0x18, 0x18, 0x18,
  0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0xff, 0x18, 0x18, 0x18, 0x18,
  0x00, 0x7e, 0xc6, 0x7e, 0x06, 0x7c, 0xdc, 0x76, 0x00, 0xc6, 0xfe, 0xc6, 0x6c, 0x38, 0xdc, 0x76,
  0x00, 0x00, 0x00, 0x3f, 0x30, 0x37, 0x36, 0x36, 0x36, 0x36, 0x36, 0x37, 0x30, 0x3f, 0x00, 0x00,
  0x00, 0x00, 0x00, 0xff, 0x00, 0xf7, 0x36, 0x36, 0x36, 0x36, 0x36, 0xf7, 0x00, 0xff, 0x00, 0x00,
  0x36, 0x36, 0x36, 0x37, 0x30, 0x37, 0x36, 0x36, 0x00, 0x00, 0x00, 0xff, 0x00, 0xff, 0x00, 0x00,
  0x36, 0x36, 0x36, 0xf7, 0x00, 0xf7, 0x36, 0x36, 0x00, 0xc6, 0x7c, 0xc6, 0xc6, 0x7c, 0xc6, 0x00,
  0x00, 0x78, 0xcc, 0xcc, 0x7c, 0x0c, 0x7e, 0x30, 0x00, 0xf8, 0x6c, 0x66, 0xf6, 0x66, 0x6c, 0xf8,
  0x00, 0xfe, 0xc0, 0xfc, 0xc0, 0xfe, 0x82, 0x7c, 0x00, 0xfe, 0xc0, 0xfc, 0xc0, 0xfe, 0x00, 0xc6,
  0x00, 0xfe, 0xc0, 0xfc, 0xc0, 0xfe, 0x18, 0x30, 0x00, 0x3c, 0x18, 0x18, 0x18, 0x38, 0x00, 0x00,
  0x00, 0x3c, 0x18, 0x18, 0x18, 0x3c, 0x18, 0x0c, 0x00, 0x3c, 0x18, 0x18, 0x18, 0x3c, 0x42, 0x3c,
  0x00, 0x3c, 0x18, 0x18, 0x18, 0x3c, 0x00, 0x66, 0x00, 0x00, 0x00, 0xf8, 0x18, 0x18, 0x18, 0x18,
  0x18, 0x18, 0x18, 0x1f, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x18,
  0x00, 0x3c, 0x18, 0x18, 0x18, 0x3c, 0x18, 0x30, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
  0x00, 0x38, 0x6c, 0xc6, 0x6c, 0x38, 0x60, 0x30, 0x00, 0xcc, 0xc6, 0xcc, 0xd8, 0xcc, 0xcc, 0x78,
  0x00, 0x38, 0x6c, 0xc6, 0x6c, 0x38, 0x82, 0x7c, 0x00, 0x38, 0x6c, 0xc6, 0x6c, 0x38, 0x06, 0x0c,
  0x00, 0x7c, 0xc6, 0xc6, 0xc6, 0x7c, 0xdc, 0x76, 0x00, 0x38, 0x6c, 0xc6, 0x6c, 0x38, 0xdc, 0x76,
  0xc0, 0x7c, 0x66, 0x66, 0x66, 0x66, 0x00, 0x00, 0xf0, 0x60, 0x7c, 0x66, 0x66, 0x7c, 0x60, 0xe0,
  0x00, 0xf0, 0x60, 0x7c, 0x66, 0x7c, 0x60, 0xf0, 0x00, 0x7c, 0xc6, 0xc6, 0xc6, 0xc6, 0x30, 0x18,
  0x00, 0x7c, 0xc6, 0xc6, 0xc6, 0x00, 0x82, 0x7c, 0x00, 0x7c, 0xc6, 0xc6, 0xc6, 0xc6, 0x30, 0x60,
  0xfc, 0x06, 0x7e, 0xc6, 0xc6, 0xc6, 0x30, 0x18, 0x00, 0x3c, 0x18, 0x3c, 0x66, 0x66, 0x18, 0x0c,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x18, 0x0c,
  0x00, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x18, 0x18, 0x7e, 0x18, 0x18,
  0xff, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x86, 0x5f, 0x2a, 0xf6, 0x3a, 0xe4, 0x32, 0xe1,
  0x00, 0x1b, 0x1b, 0x1b, 0x7b, 0xdb, 0xdb, 0x7f, 0x7c, 0x86, 0x3c, 0x66, 0x66, 0x3c, 0x61, 0x3e,
  0x00, 0x00, 0x18, 0x00, 0x7e, 0x00, 0x18, 0x00, 0x38, 0x0c, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x38, 0x6c, 0x6c, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc6, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x18, 0x18, 0x38, 0x18,
  0x00, 0x00, 0x00, 0x78, 0x0c, 0x38, 0x0c, 0x78, 0x00, 0x00, 0x00, 0x7c, 0x30, 0x18, 0x0c, 0x78,
  0x00, 0x00, 0x3c, 0x3c, 0x3c, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/**
// @details
//
// Function to create a font's display lists from the internal data that
// was leached from the open source fonts shiped with Slackware 15.0.
// It uses OpenGL's internals for handling bitmaps and has my hardwored data
// of the font to the appropriate X-variables structure members, and to
// allocate in the GL context the display lists for all letters.
//
// @author Ioannis Nompelis <nompelis@nobelware.com>
*/
int xwindow_setupLinuxFont( struct my_xwin_vars *xvars )
{
   // first make the current GL context active so we can access it
   glXMakeCurrent( xvars->xdisplay, xvars->xwindow, xvars->glxc );

   // These are IN's guesses for chars and numbers for the console font
   xvars->font_min_char = 1;
   xvars->font_max_char = 255;
   xvars->font_height = 8;
   xvars->font_width = 8;

   // generate the GL list-set for the particular number of lists
   xvars->font_base = glGenLists((GLsizei) (xvars->font_max_char + 1));
   if( xvars->font_base == 0 ) {
      fprintf( stderr, " [Error]  Could not make custom font display list.\n" );
      return 2;
   }

   // Fill in the lists
   GLuint i;
   glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
   for(i=1;i<256;++i) {
      glNewList( xvars->font_base + i, GL_COMPILE );
      glBitmap( xvars->font_width, xvars->font_height,
                0.0, 2.0,  7.0, 0.0, &( font_Cp850_8X8_8x8_flip[ i*8 ] ) );
      glEndList();
   }

   return 0;
}


/**
// @details
//
// Function to delete display lists associated with a font
//
// @author Ioannis Nompelis <nompelis@nobelware.com>
*/
void xwindow_deletefont( GLuint iFontBase, unsigned int i )
{
   glDeleteLists( iFontBase, (GLsizei) i + 1 );
}

