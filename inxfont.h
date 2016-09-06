/*****************************************************************************
 INXlib v0.1
 A simple skeleton framework for building X11 windowed applications with XLib.
 It includes an OpenGL context for 3D graphics.
 Copyright 2016 Ioannis Nompelis
 *****************************************************************************/

#ifndef _INXFONT_H_
#define _INXFONT_H_

#include <X11/Xlib.h>
//#include <X11/Xutil.h>
//#include <X11/keysym.h>

#include <GL/gl.h>
#include <GL/glx.h>
//#include <GL/glu.h>


/** @brief
 * A function to setup a font for use in an OpenGL context
 */
int xwindow_setupDefaultFont( struct my_xwin_vars *xvars, char *font_name );

/** @brief
 * A function to delete individual lists (derived from a font) from an OpenGL
 * context
 */
void xwindow_deletefont( GLuint iFontBase, unsigned int i );

#endif
