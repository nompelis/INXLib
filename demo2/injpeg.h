/***************************************************************************
 Sample code to read JPEG images using "libjpeg" (the open library)
 Ioannis Nompelis <nompelis@nobelware.com>      Created:  20130415
 Ioannis Nompelis <nompelis@nobelware.com>      Modified: 20150406
 ***************************************************************************/


int injpg_ReadImage(const char *filename, unsigned char **img_data,
                    unsigned int *iwidth, unsigned int *iheight, int *irgb);

int injpg_PeekImage(const char *filename,
                    unsigned int *iwidth, unsigned int *iheight, int *irgb);

int injpg_CopyJPEGLayerToTIFFLayer(
                const unsigned int width, const unsigned int height,
                const int irgb,
                const unsigned char *cdata, unsigned int **udata);

