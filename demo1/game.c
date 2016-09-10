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

#include <math.h>
#include <string.h>

#include "game.h"     // defines the struct

// game variables are persistent and global
extern struct my_game_vars gvars;

//
// Function to draw a reference box
//

int DrawBox() {

   GLfloat col[3];
   GLfloat blue[] = { 0.2, 0.2, 1.0 };
   GLfloat d = 0.5;
   GLfloat x1[3],x2[3],x3[3],x4[3];
   GLfloat x5[3],x6[3],x7[3],x8[3];

   // colour
   col[0] = 0.8;
   col[1] = 0.8;
   col[2] = 0.8;

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

   glLineWidth( 1.0 );
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

   // --- lower box
   x5[1] -= 2.1*d;
   x6[1] -= 2.1*d;
   x7[1] -= 2.1*d;
   x8[1] -= 2.1*d;
   glBegin(GL_LINE_LOOP);
   glColor3fv( blue );

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

   return(0);
}

//
// Function to draw the sphere
//

int DrawBall( int im, int jm,
              double *x, double *y, double *z,
              double *u, double *v, double *w ) {

   GLfloat col[3];
   GLfloat white[] = { 1.0, 1.0, 1.0 };
   GLfloat black[] = { 0.0, 0.0, 0.0 };
   GLfloat xx[3],xn[3];
   GLfloat shine[] = { 100.0 };
   int i,j,n;

   // colour
   col[0] = 0.8;
   col[1] = 0.2;
   col[2] = 0.2;

   // set polygon drawing attributes
   glEnable( GL_CULL_FACE );
   glPolygonMode( GL_FRONT, GL_FILL );
   glPolygonMode( GL_BACK, GL_LINE );
   glFrontFace( GL_CCW );

   glMaterialfv( GL_FRONT, GL_AMBIENT, col );
   glMaterialfv( GL_FRONT, GL_DIFFUSE, col );
   glMaterialfv( GL_FRONT, GL_SPECULAR, white );
   glMaterialfv( GL_FRONT, GL_SHININESS, shine );

   // triangles from vertices of the sphere 2D manifold
   glBegin(GL_TRIANGLES);
   glColor3fv( col );

   for(i=0;i<im-1;++i) {
      for(j=0;j<jm-1;++j) {
         n = i*jm + j;
         xn[0] = (GLfloat) u[n];
         xn[1] = (GLfloat) v[n];
         xn[2] = (GLfloat) w[n];
         glNormal3fv( xn );
         xx[0] = (GLfloat) x[n];
         xx[1] = (GLfloat) y[n];
         xx[2] = (GLfloat) z[n];
         glVertex3fv( xx );

         n = (i+1)*jm + j;
         xn[0] = (GLfloat) u[n];
         xn[1] = (GLfloat) v[n];
         xn[2] = (GLfloat) w[n];
         glNormal3fv( xn );
         xx[0] = (GLfloat) x[n];
         xx[1] = (GLfloat) y[n];
         xx[2] = (GLfloat) z[n];
         glVertex3fv( xx );

         n = i*jm + j+1;
         xn[0] = (GLfloat) u[n];
         xn[1] = (GLfloat) v[n];
         xn[2] = (GLfloat) w[n];
         glNormal3fv( xn );
         xx[0] = (GLfloat) x[n];
         xx[1] = (GLfloat) y[n];
         xx[2] = (GLfloat) z[n];
         glVertex3fv( xx );


         n = i*jm + j+1;
         xn[0] = (GLfloat) u[n];
         xn[1] = (GLfloat) v[n];
         xn[2] = (GLfloat) w[n];
         glNormal3fv( xn );
         xx[0] = (GLfloat) x[n];
         xx[1] = (GLfloat) y[n];
         xx[2] = (GLfloat) z[n];
         glVertex3fv( xx );

         n = (i+1)*jm + j;
         xn[0] = (GLfloat) u[n];
         xn[1] = (GLfloat) v[n];
         xn[2] = (GLfloat) w[n];
         glNormal3fv( xn );
         xx[0] = (GLfloat) x[n];
         xx[1] = (GLfloat) y[n];
         xx[2] = (GLfloat) z[n];
         glVertex3fv( xx );

         n = (i+1)*jm + j+1;
         xn[0] = (GLfloat) u[n];
         xn[1] = (GLfloat) v[n];
         xn[2] = (GLfloat) w[n];
         glNormal3fv( xn );
         xx[0] = (GLfloat) x[n];
         xx[1] = (GLfloat) y[n];
         xx[2] = (GLfloat) z[n];
         glVertex3fv( xx );
      }
   }

   glEnd();

   return(0);
}


//
// Function to draw the shadow of the sphere
//

int DrawShadow( int im, int jm,
                double *x, double *y, double *z )
{
   GLfloat col[3];
   GLfloat xx[3],xn[3];
   GLfloat black[] = { 0.0, 0.0, 0.0 };
   GLfloat shine[] = { 50.0 };
   int i,j,n;

   // colour
   col[0] = 0.1;
   col[1] = 0.1;
   col[2] = 0.1;

   // set polygon drawing attributes
   glEnable( GL_CULL_FACE );
   glPolygonMode( GL_FRONT, GL_FILL );
   glPolygonMode( GL_BACK, GL_LINE );
   glFrontFace( GL_CCW );

   glMaterialfv( GL_FRONT, GL_AMBIENT, col );
   glMaterialfv( GL_FRONT, GL_DIFFUSE, col );
   glMaterialfv( GL_FRONT, GL_SPECULAR, black );
   glMaterialfv( GL_FRONT, GL_SHININESS, shine );

   // triangles from vertices of the sphere 2D manifold
   glBegin(GL_TRIANGLES);
   glColor3fv( col );

   xn[0] = 0.0;
   xn[1] = 1.0;
   xn[2] = 0.0;
   xx[1] = -0.5;
   for(i=0;i<im-1;++i) {
      j = jm/2;
      {
         glNormal3fv( xn );
         xx[0] = 0.0;
         xx[2] = 0.0;
         glVertex3fv( xx );

         n = (i+1)*jm + j;
         glNormal3fv( xn );
         xx[0] = (GLfloat) x[n];
         xx[2] = (GLfloat) z[n];
         glVertex3fv( xx );

         n = i*jm + j;
         glNormal3fv( xn );
         xx[0] = (GLfloat) x[n];
         xx[2] = (GLfloat) z[n];
         glVertex3fv( xx );
      }
   }

   glEnd();

   return(0);
}


//
// Function to draw the paddle
//

int DrawPaddle() {

   GLfloat col[3];
   GLfloat d = 0.1;
   GLfloat x1[3],x2[3],x3[3],x4[3];
   GLfloat x5[3],x6[3],x7[3],x8[3];

   // colour
   col[0] = 0.5;
   col[1] = 0.5;
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

   glLineWidth( 2.0 );
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

   return(0);
}


//
// Function to place the light in the scene
//

int place_light()
{
   GLfloat ambient[] = {0.0, 0.2, 0.0, 1.0};  // Hardwired dim-green colour
   GLfloat white[] = {1.0, 1.0, 1.0, 1.0};  // Hardwired white colour
   GLfloat position[] = {8.0, 8.0,-10.0, 1.0}; // Hardwired light position

   /* set light position and colours */
   glLightfv( GL_LIGHT0, GL_POSITION, position );
   glLightfv( GL_LIGHT0, GL_AMBIENT, ambient );
   glLightfv( GL_LIGHT0, GL_DIFFUSE, white );
   glLightfv( GL_LIGHT0, GL_SPECULAR, white );
   glLightf( GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.8f );
   glLightf( GL_LIGHT0, GL_LINEAR_ATTENUATION,   0.0f );
   glEnable( GL_LIGHT0 );

   return(0);
}


//
// Function to keep track of user score
//

void collide_paddle( )
{
   // This function is left up to the user as an exercise. ;-)
   // Code it and place it appropriately in the code to keep track of when
   // the paddle touches the ball and terminate the game if the ball hits
   // the blue slab without touching the paddle. Demonstrate that your code
   // works and receive a free one-year license to any of my commercial codes.
}


//
// Function to update the ball's position in the box
//

void update_ball( GLfloat *position )
{
   static int istart = 0;
   static float vel_vec[3], new_pos[3];
   static float dt = 0.01;

   dt += 0.00005;

   // initialization
   if( istart == 0 ) {
      new_pos[0] = 0.4*cosf( (float) rand() );
      new_pos[1] = 0.4*cosf( (float) rand() );
      new_pos[2] = 0.4*cosf( (float) rand() );

      vel_vec[0] = cosf( (float) rand() );
      vel_vec[1] = cosf( (float) rand() );
      vel_vec[2] = cosf( (float) rand() );

      istart = 1;
   }

   printf("Vel: %f %f %f \n",vel_vec[0], vel_vec[1], vel_vec[2] );
   printf("Pos: %f %f %f \n",position[0], position[1], position[2] );

   // update position from velocity vector
   new_pos[0] += vel_vec[0] * dt;
   new_pos[1] += vel_vec[1] * dt;
   new_pos[2] += vel_vec[2] * dt;

   // checks for collisions
   if( new_pos[0] >= +(0.5-0.1) ) {
      new_pos[0] = - new_pos[0] + 2.0*(0.5-0.1);
      vel_vec[0] = - vel_vec[0];
   }
   if( new_pos[0] <= -(0.5-0.1) ) {
      new_pos[0] = - new_pos[0] - 2.0*(0.5-0.1);
      vel_vec[0] = - vel_vec[0];
   }

   if( new_pos[1] >= +(0.5-0.1) ) {
      new_pos[1] = - new_pos[1] + 2.0*(0.5-0.1);
      vel_vec[1] = - vel_vec[1];
   }
   if( new_pos[1] <= -(0.5-0.1) ) {
      new_pos[1] = - new_pos[1] - 2.0*(0.5-0.1);
      vel_vec[1] = - vel_vec[1];
      (void) collide_paddle( new_pos[0], new_pos[2] );
   }

   if( new_pos[2] >= +(0.5-0.1) ) {
      new_pos[2] = - new_pos[2] + 2.0*(0.5-0.1);
      vel_vec[2] = - vel_vec[2];
   }
   if( new_pos[2] <= -(0.5-0.1) ) {
      new_pos[2] = - new_pos[2] - 2.0*(0.5-0.1);
      vel_vec[2] = - vel_vec[2];
   }

   position[0] = new_pos[0];
   position[1] = new_pos[1];
   position[2] = new_pos[2];
}


//
// Function to draw the scene; it also acts as the game engine
//

void game_draw( int iloc_, int jloc_,
                unsigned int width_, unsigned int height_,
                unsigned int font_base, Display *xdisplay, Window xwindow )
{
   char text[] = "Game demo Copyright 2016, Ioannis Nompelis";
   GLfloat mvt[] = {1.0, 0.0, 0.0, 0.0,
                    0.0, 1.0, 0.0, 0.0,
                    0.0, 0.0, 1.0, 0.0,
                    0.0, 0.0, 0.0, 1.0};
   GLfloat ambient[] = {0.2, 0.2, 0.2, 1.0};  // Hardwired dim-white colour
   GLfloat diffuse[] = {1.0, 1.0, 1.0, 1.0};  // Hardwired white colou

   GLfloat position[3];

   update_ball( position );

   glShadeModel(GL_SMOOTH);
   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
   glClearColor( 0.15, 0.15, 0.15, 1.0 );
   glClearDepth(10.0);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glViewport( iloc_, jloc_, width_, height_ );

   glEnable(GL_SCISSOR_TEST);
   glScissor( (GLint) (iloc_ -1), (GLint) (jloc_ -1),
              (GLsizei) (width_ +1), (GLsizei) (height_ +1) );

   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LEQUAL);

   glShadeModel( GL_SMOOTH );
   glEnable( GL_MULTISAMPLE );

   glEnable( GL_LINE_SMOOTH );
   glEnable( GL_POLYGON_SMOOTH );
   glHint( GL_POINT_SMOOTH_HINT, GL_NICEST );
   glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
   glHint(  GL_POLYGON_SMOOTH_HINT, GL_NICEST );
   glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

   glMatrixMode(GL_PROJECTION);   // select the projection matrix mode
   glLoadIdentity();              // reset the proj. matrix
#define PERSPECTIVE
#ifdef PERSPECTIVE
   gluPerspective( 30.0, ((GLfloat) width_)/((GLfloat) height_), 0.001, 20.0 );
#else
   glOrtho( -1.5,1.5, -1.5 * ((GLdouble) height_)/((GLdouble) width_),
                       1.5 * ((GLdouble) height_)/((GLdouble) width_),
                             -10.0,10.0 );
#endif

   glMatrixMode(GL_MODELVIEW);    // Select the modelview matrix stack
   glLoadIdentity();              // reset the modelv. matrix

   // setting the lighting model (should be done once)
   glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
   glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
   glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);   // Real lighting
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
   // setting the light and its position in space
   (void) place_light();

   glTranslatef( 0.0, 0.0, -3.0 );

// uncomment the following and get dizzy!
/*  {
   static float xang = 0.0;
   static float yang = 0.0;
   glRotatef( xang, 1.0, 0.0, 0.0);
   glRotatef( yang, 0.0, 1.0, 0.0);
   xang += 0.5;
   yang += 0.5;
}  */

   (void) DrawBox();

   glEnable(GL_LIGHTING);
   glPushMatrix();
   glTranslatef( position[0], position[1], position[2] );
   (void) DrawBall( gvars.im, gvars.jm, gvars.sx, gvars.sy, gvars.sz,
                                        gvars.su, gvars.sv, gvars.sw );

   glPopMatrix();

   glDisable(GL_LIGHTING);

   glPushMatrix();
   glTranslatef( position[0], 0.0, position[2] );
   (void) DrawShadow( gvars.im, gvars.jm, gvars.sx, gvars.sy, gvars.sz );
   glPopMatrix();

   glDisable(GL_LIGHTING);

   glPushMatrix();
   // variables gvars.px,gvarsp.pz are updated in inxlib_user.c
   glTranslatef( gvars.px, -0.6, gvars.pz );
   (void) DrawPaddle();
   glPopMatrix();

   glWindowPos3i( 10, 10, 0 );
   glListBase( font_base );
   glCallLists( strlen(text), GL_UNSIGNED_BYTE, (GLubyte *) text);

   glDisable(GL_SCISSOR_TEST);

   printf(" INFO: %d %d %d %d \n", iloc_, jloc_, width_, height_ );
}

