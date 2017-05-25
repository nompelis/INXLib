/***************************************************************************
 Sample code to read JPEG images using "libjpeg" (the open library)
 Ioannis Nompelis <nompelis@nobelware.com>      Created:  20130415
 Ioannis Nompelis <nompelis@nobelware.com>      Modified: 20170525
 ***************************************************************************/

/******************************************************************************
 Copyright (c) 2013-2015, Ioannis Nompelis
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
#include <bits/types.h>


#include <jpeglib.h>


int injpg_ReadImage(const char *filename, unsigned char **img_data,
                    unsigned int *iwidth, unsigned int *iheight, int *irgb)
#define FUNC  "injpg_ReadImage"
{

   FILE *fp;
   struct jpeg_decompress_struct cinfo;
   struct jpeg_error_mgr jerr;
   size_t isize;
   JSAMPARRAY buffer;
   int istride,n,m;


   if(filename == NULL) {
      fprintf(stderr," e [%s]  Filename is null\n",FUNC);
      return(1);
   }

   if(*img_data != NULL) {
      fprintf(stderr," e [%s]  Return pointer is not null\n",FUNC);
      return(1);
   }

   fp = fopen(filename,"rb");
   if(fp == NULL) {
      fprintf(stderr," e [%s]  Could not open file %s\n",FUNC,filename);
      return(1);
   }

   /* assign the error output stream for this handle */
   cinfo.err = jpeg_std_error(&jerr);
   /* associate this handle with a decompression object */
   jpeg_create_decompress(&cinfo);
   /* this makes the library read directly from a file */
   jpeg_stdio_src(&cinfo, fp);
   /* reading the image header */
   jpeg_read_header(&cinfo, TRUE);

   /* printed nominal sizes */
   fprintf(stdout," i [%s]  Reading file \"%s\" \n",FUNC,filename);
   fprintf(stdout,"   Width: %d  Height: %d \n",cinfo.image_width,cinfo.image_height);
   fprintf(stdout,"   Components: %d \n",cinfo.num_components);
   fprintf(stdout,"   Colorspace: %d \n",cinfo.jpeg_color_space);

   /* start decompression */
   jpeg_start_decompress(&cinfo);
   fprintf(stdout,"   Output size: %d x %d x %d \n",
           cinfo.output_width, cinfo.output_height ,cinfo.output_components);

   /* create storage for uncompressed image */
   isize = (size_t) (cinfo.output_height * cinfo.output_width
                   * cinfo.output_components);
   *img_data = (unsigned char *) malloc(isize);
   if(*img_data == NULL) {
      fprintf(stderr," e [%s}  Could not allocate space for image data\n",FUNC);
      return(1);
   }


   /* create image via scanlines; use a particular buffer for this */
   istride = cinfo.output_width * cinfo.output_components;
   buffer = (*cinfo.mem->alloc_sarray)
		((j_common_ptr) &cinfo, JPOOL_IMAGE, istride, 1);

   m = 0;
   while(cinfo.output_scanline < cinfo.output_height) {
     /* jpeg_read_scanlines expects an array of pointers to scanlines.
      * Here the array is only one element long, but you could ask for
      * more than one scanline at a time if that's more convenient.
      */
      (void) jpeg_read_scanlines(&cinfo, buffer, 1);
//    printf("Read line %d \n",cinfo.output_scanline);
      for(n=0; n<istride; ++n) {
        (*img_data)[m+n] = (unsigned char) (buffer[0][n]);
      }
      m = m + istride;
   }

   /* assign returned variables */
   *iwidth = cinfo.output_width;
   *iheight = cinfo.output_height;
   *irgb = cinfo.output_components;

   /* terminate decompression */
   (void) jpeg_finish_decompress(&cinfo);
   (void) jpeg_destroy_decompress(&cinfo);

   fclose(fp);

   fprintf(stdout," i [%s]  Done. Returning data.\n",FUNC);

   return(0);
}
#undef FUNC


/* this function is meant to write the result in a tecplot-viewable file */
int test_output(const unsigned char *img_data, unsigned int iwidth, unsigned int iheight, int irgb)
{
   int i,j;
   long n;
   FILE *fp;
   float x,y,rr,gg,bb,ss;


   fp = fopen("output.dat","w");

   fprintf(fp,"variables = x y RED GREEN BLUE SUM\n");
   fprintf(fp,"zone T=\"image\",i=%d,j=%d,f=point\n",iwidth,iheight);

   for(j=0;j<iheight;++j) {
      for(i=0;i<iwidth;++i) {
         x = (float) (i);
         y = -(float) (j);
         n = j*iwidth*irgb + i*irgb;
         if( irgb == 1 ) {
            rr = (float) img_data[n];
            gg = (float) img_data[n];
            bb = (float) img_data[n];
         } else {
            rr = (float) img_data[n];
            gg = (float) img_data[n+1];
            bb = (float) img_data[n+2];
         }
         ss = rr + gg + bb;
         fprintf(fp," %f %f %f %f %f %f \n",x,y,rr,gg,bb,ss);
      }
   }

   fclose(fp);

   return(0);
}


int injpg_PeekImage(const char *filename,
                    unsigned int *iwidth, unsigned int *iheight, int *irgb)
#define FUNC  "injpg_PeekImage"
{

   FILE *fp;
   struct jpeg_decompress_struct cinfo;
   struct jpeg_error_mgr jerr;


   if(filename == NULL) {
      fprintf(stderr," e [%s]  Filename is null\n",FUNC);
      return(1);
   }

   fp = fopen(filename,"rb");
   if(fp == NULL) {
      fprintf(stderr," e [%s]  Could not open file %s\n",FUNC,filename);
      return(1);
   }

   /* assign the error output stream for this handle */
   cinfo.err = jpeg_std_error(&jerr);
   /* associate this handle with a decompression object */
   jpeg_create_decompress(&cinfo);
   /* this makes the library read directly from a file */
   jpeg_stdio_src(&cinfo, fp);
   /* reading the image header */
   jpeg_read_header(&cinfo, TRUE);

   /* printed nominal sizes */
   fprintf(stdout," i [%s]  Reading file \"%s\" \n",FUNC,filename);
   fprintf(stdout,"   Width: %d  Height: %d \n",cinfo.image_width,cinfo.image_height);
   fprintf(stdout,"   Components: %d \n",cinfo.num_components);
   fprintf(stdout,"   Colorspace: %d \n",cinfo.jpeg_color_space);


   /* assign returned variables */
   *iwidth = cinfo.image_width;
   *iheight = cinfo.image_height;
   *irgb = cinfo.num_components;

   (void) jpeg_destroy_decompress(&cinfo);

   fclose(fp);

   fprintf(stdout," i [%s]  Done.\n",FUNC);

   return(0);
}
#undef FUNC



/*
 * Fucntion to copy a buffer that was read from a JPEG image to a buffer that
 * is stored ina TIFF image layer. (JPEG stores 3x unsigned char per RGB sample
 * while TIFF stores an unsigned int (4-byte) RGBA sample.)
 * NOTE: Currently assumes that JPG is RGB or RGBA based on "irgb" but TIFF is
 * always RGBA because the TIFF buffer is unsigned int.
 */
int injpg_CopyJPEGLayerToTIFFLayer(
                const unsigned int width, const unsigned int height,
                const int irgb,
                const unsigned char *cdata, unsigned int **udata)
{
#define FUNC  "injpg_CopyJPEGLayerToTIFFLayer"
   int i,j,n;
   unsigned int *uid;


   if(irgb < 3) {
      fprintf(stdout," e [%s]  This is not a true-colour image: irgb=%d\n",FUNC,irgb);
      return(1);
   }

   if(*udata == NULL) {
      size_t isize;

      isize = (size_t) (width*height);
      *udata = (unsigned int *) malloc(isize*sizeof(unsigned int));
      if(*udata == NULL) {
         fprintf(stdout," e [%s]  Could not allocate RGBA buffer for TIFF layer\n",FUNC);
         return(-1);
      } else {
         fprintf(stdout," i [%s]  Allocated RGBA buffer TIFF layer (%ld) \n",FUNC,isize);
      }
   } else {
         fprintf(stdout," i [%s]  Using pre-allocated TIFF layer buffer\n",FUNC);
   }
   uid = *udata;

   for(j=0;j<height;++j) {
      for(i=0;i<width;++i) {
         unsigned int ui,di;

         n = j*width*irgb + i*irgb;

         ui = (unsigned int) cdata[n];
         ui = (ui << 24);
         di = ui;

         ui = (unsigned int) cdata[n+1];
         ui = (ui << 16);
         di = di + ui;

         ui = (unsigned int) cdata[n+2];
         ui = (ui << 8);
         di = di + ui;

         if(irgb == 4) {   // alpha channel may not be in the JPEG spec.
            ui = (unsigned int) cdata[n+3];
            di = di + ui;
         }

         uid[j*width + i] = di;
/*
printf("%d %d %d %d %d \n",i+1,j+1,  
 (di >> 24) ,
 (di <<  8) >> 24,
 (di << 16) >> 24);   // this seems to work
*/
      }
   }

   return(0);
}
#undef FUNC


/*
int main() {

   char *filename = "house.jpg";
   unsigned char *img_data;
   unsigned int iwidth,iheight;
   int irgb;

   img_data = (unsigned char *) NULL;
   (void) injpg_PeekImage(filename, &iwidth,&iheight,&irgb);
   (void) injpg_ReadImage(filename, &img_data,&iwidth,&iheight,&irgb);
   printf("Returned: width %d height %d rgb %d \n",iwidth,iheight,irgb);
   printf("Writing\n");
(void) test_output(img_data,iwidth,iheight,irgb);
   free(img_data);
   printf("Done\n");

   return(0);
}
*/
