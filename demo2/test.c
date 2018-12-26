
/****************************************************************************
 Simple demo to demonstrate programmatic use of the INXLib v0.1 library:
 http://nobelware.com/RawCode/inxlib/inxlib.php
 https://github.com/nompelis/INXLib

 Ioannis Nompelis 2017/05/25
 ****************************************************************************/
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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "inxlib.h"
#include "inxfont.h"

#include "image.h"    // defines the struct
#include "injpeg.h"

struct my_image_vars ivars;


int main(int argc, char **argv) {
   static struct my_xwin_vars xvars;
   int iret;

   // initialize our callbacks
   xwindow_callbacks( &xvars );

   // initialize Xlib and bring up a window with an OpenGL context
   sprintf(xvars.window_name, "OpenGL X Window (game demo)");
   iret = xwindow_setup( &xvars, -1, -1, -1, -1, 0, 0, 0 );
   if(iret != 0) exit(1);

   // making a font list (this is EXTRA)
   iret = xwindow_setupDefaultFont( &xvars, (char *) "fixed" );
   if(iret != 0) exit(2);

   // initialize certain aspects of the demo
   ivars.px = 0.0;   // paddle position
   ivars.pz = 0.0;
//// image loading goes here
{
   int i,irgb;
   char filename[100];

   // load images
   ivars.img_num = 20;
   for(i=1;i<=20;++i) {
      sprintf(filename, "brain_tomo/brain_%.3d.jpg", i);
      (void) injpg_PeekImage(filename,
                             &(ivars.iwidth[i]),&(ivars.iheight[i]),&irgb);
      (void) injpg_ReadImage(filename,
                             &(ivars.img_data[i]),
                             &(ivars.iwidth[i]),&(ivars.iheight[i]),&irgb);
      printf("Returned: width %d height %d rgb %d \n",
              ivars.iwidth[i],ivars.iheight[i],irgb);
   // if(i==10) test_output(ivars.img_data[i],
   //                       ivars.iwidth[i],ivars.iheight[i],irgb);
   }
}

   (void) xwindow_eventtrap( &xvars );

   xwindow_close( &xvars ) ;

   return(0);
}

