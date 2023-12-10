#include <math.h>
#include <string.h>
#include <GL/gl.h>
#include <GL/glu.h>

//
// This file contains all the setting up of the programmable pipeline
//
#include "my_demo.c"

//
// Function to draw a reference box
//

int DrawGridbox()
{

   GLfloat col[3];
   GLfloat d = 0.5;
   GLfloat x1[3],x2[3],x3[3],x4[3];
   GLfloat x5[3],x6[3],x7[3],x8[3];

   // colour
   col[0] = 1.0;
   col[1] = 1.0;
   col[2] = 1.0;

   // lower points
   x1[0] = -d;
   x1[1] = -d;
   x1[2] =- d;

   x2[0] =  d;
   x2[1] = -d;
   x2[2] =- d;

   x3[0] =  d;
   x3[1] = -d;
   x3[2] =+ d;

   x4[0] = -d;
   x4[1] = -d;
   x4[2] =+ d;

   // upper points
   x5[0] = -d;
   x5[1] =  d;
   x5[2] =- d;

   x6[0] =  d;
   x6[1] =  d;
   x6[2] =- d;

   x7[0] =  d;
   x7[1] =  d;
   x7[2] =+ d;

   x8[0] = -d;
   x8[1] =  d;
   x8[2] =+ d;


   glBegin(GL_LINE_LOOP);
   glColor3fv( col );

   glVertex3fv(x1);
   glVertex3fv(x2);
   glVertex3fv(x3);
   glVertex3fv(x4);

   glVertex3fv(x1);
   glVertex3fv(x5);
   glVertex3fv(x8);
   glVertex3fv(x4);

   glVertex3fv(x1);
   glVertex3fv(x2);
   glVertex3fv(x6);
   glVertex3fv(x5);

   glVertex3fv(x5);
   glVertex3fv(x6);
   glVertex3fv(x7);
   glVertex3fv(x8);

   glVertex3fv(x8);
   glVertex3fv(x7);
   glVertex3fv(x3);
   glVertex3fv(x4);

   glVertex3fv(x1);
   glEnd();

   return 0;
}


//
// a function that does standard "fixed pipeline" raterization
//

void dummy_draw( int iloc_, int jloc_,
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

// ------ an early rasterization of text stuff ----
   glMatrixMode( GL_PROJECTION );   // select the projection matrix mode
   glLoadIdentity();                // reset the proj. matrix
{  // making the projection matrix from an orthographic projection (for tests)
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

/*{  // making a ring of words on the 2D rasterization (as "overlay")
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
}*/
{  // show 2D text rasterization (as "overlay")
   GLfloat rpos[4];    // THIS WILL BE DELETED TOO
   rpos[0] = 0.1;
   rpos[1] = 0.1;
   rpos[2] = 0.5;
   rpos[3] = 1.0;
   glColor4f( 1.0, 1.0, 1.0, 1.0 );
   glRasterPos3fv( rpos );
   glListBase( font_base );
   char text[64]; sprintf( text, "FIRST 2D drawing with the fixed pipeline!" );
   glCallLists( strlen(text), GL_UNSIGNED_BYTE, (GLubyte *) text );
}
   while(( glerr = glGetError() ) != GL_NO_ERROR ) {
      const char* errorString = inGetGLErrorString(glerr);
      printf("Draw1 error: %d \"%s\"\n", glerr, errorString );
   }
// ------ endof the early rasterization of text stuff ----

// ------ the main rasterization -------
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
   const GLdouble x0=-9.0,y0=-9.0,x1=9.0,y1=9.0,xn=-49.0,xf=49.0;; // HACK 1
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
{  // a simple way of making a constant rotation
   static float xang = 0.0;
   static float yang = 0.0;
   glRotatef( xang, 1.0, 0.0, 0.0);
   glRotatef( yang, 0.0, 1.0, 0.0);
   glRotatef( 0.0 , 0.0, 0.0, 1.0);
// xang += 0.5;
   yang += 0.5;
}

   // programmable pipeline drawing (showing mixing with the fixed pipeline)
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

// ------ endof the main rasterization -------


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

   printf(" INFO: %d %d %d %d \n", iloc_, jloc_, width_, height_ );//HACK
}

