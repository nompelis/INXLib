// This is the user's application code, and this file is presently included
// from the "inxlib_user.c" file such that the function "app_draw()' is both
// immediatelly known to the parent framework and it is compiled on the fly.
// (It does not have to be this way; a prototype of "app_draw()" can be known
// in "inxlib_user.c" and the code compiled xternally.)
// The code-injetion tree looks like this:
//    app.c    (this file)
//    |
//    |- app_render_prog.c     (programmable pipeline code and more...)
//    |  |- threads.c          (used to have a producer/consumer pattern)
//    |  |- axissphere.c       (creates data for rendering a sphere)
//    |
//    |- app_render_fixed.c    (fixed pipeline code)
//   
// The function "app_draw()" does all the rendering, which uses the fixed
// pipeline to rasterize text and draw a box, and the programmable pipeline
// to draw all 3D objects. The programmable pipeline rendering is injected
// inside the fixed pipeline rendering, which shows how both can be mixed.
// Also, the programmable pipeline takes the projection/modelview matrices
// from the fixed pipeline dynamic setup via "uniform variables".
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <GL/gl.h>
#include <GL/glu.h>

// This file contains all the functions and data sued in the fixed pipeline
#include "app_render_fixed.c"

// This file contains all the functions used in the programmable pipeline
#include "app_render_prog.c"


//
// a function that does standard "fixed pipeline" raterization
//

void app_draw( int iloc_, int jloc_,
               unsigned int width_, unsigned int height_,
               unsigned int font_base, Display *xdisplay, Window xwindow )
{
   GLenum glerr;
   while(( glerr = glGetError() ) != GL_NO_ERROR ) {
      const char* errorString = inGetGLErrorString(glerr);
      printf("Dummy_Draw top error: %d \"%s\"\n", glerr, errorString );
   }

   GLfloat mvt[] = {1.0, 0.0, 0.0, 0.0,    // a foobar matrix to use
                    0.0, 1.0, 0.0, 0.0,
                    0.0, 0.0, 1.0, 0.0,
                    0.0, 0.0, 0.0, 1.0};

   // ----- main framebuffer setup -----
   glViewport( iloc_, jloc_, width_, height_ );

   glClearColor( 0.15, 0.15, 0.15, 1.0 );
   glClearDepth( 40.0 );
   glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

   glEnable( GL_DEPTH_TEST );
   glDepthFunc( GL_LEQUAL );

   glShadeModel( GL_SMOOTH );
   glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

   glDisable( GL_BLEND );

   // ---- demonstration of trimming where rasterization takes place
   // ---- NOTE: this schissoring test will affect the programmable pipeline too
   // ---- (shrinking the virtual box defined here will trip rasterization)
#define   _SCISSOR_
#ifdef _SCISSOR_
   glEnable( GL_SCISSOR_TEST );
   glScissor( (GLint) (iloc_ -1), (GLint) (jloc_ -1),
              (GLsizei) (width_ +1), (GLsizei) (height_ +1) );
#endif

   // ====== an early rasterization of text stuff ======
   glMatrixMode( GL_PROJECTION );   // select the projection matrix mode
   glLoadIdentity();                // reset the proj. matrix
   // making the projection matrix from an orthographic projection (for tests)
   if(1) {
      const GLdouble x0=0.0,y0=0.0,x1=2.0,y1=2.0,xn=-10.0,xf=10.0;; // HACK
      glMultMatrixf( mvt );
      glOrtho( x0,x1, y0,y1, xn,xf );
   }

   glMatrixMode( GL_MODELVIEW );    // Select the modelview matrix stack
   glLoadIdentity();                // reset the modelv. matrix
   glMultMatrixf( mvt );

   glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
   glDisable( GL_CULL_FACE );

   glDisable( GL_LIGHTING );

   // making a ring of words on the 2D rasterization (as "overlay")
   if(0) {
      GLfloat rpos[4];    // THIS WILL BE DELETED
      int n;
      double d;
      float c;
      rpos[3] = 1.0;
      for(n=0;n<360;n=n+10) {
         d = (double) n; d = d/180.0*3.14156;
         c = (float) n; c = c/360.0;
         rpos[0] =  (cos(d) + 0.0);
         rpos[1] =  (sin(d) + 0.0);
         rpos[2] =  0.5;
//printf("%lf %lf %lf \n",rpos[0],rpos[1],rpos[2]);//HACK
         glColor4f( 1.0, c, c, 1.0 );

         glRasterPos3fv( rpos );
         glListBase( font_base );
         char text[10]; sprintf( text, "DEMO" );
         glCallLists( strlen(text), GL_UNSIGNED_BYTE, (GLubyte *) text );
      }
   }

   if(1) {  // show 2D text rasterization (as "overlay")
      GLfloat rpos[4];    // THIS WILL BE DELETED TOO
      rpos[0] = 0.1;
      rpos[1] = 0.1;
      rpos[2] = 0.5;
      rpos[3] = 1.0;
      glColor4f( 1.0, 1.0, 1.0, 1.0 );
      glRasterPos3fv( rpos );
      glListBase( font_base );
      char text[64]; sprintf( text, "FIRST 2D drawing with a fixed pipeline!" );
      glCallLists( strlen(text), GL_UNSIGNED_BYTE, (GLubyte *) text );
   }
   while(( glerr = glGetError() ) != GL_NO_ERROR ) {
      const char* errorString = inGetGLErrorString(glerr);
      printf("Draw1 error: %d \"%s\"\n", glerr, errorString );
   }
   // ====== endof the early rasterization of text stuff ======

   // ====== the main rasterization ======
   // manage fog for depth perception in the fixed pipeline //
#define NO_USE_FOG_
#ifdef _USE_FOG_
   GLfloat clr[3] = {0.2, 0.2, 0.2};
   GLfloat f;
   glHint(GL_FOG_HINT, GL_NICEST);
   glFogi(GL_FOG_MODE, GL_EXP);
   glFogf(GL_FOG_DENSITY, 0.3);
   glFogfv(GL_FOG_COLOR, clr);
   f = -1.0;
   glFogf(GL_FOG_START, f);
   f = 10.0;
   glFogf(GL_FOG_END, f);
#endif

   glMatrixMode( GL_PROJECTION );   // select the projection matrix mode
   glLoadIdentity();                // reset the proj. matrix
#define NO_ORTHO_
#ifdef _ORTHO_
//--- set an orthographic projection
{
   const GLdouble x0=-9.0,y0=-9.0,x1=9.0,y1=9.0,xn=-49.0,xf=49.0;
   const GLdouble aspect = ((double) width_)/((double) height_);
   glOrtho( x0*aspect,x1*aspect, y0,y1, xn,xf );
}
#else
//--- set a perspective projection (over-writing the projection matrix)
   gluPerspective( 30.0, ((GLfloat) width_)/((GLfloat) height_), 0.01, 40.0 );
#endif

   glMatrixMode( GL_MODELVIEW );    // Select the modelview matrix stack
   glLoadIdentity();                // reset the modelv. matrix

   glTranslatef( 0.0, 0.0, -5.0 );
{  // a simple way of making a perpetual rotation
   static float xang = 0.0;
   static float yang = 0.0;
   glRotatef( xang, 1.0, 0.0, 0.0);
   glRotatef( yang, 0.0, 1.0, 0.0);
   glRotatef( 0.0 , 0.0, 0.0, 1.0);
   yang += 0.5;
}

   // ---- programmable pipeline drawing ----
   // (this shows that we can mix the programmable and the fixed pipelines!)
   program_display();

   // fixed pipeline preparation
   prepare();
   // fixed pipeline drawing
#ifdef _USE_FOG_
   glEnable(GL_FOG);
#endif
   (void) DrawGridbox();
#ifdef _USE_FOG_
   glDisable(GL_FOG);
#endif
   // fixed pipeline clean-up
   unprepare();
   // ---- endof programmable pipeline drawing ----

   // ====== endof the main rasterization ======


   // ------ the late rasterization of text stuff ----
   glMatrixMode( GL_PROJECTION );   // select the projection matrix mode
   glLoadIdentity();                // reset the proj. matrix
{  const GLdouble x0=0.0,y0=0.0,x1=2.0,y1=2.0,xn=-10.0,xf=10.0;; // HACK
   glOrtho( x0,x1, y0,y1, xn,xf );
}
   glMatrixMode( GL_MODELVIEW );    // Select the modelview matrix stack
   glLoadIdentity();                // reset the modelv. matrix
{  // show 2D text rasterization (as "overlay")
   GLfloat rpos[4];    // THIS WILL BE DELETED TOO
   rpos[0] = 0.8;
   rpos[1] = 0.1;
   rpos[2] = 0.5;
   rpos[3] = 1.0;
   glColor4f( 0.4, 0.4, 1.0, 1.0 );
   glRasterPos3fv( rpos );
   glListBase( font_base );
   char text[64]; sprintf( text, "LAST 2D drawing with the fixed pipeline!" );
   glCallLists( strlen(text), GL_UNSIGNED_BYTE, (GLubyte *) text );
}
   // ------ endof the late rasterization of text stuff ----


#ifdef _SCISSOR_
   glDisable( GL_SCISSOR_TEST );
#endif

// printf(" INFO: %d %d %d %d \n", iloc_, jloc_, width_, height_ );//HACK
}

