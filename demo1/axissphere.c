/*
 * A utility to make a map-style sphere with normals and texels
 * Ioannis Nompelis 2016/09/10
 */
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
#include <math.h>


/*
 * Function that makes a spherical shell surface of structured quadrilaterals
 * with a degenerate axis singularity at jmin and jmax. It also generates the
 * texels for appropriate texture-mapping.
 */

int inMakeAxisSphereshell(int im, int jm,
                          double **x, double **y, double **z,
                          double **u, double **v, double **w,
                          float **p, float **q)
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
   *p = (float *) malloc(isize*sizeof(float));
   *q = (float *) malloc(isize*sizeof(float));
   if(*x == NULL || *y == NULL || *z == NULL ||
      *u == NULL || *v == NULL || *w == NULL ||
      *p == NULL || *q == NULL) {
      fprintf(stdout," e  [inAxisSphereshell]  Could not allocate arrays \n");
      if(*x != NULL) free(*x);
      if(*y != NULL) free(*y);
      if(*z != NULL) free(*z);
      if(*u != NULL) free(*u);
      if(*v != NULL) free(*v);
      if(*w != NULL) free(*w);
      if(*p != NULL) free(*p);
      if(*q != NULL) free(*q);
      return(1);
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
         (*p)[n] = (float) ksi;
         (*q)[n] = (float) eta;

/*  
 // sheet
         (*x)[n] = ((double) i)/((double) im); 
         (*y)[n] = ((double) j)/((double) im);
         (*z)[n] = 0.0;
 // sheet normals
         (*u)[n] = 0.0;
         (*v)[n] = 0.0;
         (*w)[n] = 1.0;
  */

         // scale all vertices by a factor
         (*x)[n] = (*x)[n] * 0.1;
         (*y)[n] = (*y)[n] * 0.1;
         (*z)[n] = (*z)[n] * 0.1;
      }
   }

   return(0);
}


/*
 * Function to plot the geometry in TecPlot or ParaView
 */

int inDumpTecplotShell(char *fname, int im, int jm,
                        double *x, double *y, double *z,
                        double *u, double *v, double *w,
                        float *p, float *q)
{
   int i,j;
   FILE *fp;


   fp = fopen(fname,"w");
   if(fp == NULL) {
      fprintf(stdout," e  Could not create file \"%s\" \n",fname);
      return(1);
   }

   fprintf(fp,"variables = x y z u v w p q \n");
   fprintf(fp,"zone T=\"sphere shell\", i=%d, j=%d,k=1, f=point \n",im,jm);

   for(j=0;j<jm;++j) {
      for(i=0;i<im;++i) {
         int n = i*jm + j;

         fprintf(fp," %lf %lf %lf \n",x[n],y[n],z[n]);
         fprintf(fp," %lf %lf %lf \n",u[n],v[n],w[n]);
         fprintf(fp," %f  %f      \n",p[n],q[n]);
      }
   }

   fclose(fp);

   return(0);
}

// Compile this file with "-DMAIN" to run it standalone
#ifdef MAIN
int main() {
   int im = 40,jm = 20;
   double *x,*y,*z;
   double *u,*v,*w;
   float  *p,*q;

   (void) inMakeAxisSphereshell(im,jm,&x,&y,&z,&u,&v,&w,&p,&q);

   (void) inDumpTecplotShell("shell.dat",im,jm,x,y,z, u,v,w, p,q);

   return(0);
}
#endif

