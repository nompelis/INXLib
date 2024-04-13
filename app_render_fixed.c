// This file contains some of the code that is uses to render using the
// fixed pipeline; the high-level rendering is done with the fixed pipeline
// via the "app_draw()" function in the "app.c" file, which calls functions
// from here and to use the programmable pipeline. (Nothing 3D is rendered
// with what is here, and the functions are just placeholders.)

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/stat.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include <math.h>

//
// Function to draw a reference box using the fixed pipeline
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
// ----- a function to prepare the fixed rasterization pipeline
//
void prepare(void)
{
   glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE );
   // controls specular reflections
   glLightModeli( GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE );
   // order of specular light processing
   glLightModeli( GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR );
// glLightModeli( GL_LIGHT_MODEL_COLOR_CONTROL, GL_SINGLE_COLOR );
   // ambient light regardless of lights present
   GLfloat ambient_light[] = { 0.1, 0.1, 0.1, 1.0 };
   glLightModelfv( GL_LIGHT_MODEL_AMBIENT, ambient_light );

   GLfloat position[] = { -12.1, 19.1, 19.1, 1.0 };
//static float r1 = 0.0;
//r1 += 0.05*acos(-1.0);
//position[0] += 2.0*cos((double) r1);
//position[1] += 2.0*sin((double) r1);
   glLightfv( GL_LIGHT0, GL_POSITION, position );
   GLfloat ambient[] = { 0.02, 0.02, 0.02, 1.0 };
   GLfloat diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat specular[] = { 0.0, 0.0, 0.0, 1.0 };   // For now...
   glLightfv( GL_LIGHT0, GL_AMBIENT, ambient );
   glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuse );
   glLightfv( GL_LIGHT0, GL_SPECULAR, specular );
   glLightf( GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.0 );
   glLightf( GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.05 );
   glEnable( GL_LIGHT0 );


   GLfloat col[] = { 1.0, 0.2, 0.2 };
   glMaterialfv( GL_FRONT, GL_AMBIENT, col );
// glMaterialfv( GL_BACK,  GL_AMBIENT, col );
   glMaterialfv( GL_FRONT, GL_DIFFUSE, col );
// glMaterialfv( GL_BACK,  GL_DIFFUSE, col );
   col[1] = 1.0; col[2] = 1.0;
   glMaterialfv( GL_FRONT, GL_SPECULAR, col );
// glMaterialfv( GL_BACK,  GL_SPECULAR, col );
}

//
// ----- a function to "undo stuff" from the fixed rasterization pipeline
//
void unprepare(void)
{

}

