
/****************************************************************************
 Simple game demo to demonstrate programmatic use of the INXLib v0.1 library:
 http://nobelware.com/RawCode/inxlib/inxlib.php
 https://github.com/nompelis/INXLib

 Ioannis Nompelis 2016/09/10
 ****************************************************************************/
/******************************************************************************
 Copyright (c) 2016, Ioannis Nompelis
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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "inxlib.h"
#include "inxfont.h"

#include "game.h"    // defines the struct

struct my_game_vars gvars;


int main(int argc, char **argv) {
   static struct my_xwin_vars xvars;
   int iret;

   // initialize our callbacks
   xwindow_callbacks( &xvars );

   // initialize Xlib and bring up a window with an OpenGL context
   sprintf(xvars.window_name, "OpenGL X Window (game demo)");
   iret = xwindow_setup( &xvars, -1, -1, -1, -1 );
   if(iret != 0) exit(1);

   // making a font list (this is EXTRA)
   iret = xwindow_setupDefaultFont( &xvars, (char *) "fixed" );
   if(iret != 0) exit(2);

   // initialize certain aspects of the demo
   gvars.px = 0.0;   // paddle position
   gvars.pz = 0.0;
   gvars.im = 40;    // sphere discretization size
   gvars.jm = 20;
   (void) inMakeAxisSphereshell( gvars.im, gvars.jm,
                                 &(gvars.sx), &(gvars.sy), &(gvars.sz),
                                 &(gvars.su), &(gvars.sv), &(gvars.sw),
                                 &(gvars.sp), &(gvars.sq) );

   (void) xwindow_eventtrap( &xvars );

   xwindow_close( &xvars ) ;

   return(0);
}

