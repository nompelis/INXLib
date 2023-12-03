/*****************************************************************************
 INXlib v0.6
 A simple skeleton framework for building X11 windowed applications with XLib.
 It includes an OpenGL context for 3D graphics.
 Copyright 2016-2023 Ioannis Nompelis
 *****************************************************************************/

#ifndef _INXLIB_H_
#define _INXLIB_H_

#include <X11/Xlib.h>
//#include <X11/Xutil.h>
#include <X11/keysym.h>

#include <GL/gl.h>
#include <GL/glx.h>
//#include <GL/glu.h>

/**
// @brief
// A struct that holds an X-windows font's variables
*/
struct my_xwin_font {
   unsigned int base;
   unsigned int max_char;
   unsigned int min_char;
   unsigned int height;
   unsigned int width;
};

/**
// @brief
// The main struct that holds the X-windows related variables
*/
struct my_xwin_vars {
   Display *xdisplay;
   int xscreen;
   Window xroot, xwindow;
   GLXContext glxc;
   GLXContext glxc2;

   char window_name[100];
   unsigned int win_height;
   unsigned int win_width;
   int win_xpos;
   int win_ypos;

   //---- OpenGL font related items (what we call "default font" now)
   unsigned int font_base;
   unsigned int font_max_char;
   unsigned int font_min_char;
   unsigned int font_height;
   unsigned int font_width;
   //---- additional fonts
   struct my_xwin_font* fonts;    // user is responsible for cleaning up!

   //---- pointers to user-specified functions to provide callbacks (reactions)
   int (*callback_FrameEntry)( struct my_xwin_vars * );
   int (*callback_FrameExit)( struct my_xwin_vars * );
   int (*callback_Expose)( struct my_xwin_vars *, XEvent * );
   int (*callback_ConfigureNotify)( struct my_xwin_vars *, XEvent * );
   int (*callback_MapNotify)( struct my_xwin_vars *, XEvent * );
   int (*callback_KeyPress)( struct my_xwin_vars *, XEvent * );
   int (*callback_KeyRelease)( struct my_xwin_vars *, XEvent * );
   int (*callback_ButtonPress)( struct my_xwin_vars *, XEvent * );
   int (*callback_ButtonRelease)( struct my_xwin_vars *, XEvent * );
   int (*callback_MotionNotify)( struct my_xwin_vars *, XEvent * );
   int (*callback_EnterNotify)( struct my_xwin_vars *, XEvent * );
   int (*callback_FocusIn)( struct my_xwin_vars *, XEvent * );
   int (*callback_FocusOut)( struct my_xwin_vars *, XEvent * );

   //---- pointer to the function that does the drawing
   int(*callback_DrawScreen)( struct my_xwin_vars *, void * );

   //---- variable to allow for the user to terminate the event loop
   int iterm_loop;
};


//
// function prototypes
//

/**
// @brief
// A function that sets callbacks
*/
int xwindow_callbacks( struct my_xwin_vars *xvars );

/**
// @brief
// A function to open an X window and allocate a GLX OpenGL context in it
*/
int xwindow_setup( struct my_xwin_vars *xvars,
                   int width, int height, int xpos, int ypos,
                   int ithread, int irender, int iframe );

/**
// @brief
// A function to open an X window and allocate two GLX OpenGL contexts in it
// that share resource
*/
int xwindow_setup_dualglx( struct my_xwin_vars *xvars,
                           int width, int height, int xpos, int ypos,
                           int iframe );

/**
// @brief
// A function to close an X window and destroy the GLX OpenGL context
*/
int xwindow_close( struct my_xwin_vars *xvars );

/**
// @brief
// A function to continuously trap and handle events
*/
int xwindow_eventtrap( struct my_xwin_vars *xvars );

/**
// @brief
// A function that is called when callbacks are set
*/
int xwindow_user( struct my_xwin_vars *xvars );


/** @brief
 * A function to setup a default font for use in an OpenGL context
 */
int xwindow_setupDefaultFont( struct my_xwin_vars *xvars, char *font_name );

/** @brief
 * A function to setup a Linux font for use in an OpenGL context
 */
int xwindow_setupLinuxFont( struct my_xwin_vars *xvars );

/** @brief
 * A function to delete individual lists (derived from a font) from an OpenGL
 * context
 */
void xwindow_deletefont( GLuint iFontBase, unsigned int i );

#endif
