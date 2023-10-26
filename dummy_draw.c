#include <math.h>
#include <string.h>
#include <GL/glu.h>


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


void dummy_draw( int iloc_, int jloc_,
                 unsigned int width_, unsigned int height_,
                 unsigned int font_base, Display *xdisplay, Window xwindow )
{
   GLfloat mvt[] = {1.0, 0.0, 0.0, 0.0,
                    0.0, 1.0, 0.0, 0.0,
                    0.0, 0.0, 1.0, 0.0,
                    0.0, 0.0, 0.0, 1.0};

   glShadeModel( GL_SMOOTH );
   glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
   glClearColor( 0.15, 0.15, 0.15, 1.0 );
   glClearDepth( 40.0 );
   glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

   glViewport( iloc_, jloc_, width_, height_ );

   glEnable( GL_SCISSOR_TEST );
   glScissor( (GLint) (iloc_ -1), (GLint) (jloc_ -1),
              (GLsizei) (width_ +1), (GLsizei) (height_ +1) );

   glMatrixMode(GL_PROJECTION);   // select the projection matrix mode
   glLoadIdentity();              // reset the proj. matrix

{
   const GLdouble x0=0.0,y0=0.0,x1=2.0,y1=2.0,xn=-10.0,xf=10.0;; // HACK
   glMultMatrixf( mvt );
   glOrtho( x0,x1, y0,y1, xn,xf );
}

   glMatrixMode(GL_MODELVIEW);    // Select the modelview matrix stack
   glLoadIdentity();              // reset the modelv. matrix
   glMultMatrixf( mvt );

   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LEQUAL);

   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glDisable(GL_CULL_FACE);

   glDisable(GL_LIGHTING);


/**/{
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
   glColor3f(1.0,c,c);
//printf("%lf %lf %lf \n",rpos[0],rpos[1],rpos[2]);//HACK

   glRasterPos3fv( rpos );
   glListBase( font_base );
   char text[10]; sprintf(text, "DEMO");
   glCallLists( strlen(text), GL_UNSIGNED_BYTE, (GLubyte *) text );
   }
}/**/

   // manage fog for depth perception //
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

   glMatrixMode(GL_PROJECTION);   // select the projection matrix mode
   glLoadIdentity();              // reset the proj. matrix
//--- set an orthographic projection
{
   const GLdouble x0=-1.0,y0=-1.0,x1=1.0,y1=1.0,xn=-4.0,xf=4.0;; // HACK 1
   glOrtho( x0,x1, y0,y1, xn,xf );
}
//--- set a perspective projection (over-writing the projection matrix)
   glLoadIdentity();              // reset the proj. matrix
   gluPerspective( 30.0, ((GLfloat) width_)/((GLfloat) height_), 0.01, 40.0 );

   glMatrixMode(GL_MODELVIEW);    // Select the modelview matrix stack
   glLoadIdentity();              // reset the modelv. matrix

   glTranslatef( 0.0, 0.0, -5.0 ); //HACK 2
{
   static float xang = 0.0;
   static float yang = 0.0;
   glRotatef( xang, 1.0, 0.0, 0.0);
   glRotatef( yang, 0.0, 1.0, 0.0);
   glRotatef( 0.0 , 0.0, 0.0, 1.0);
   xang += 0.5;
   yang += 0.5;
}

   glEnable(GL_FOG);
(void) DrawGridbox();
   glDisable(GL_FOG);


   glDisable( GL_SCISSOR_TEST );

   printf(" INFO: %d %d %d %d \n", iloc_, jloc_, width_, height_ );//HACK
}

