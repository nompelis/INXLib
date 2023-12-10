//
// A utility to make a map-style sphere with normals and texels
///
/******************************************************************************
 Copyright (c) 2016-2023, Ioannis Nompelis
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
#include <math.h>

//
// Function that makes a spherical shell surface of structured quadrilaterals
// with a degenerate axis singularity at jmin and jmax. It also generates the
// texels for appropriate texture-mapping. The numbers "im", "jm" are the
// node counts in the i and j directions; the last index in each direction
// overlaps the first of the structured grid.
//

int inMakeAxisSphereshell1( int im, int jm,
                            double **x, double **y, double **z,
                            double **u, double **v, double **w,
                            float **s, float **t )
{
   int i,j,n;
   size_t isize;
   double pi,angle,azimuth;
   double ksi,eta;


   pi = acos(-1.0);

   isize = im*jm;
   *x = (double *) malloc(isize*sizeof(double));
   *y = (double *) malloc(isize*sizeof(double));
   *z = (double *) malloc(isize*sizeof(double));
   *u = (double *) malloc(isize*sizeof(double));
   *v = (double *) malloc(isize*sizeof(double));
   *w = (double *) malloc(isize*sizeof(double));
   *s = (float *) malloc(isize*sizeof(float));
   *t = (float *) malloc(isize*sizeof(float));
   if( *x == NULL || *y == NULL || *z == NULL ||
       *u == NULL || *v == NULL || *w == NULL ||
       *s == NULL || *t == NULL ) {
      fprintf( stdout," Error: could not allocate arrays \n" );
      if( *x != NULL ) free( *x );
      if( *y != NULL ) free( *y );
      if( *z != NULL ) free( *z );
      if( *u != NULL ) free( *u );
      if( *v != NULL ) free( *v );
      if( *w != NULL ) free( *w );
      if( *s != NULL ) free( *s );
      if( *t != NULL ) free( *t );
      return 1;
   }

   for(i=0;i<im;++i) {
      ksi = ((double) i)/ ((double) (im-1));
      angle = 2.0*pi * ksi;

      for(j=0;j<jm;++j) {
         eta = ((double) j)/ ((double) (jm-1));
         azimuth = pi * (1.0 - eta);

         n = i*jm + j;

         (*x)[n] = cos(angle)*sin(azimuth);
         (*y)[n] = cos(azimuth);
         (*z)[n] = sin(angle)*sin(azimuth);
         // normals
         (*u)[n] = (*x)[n];
         (*v)[n] = (*y)[n];
         (*w)[n] = (*z)[n];
         // texels
         (*s)[n] = (float) ksi;
         (*t)[n] = (float) eta;

      }
   }

   return 0;
}


//
// Function to plot the structured manifold in TecPlot or ParaView
//

int inDumpTecplotShell1( char *fname, int im, int jm,
                         const double *x, const double *y, const double *z,
                         const double *u, const double *v, const double *w,
                         const float *s, const float *t )
{
   int i,j;
   FILE *fp;

   fp = fopen( fname, "w" );
   if( fp == NULL ) {
      fprintf( stdout, " Error: could not create file \"%s\" \n", fname );
      return 1;
   }

   fprintf( fp, "VARIABLES = x y z u v w s t \n");
   fprintf( fp, "ZONE T=\"sphere shell\", i=%d, j=%d,k=1, f=point \n", im,jm );

   for(j=0;j<jm;++j) {
      for(i=0;i<im;++i) {
         int n = i*jm + j;

         fprintf( fp," %lf %lf %lf \n", x[n], y[n], z[n] );
         fprintf( fp," %lf %lf %lf \n", u[n], v[n], w[n] );
         fprintf( fp," %f %f \n", s[n], t[n] );
      }
   }

   fclose( fp );

   return 0;
}


// 
// Function that makes a spherical shell surface of "structured" triangles
// with a degenerate axis singularity at jmin and jmax. It also generates the
// texels for appropriate texture-mapping. The numbers "im", "jm" are the
// node counts in the i and j directions; the last index in each direction
// overlaps the first of the structured grid. There are two disjoint sets of
// triangles of zero area (at the ends along the degenerate axis).
//

int inMakeAxisSphereshell2( int im, int jm,
                            double **x, double **y, double **z,
                            double **u, double **v, double **w,
                            float **s, float **t, int **ie )
{
   int i,j,n;
   size_t isize;
   double pi,angle,azimuth;
   double ksi,eta;


   pi = acos(-1.0);

   isize = im*jm;
   *x = (double*) malloc(isize*sizeof(double));
   *y = (double*) malloc(isize*sizeof(double));
   *z = (double*) malloc(isize*sizeof(double));
   *u = (double*) malloc(isize*sizeof(double));
   *v = (double*) malloc(isize*sizeof(double));
   *w = (double*) malloc(isize*sizeof(double));
   *s = (float*) malloc(isize*sizeof(float));
   *t = (float*) malloc(isize*sizeof(float));
   *ie = (int*) malloc( 2 * isize * 3 * sizeof(float) );
   if( *x == NULL || *y == NULL || *z == NULL ||
       *u == NULL || *v == NULL || *w == NULL ||
       *s == NULL || *t == NULL|| *ie == NULL ) {
      fprintf( stdout," Error: could not allocate arrays \n" );
      if( *x != NULL ) free( *x );
      if( *y != NULL ) free( *y );
      if( *z != NULL ) free( *z );
      if( *u != NULL ) free( *u );
      if( *v != NULL ) free( *v );
      if( *w != NULL ) free( *w );
      if( *s != NULL ) free( *s );
      if( *t != NULL ) free( *t );
      if( *ie != NULL ) free( *ie );
      return 1;
   }

   for(i=0;i<im;++i) {
      ksi = ((double) i)/ ((double) (im-1));
      angle = 2.0*pi * ksi;

      for(j=0;j<jm;++j) {
         eta = ((double) j)/ ((double) (jm-1));
         azimuth = pi * (1.0 - eta);

         n = i*jm + j;

         (*x)[n] = cos(angle)*sin(azimuth);
         (*y)[n] = cos(azimuth);
         (*z)[n] = sin(angle)*sin(azimuth);
         // normals
         (*u)[n] = (*x)[n];
         (*v)[n] = (*y)[n];
         (*w)[n] = (*z)[n];
         // texels
         (*s)[n] = (float) ksi;
         (*t)[n] = (float) eta;

      }
   }

   int* ic = *ie;
   int ne=0;
   for(i=0;i<im-1;++i) {
      for(j=0;j<jm-1;++j) {
         n = i*jm + j;

         ic[ 3*ne  ] = n;
         ic[ 3*ne+1] = n + jm;
         ic[ 3*ne+2] = n + 1;
         ++ne;

         ic[ 3*ne  ] = n + 1;
         ic[ 3*ne+1] = n + jm;
         ic[ 3*ne+2] = n + jm + 1;
         ++ne;
      }
   }

   return 0;
}


//
// Function to plot the triangulated manifold in TecPlot or ParaView
//

int inDumpTecplotShell2( char *fname, int im, int jm,
                         const double *x, const double *y, const double *z,
                         const double *u, const double *v, const double *w,
                         const float *s, const float *t, const int* ie )
{
   int n, ne = 2*(im-1)*(jm-1);
   FILE *fp;

   fp = fopen( fname, "w" );
   if( fp == NULL ) {
      fprintf( stdout, " Error: could not create file \"%s\" \n", fname );
      return 1;
   }

   fprintf( fp, "VARIABLES = x y z u v w s t \n");
   fprintf( fp, "ZONE T=\"sphere shell\", NODES=%d, ELEMENTS=%d, \n", im*jm,ne);
   fprintf( fp, "     ZONETYPE=FETRIANGLE, DATAPACKING=POINT \n" );

   for(n=0;n<im*jm;++n) {
      fprintf( fp," %f %f %f   ", x[n], y[n], z[n] );
      fprintf( fp," %f %f %f   ", u[n], v[n], w[n] );
      fprintf( fp," %f %f \n", s[n], t[n] );
   }
   for(n=0;n<ne;++n) {
      fprintf( fp," %d %d %d \n", ie[3*n]+1, ie[3*n+1]+1, ie[3*n+2]+1 );
   }

   fclose( fp );

   return 0;
}


// 
// Function that makes a spherical shell surface of "structured" triangles
// that have independent vertices (for OpenGL) rendering). It also generates
// the texels for appropriate texture-mapping. The numbers "im", "jm" are the
// node counts in the i and j directions; the last index in each direction
// overlaps the first of the structured grid. However, the triangles are all
// independent and there are none with zero area. The returned array stores
// positions, color, normal vector components, and texels. Connectivity is
// also returned.
//

int inMakeAxisSphereshell3( int im, int jm, float **x )
{
   int i,j;
   size_t isize,ioff;
   double pi,angle,azimuth;
   double ksi,eta;


   pi = acos(-1.0);

   // "jm-2" regular "im-1" 2-triangle bands, plus 2 bands of "im-1" triangles
   int ne = 2*(im-1)*(jm-1 -2) + 2*(im-1);
   isize = (size_t) ne;
   ioff = 3 + 3 + 2 + 4;   // 3 pos. + 3 normal + 2 texel + 4 color (RGBA)
   *x = (float*) malloc( 3 * isize * ioff * sizeof(float) );
   if( *x == NULL ) {
      fprintf( stdout," Error: could not allocate arrays \n" );
      return 1;
   }
#ifdef _DEBUG_
   fprintf( stdout, " [DEBUG]  Array of %ld floats (%ld bytes) \n",
            3*isize*ioff, 3*isize*ioff * sizeof(float) );
#endif

#define HACKJOB 0
   float* xx = *x;
   size_t nn=0,k;
   for(i=0;i<im-1;++i) {
      for(j=0;j<jm-1;++j) {
         float data[4][ioff];
         for(k=0;k<4;++k) {
            ksi = ((double) (i))/ ((double) (im-1));
            angle = 2.0*pi * ksi;
            eta = ((double) (HACKJOB+ j))/ ((double) (2*HACKJOB+ jm-1));
            azimuth = pi * (1.0 - eta);
            if( k == 1 ) {
               ksi = ((double) (i+1))/ ((double) (im-1));
               angle = 2.0*pi * ksi;
            } else if( k == 2 ) {
               eta = ((double) (HACKJOB+ j+1))/ ((double) (2*HACKJOB+ jm-1));
               azimuth = pi * (1.0 - eta);
            } else if( k == 3 ) {
               ksi = ((double) (i+1))/ ((double) (im-1));
               angle = 2.0*pi * ksi;
               eta = ((double) (HACKJOB+ j+1))/ ((double) (2*HACKJOB+ jm-1));
               azimuth = pi * (1.0 - eta);
            }

            // vertex position
            data[k][0] = (float) (cos(angle)*sin(azimuth));
            data[k][1] = (float) (cos(azimuth));
            data[k][2] = (float) (sin(angle)*sin(azimuth));
            // normal
            data[k][3] = data[0][0];
            data[k][4] = data[0][1];
            data[k][5] = data[0][2];
            // texell
            data[k][6] = (float) ksi;
            data[k][7] = (float) eta;
            // colour has latitudinal variations
            data[k][8]  = (float) (sin(azimuth) * sin(azimuth));
            data[k][9]  = (float) sin(2.0*azimuth);
            data[k][10] = (float) (cos(azimuth) * cos(azimuth));
            data[k][11] = 1.0; // full alpha (opaque)
         }

         if( j == 0 ) {
            for(k=0;k<ioff;++k) { xx[nn*ioff + k] = data[2][k]; }
            ++nn;
            for(k=0;k<ioff;++k) { xx[nn*ioff + k] = data[1][k]; }
            ++nn;
            for(k=0;k<ioff;++k) { xx[nn*ioff + k] = data[3][k]; }
            ++nn;
         } else if( j == jm-1-1 ) {
            for(k=0;k<ioff;++k) { xx[nn*ioff + k] = data[0][k]; }
            ++nn;
            for(k=0;k<ioff;++k) { xx[nn*ioff + k] = data[1][k]; }
            ++nn;
            for(k=0;k<ioff;++k) { xx[nn*ioff + k] = data[2][k]; }
            ++nn;
         } else {
            for(k=0;k<ioff;++k) { xx[nn*ioff + k] = data[0][k]; }
            ++nn;
            for(k=0;k<ioff;++k) { xx[nn*ioff + k] = data[1][k]; }
            ++nn;
            for(k=0;k<ioff;++k) { xx[nn*ioff + k] = data[2][k]; }
            ++nn;

            for(k=0;k<ioff;++k) { xx[nn*ioff + k] = data[1][k]; }
            ++nn;
            for(k=0;k<ioff;++k) { xx[nn*ioff + k] = data[2][k]; }
            ++nn;
            for(k=0;k<ioff;++k) { xx[nn*ioff + k] = data[3][k]; }
            ++nn;
         }
      }
   }

   return 0;
}

//
// Function to plot the Gl-rendering formatted data in TecPlot or ParaView
//

int inDumpTecplotShell3( char *fname, int ne, const float *x )
{
   int n;
   FILE *fp;

   fp = fopen( fname, "w" );
   if( fp == NULL ) {
      fprintf( stdout, " Error: could not create file \"%s\" \n", fname );
      return 1;
   }

   fprintf( fp, "VARIABLES = x y z  u v w  s t  R G B A \n");
   fprintf( fp, "ZONE T=\"sphere shell\", NODES=%d, ELEMENTS=%d, \n", 3*ne, ne);
   fprintf( fp, "     ZONETYPE=FETRIANGLE, DATAPACKING=POINT \n" );

   for(n=0;n<ne;++n) {
      for(int k=0;k<3;++k) {
         int m = (3*n + k)*12;
         fprintf( fp," %f %f %f   ", x[m], x[m+1], x[m+2] );
         fprintf( fp," %f %f %f   ", x[m+3], x[m+4], x[m+5] );
         fprintf( fp," %f %f   ", x[m+6], x[m+7] );
         fprintf( fp," %f %f %f %f \n", x[m+8], x[m+9], x[m+10], x[m+11] );
     }
   }
   for(n=0;n<ne;++n) {
      fprintf( fp," %d %d %d \n", 3*n+1, 3*n+2, 3*n+3 );
   }

   fclose( fp );

   return 0;
}




// Compile this file with "-DMAIN" to run it standalone
#ifdef _DRIVER_
int main() {
   int im = 40,jm = 20;
   double *x,*y,*z;
   double *u,*v,*w;
   float  *s,*t;
   int *ie;

// (void) inMakeAxisSphereshell1(im,jm,&x,&y,&z,&u,&v,&w,&s,&t);
// (void) inDumpTecplotShell1("shell1.dat",im,jm,x,y,z, u,v,w, s,t);
// free(x); free(y); free(z); free(u); free(v); free(w); free(s); free(t);

// (void) inMakeAxisSphereshell2(im,jm,&x,&y,&z,&u,&v,&w,&s,&t,&ie);
// (void) inDumpTecplotShell2("shell2.dat",im,jm,x,y,z, u,v,w, s,t,ie);
// free(x); free(y); free(z); free(u); free(v); free(w); free(s); free(t);
// free(ie);

   inMakeAxisSphereshell3( im, jm, &s );
   int ne = 2*(im-1)*(jm-1 -2) + 2*(im-1);
   (void) inDumpTecplotShell3("shell3.dat",ne,s);
   free(s);

   return 0;
}
#endif

