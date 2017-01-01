/******************************************************************************
 Copyright (c) 2016-2017, Ioannis Nompelis
 All rights reserved.

 Redistribution and use in source and binary forms, with or without any
 modification, are permitted provided that the following conditions are met:
 1. Redistribution of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
 2. Redistribution in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
 3. All advertising materials mentioning features or use of this software
    must display the following acknowledgement:
    "This product includes software developed by Ioannis Nompelis."
 4. Neither the name of Ioannis Nompelis and his partners/affiliates nor the
    names of other contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.
 5. Redistribution or use of source code and binary forms for profit must
    have written permission of the copyright holder.
 
 THIS SOFTWARE IS PROVIDED BY IOANNIS NOMPELIS ''AS IS'' AND ANY
 EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL IOANNIS NOMPELIS BE LIABLE FOR ANY
 DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 ******************************************************************************/

///////////////////////////////////////////////////////////////////////////
// This example shows how to build on top of the currrent framework by
// compiling everything together. Functions built by the user are all
// compiled together, and thus inclusion of other .c files is _expected_.
// This is not the best practice, but it is the quickest and dirtiest.
// IN 2016/06/29
///////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <inxlib.h>

//
// Including this external file allows it to be compiled together with this
// example. This makes the function-pointer assignment really simple, because
// it can otherwise be a real burden for the user to achieve.
//
#include "game.c"         // HACK TO DEMONSTRATE

//
// Generic function to be called for drawing
// (The only reason this function is introduced is so that the appropriate
// arguments are passed to the actual drawing function when it is invoked from
// the function pointer in the main loop.)
//
int user_draw( struct my_xwin_vars *xvars, void *data )
{
#ifdef _CASE3_
   // DEMO 3
   // We use a C cpde that is a simple game

   // we call the drawing frunction from the first case  to draw a background
   game_draw( 0, 0,
              xvars->win_width, xvars->win_height,
              xvars->font_base, xvars->xdisplay, xvars->xwindow );
   // we pass the pointers given to us and sit back and wait...
#endif


   // swap the buffers to the frame we just rendered
   glXSwapBuffers( xvars->xdisplay, xvars->xwindow );
   printf("THIS SHOULD HAVE PLOTTED SOMETHING \n");

   return(0);
}


//
// Generic function to be called when the window is "configured"
// (The only reason this function is introduced is so that the appropriate
// arguments are passed to the actual configure function when it is invoked from
// the function pointer in the main loop.)
//
int user_configure( struct my_xwin_vars *xvars, XEvent *event )
{

   return(0);
}

//
// Generic function to be called when we trap and handle a KeyPress event
// (The only reason this function is introduced is so that the appropriate
// arguments are passed to the actual C++ function when it is invoked from
// the function pointer in the main loop.)
//
int user_keypress( struct my_xwin_vars *xvars, XEvent *event )
{

   return(0);
}

//
// Generic function to be called when we trap and handle a KeyRelease event
// (Works similarly to the keypress function.)
//
int user_keyrelease( struct my_xwin_vars *xvars, XEvent *event )
{

   return(0);
}

//
// Generic function to be called when we trap and handle a MotionNotify event
//
int user_motionnotify( struct my_xwin_vars *xvars, XEvent *event )
{
   XButtonEvent *ep = (XButtonEvent *) event;
   static int ix = -1;
   static int iy = -1;
   float dx,dy;

   dx = ((float) (ep->x - ix)) / ((float) xvars->win_width);
   dy = ((float) (ep->y - iy)) / ((float) xvars->win_width);
   if( ix == -1 ) {   // first motion
      dx = 0.0;
      dy = 0.0;
   }
   ix = ep->x;
   iy = ep->y;

   gvars.px += dx*1.8;
   gvars.pz += dy*2.8;

   // keep the paddle in the box
   if( gvars.px < -(0.5-0.05) ) gvars.px = -(0.5-0.05);
   if( gvars.px > +(0.5-0.05) ) gvars.px = +(0.5-0.05);
   if( gvars.pz < -(0.5-0.05) ) gvars.pz = -(0.5-0.05);
   if( gvars.pz > +(0.5-0.05) ) gvars.pz = +(0.5-0.05);

   return(0);
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


   return(0);
}

