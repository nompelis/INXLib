/*****************************************************************************
 INXlib v0.1
 A simple skeleton framework for building X11 windowed applications with XLib.
 It includes an OpenGL context for 3D graphics.
 Copyright 2016 Ioannis Nompelis
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "inxlib.h"
#include "inxfont.h"


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
   glXMakeCurrent( xvars->xdisplay, xvars->xwindow, xvars->glx_context );

   fontInfo = XLoadQueryFont( xvars->xdisplay, font_name );
   if(fontInfo == NULL) {
      fprintf(stderr," e  Could not find font! This is surprising.\n");
      return(1);
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
   if(xvars->font_base == 0) {
      fprintf(stderr," e  OpenGL could not make the font display list.\n");
      XFreeFont( xvars->xdisplay, fontInfo );
      return(2);
   }

   //
   // Fill in the lists with of the ll-fl+1 characters starting from
   // character fl and using as count the number FontBase+fl
   // (this is smart, because now we can use the character's value to
   // call the list that represents that character).
   //
   glXUseXFont(fid, xvars->font_min_char,
                    xvars->font_max_char - xvars->font_min_char + 1,
                    xvars->font_base + xvars->font_min_char );

   // free the font struct
   XFreeFont( xvars->xdisplay, fontInfo );

   return(0);
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

