
//
// function to absorb all details of creating vertex data
// (Takes sizes and an index of a tile in order to create the data for the
// tile that corresponds to this index; this abstracts all implied logic on
// which tile it is, etc, but keeps the sizing of the object general.)
//

void makeTileData( GLfloat *vdata, int im, int jm, int idx )
{
   const float pi = (float) acos(-1.0);
   float xr,yr;
   switch( idx ) {
    case 0: xr =-1.0;    yr =-1.0; break;
    case 1: xr = 0.0;    yr =-1.0; break;
    case 2: xr = 1.0;    yr =-1.0; break;
    case 3: xr =-1.0;    yr = 0.0; break;
    case 4: xr = 0.0;    yr = 0.0; break;
    case 5: xr = 1.0;    yr = 0.0; break;
    case 6: xr =-1.0;    yr = 1.0; break;
    case 7: xr = 0.0;    yr = 1.0; break;
    case 8: xr = 1.0;    yr = 1.0; break;
   }

   if(1) {
      float dx = 1.0/((float) (im-1));
      float dy = 1.0/((float) (jm-1));
      int nn = 0;
      for(int j=0;j<jm-1;++j) {
      for(int i=0;i<im-1;++i) {
         float x = ((float) i) * dx + xr;
         float y = ((float) j) * dy + yr;

         vdata[nn+0] = x;
         vdata[nn+1] = y;
         vdata[nn+2] = -0.1 * (float) (sin(5.0*x)*sin(5.0*y));
         vdata[nn+3] = 0.0;
         vdata[nn+4] = 0.0;
         vdata[nn+5] = 1.0;
         vdata[nn+6] = +0.0;
         vdata[nn+7] = +0.0;
         vdata[nn+8]  = 0.5;
         vdata[nn+9]  = 0.5;
         vdata[nn+10] = 0.8;
         vdata[nn+11] = 1.0;
         nn += 12;

         vdata[nn+0] = x + dx;
         vdata[nn+1] = y;
         vdata[nn+2] = -0.1 * (float) (sin(5.0*(x+dx))*sin(5.0*y));
         vdata[nn+3] = 0.0;
         vdata[nn+4] = 0.0;
         vdata[nn+5] = 1.0;
         vdata[nn+6] = +0.0;
         vdata[nn+7] = +0.0;
         vdata[nn+8]  = 0.5;
         vdata[nn+9]  = 0.5;
         vdata[nn+10] = 0.8;
         vdata[nn+11] = 1.0;
         nn += 12;

         vdata[nn+0] = x;
         vdata[nn+1] = y + dy;
         vdata[nn+2] = -0.1 * (float) (sin(5.0*x)*sin(5.0*(y+dy)));
         vdata[nn+3] = 0.0;
         vdata[nn+4] = 0.0;
         vdata[nn+5] = 1.0;
         vdata[nn+6] = +0.0;
         vdata[nn+7] = +0.0;
         vdata[nn+8]  = 0.5;
         vdata[nn+9]  = 0.5;
         vdata[nn+10] = 0.8;
         vdata[nn+11] = 1.0;
         nn += 12;

         vdata[nn+0] = x;
         vdata[nn+1] = y + dy;
         vdata[nn+2] = -0.1 * (float) (sin(5.0*x)*sin(5.0*(y+dy)));
         vdata[nn+3] = 0.0;
         vdata[nn+4] = 0.0;
         vdata[nn+5] = 1.0;
         vdata[nn+6] = +0.0;
         vdata[nn+7] = +0.0;
         vdata[nn+8]  = 0.5;
         vdata[nn+9]  = 0.5;
         vdata[nn+10] = 0.8;
         vdata[nn+11] = 1.0;
         nn += 12;

         vdata[nn+0] = x + dx;
         vdata[nn+1] = y;
         vdata[nn+2] = -0.1 * (float) (sin(5.0*(x+dx))*sin(5.0*y));
         vdata[nn+3] = 0.0;
         vdata[nn+4] = 0.0;
         vdata[nn+5] = 1.0;
         vdata[nn+6] = +0.0;
         vdata[nn+7] = +0.0;
         vdata[nn+8]  = 0.5;
         vdata[nn+9]  = 0.5;
         vdata[nn+10] = 0.8;
         vdata[nn+11] = 1.0;
         nn += 12;

         vdata[nn+0] = x + dx;
         vdata[nn+1] = y + dy;
         vdata[nn+2] = -0.1 * (float) (sin(5.0*(x+dx))*sin(5.0*(y+dy)));
         vdata[nn+3] = 0.0;
         vdata[nn+4] = 0.0;
         vdata[nn+5] = 1.0;
         vdata[nn+6] = +0.0;
         vdata[nn+7] = +0.0;
         vdata[nn+8]  = 0.5;
         vdata[nn+9]  = 0.5;
         vdata[nn+10] = 0.8;
         vdata[nn+11] = 1.0;
         nn += 12;

      }}
   }
}


//
// function to run through OpenGL operations for building a new VBO
// (This function should simply push data to the graphics context --and to
// the underlying rendering hardware-- and return a handle for the VBO.)
//

void makeVBO( GLuint *vbo, int vertex_count, float* data )
{
   size_t isize = 12*((size_t) vertex_count)*sizeof(GLfloat);

   glGenBuffers( 1, vbo );
   glBindBuffer( GL_ARRAY_BUFFER, *vbo );
// glBufferData( GL_ARRAY_BUFFER, isize, data, GL_DYNAMIC_DRAW );
   glBufferData( GL_ARRAY_BUFFER, isize, data, GL_STATIC_DRAW );
   /// My attempt to completely flush data movement. Make this thread sit here
   /// until the VBO operations in the GPU have completed. If I were to send
   /// this to the rendering thread to properly fence, it would block, and thus
   /// I am accepting that it can glitch. Waiting here
   /// Rendering thread still glitches...
   GLsync syncObj = glFenceSync( GL_SYNC_GPU_COMMANDS_COMPLETE, 0 );
   GLenum unum = glClientWaitSync( syncObj,
                      GL_SYNC_FLUSH_COMMANDS_BIT, (GLuint64) 1000000000 );
   switch( unum ) {
    case GL_ALREADY_SIGNALED:
      fprintf( stdout, " [Thread]  Fence: \"Already signaled\"\n" );
    break;
    case GL_TIMEOUT_EXPIRED:
      fprintf( stdout, " [Thread]  Fence: \"Timeout expired\"\n" );
    break;
    case GL_CONDITION_SATISFIED:
      fprintf( stdout, " [Thread]  Fence: \"Condition satisfied\"\n" );
    break;
    case GL_WAIT_FAILED:
      fprintf( stdout, " [Thread]  Fence: \"Wait failed\"\n" );
    break;
   };

   glBindBuffer( GL_ARRAY_BUFFER, 0 );
  printf("Made (new) VBO handle: %d \n", *vbo );//HACK
}


//
// function to run through OpenGL operations for building a new VAO that
// wraps an existing VBO
// (This function needs a VBO handle. It is mean to be executed by the thread
// that renders.)
//

void makeVAO( GLuint *vao, GLuint vbo )
{
   glGenVertexArrays( 1, vao );
   glBindVertexArray( *vao );

   glBindBuffer( GL_ARRAY_BUFFER, vbo );
  printf("Binding VBO handle: %d \n", vbo );//HACK
    
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,   // position (1st attr)
                         12 * sizeof(float), (void*)0);
   glEnableVertexAttribArray(0);
    
   glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE,   // normal (3rd attr)
                         12 * sizeof(float), (void*)(3 * sizeof(float)));
   glEnableVertexAttribArray(2);
    
   glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE,   // texel (not sure)
                         12 * sizeof(float), (void*)(6 * sizeof(float)));
   glEnableVertexAttribArray(3);
    
   glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE,   // color (2nd attr)
                         12 * sizeof(float), (void*)(8 * sizeof(float)));
   glEnableVertexAttribArray(1);
    
   glBindVertexArray(0);
}


//
// function to absorb all details of the dynamic scene generation
// (if this function is called, a swap will happen for the rendering thread)
//

void updateScene( struct my_payload* p )
{
   // select book-keeping VAO/VBO "name" set to modify
   // (there is set "0" and set "1", addressed by index "i" on the tiled grid)
   int ics=0,icd=1;
   if( p->iuse == 1 ) { ics=1; icd=0; }

   // copy buffer "names" from the set in use to the set that will be used
   for(int i=0;i<9;++i) {
      p->grid_VAO[icd][i] = p->grid_VAO[ics][i];
      p->grid_VBO[icd][i] = p->grid_VBO[ics][i];
      p->bstat[icd][i]    = p->bstat[ics][i];
   }

   // this variable keeps track of which tile on the 3x3 grid to generate
   // (this construct makes this function act as an independent "producer"
   // in a producer/consumer pattern)
   static int ik=-1;
   if( ++ik == 9 ) ik=0;
   // generates data for the appropriate tile (loads the single array)
   makeTileData( p->grid_vdata, p->im, p->jm, ik );

  printf("RENDERING: iuse %d  UPDATING: icd %d \n", p->iuse, icd );//HACK
   // flag the buffer "names" that will be used or deleted
   for(int k=0;k<9;++k) {
      if( k == ik ) {               // the chosen tile
         if( p->bstat[icd][k] == 0 ) {
  printf(" ========== BUILDING Vertex Buffer Obj %d ======== \n",k);//HACK
            makeVBO( &( p->grid_VBO[icd][k] ),
                        p->grid_vertex_count, p->grid_vdata );
            p->bstat[icd][k] = 1;        // flag for "has new VBO, needs VAO"
         }
      }

      // all tiles
      if( p->bstat[icd][k] == 2 ) {      // check if it is being rendered
  printf(" ========== FLAG TO-DELETE Vertex Buffer Obj %d ======== \n",k);//HACK
         p->bstat[icd][k] = 4;           // flagged for delection
      }
   }
}


//
// This function is called by the rendering thread when it needs to clean-up
// the VBOs that or not being rendered; it is supposed to be called right after
// a scene swap and it is not expected to be expennsive.)
//

void cleanScene( struct my_payload* p )
{
   // select arrays to delete; get the blcok of data that was just rendered
   int iuse = payload.iuse;

   for(int i=0;i<9;++i) {
      if( payload.bstat[ iuse ][i] == 8 ) {   // flagged for deletion
  printf(" ========== DELETING Vertex Buffer Obj %d -> %d ======== \n",i,payload.grid_VBO[ iuse ][i]);//HACK
         glDeleteBuffers( 1, &(payload.grid_VBO[ iuse ][i]) );
         payload.bstat[ iuse ][i] = 0;   // flag as "not being rendered"
      }
   }
}


//
// function to become the thread that keeps generating the scene/graphics
// (This thread only generates and deletes VBOs, but also sets the flags that
// the rendering thread uses to generate and delete VAOs.)
//

void* sceneMakerThread( void* arg )
{
   struct my_payload* p = (struct my_payload*) arg;
// struct inogl_s* ogl = (struct inogl_s*) &(p->ogl);
   fd_set rfds;
   struct timeval tv;
   int nfds;
   int istate=1;

   nfds = p->pipefd[0] + 1;     // count of file descriptors to monitor is fixed

   // a long break is needed to initially synchronize (insteading of using a
   // mutex and confusing the kids)
   sleep(1);

   // make the scene creation OpenGL context current for this thread
// glXMakeCurrent( p->xdisplay, p->xwindow, p->glxc );
   glXMakeContextCurrent( p->xdisplay, p->glxwin, p->glxwin, p->glxc );

   // perpetual loop that is the thread's heartbeat
   while( istate ) {   // allow for exit from loop

      // state of the scene maker
      if( istate == 1 ) { // maker is doing things it needs to be doing...
//printf("Scene maker sleeping \n");
         usleep(700000);
         updateScene( p );
         istate = 2;

      } else if( istate == 2 ) { // maker must synchronize with renderer
         // set the common variable to the new rendering set
         if( payload.iuse == 0 ) { payload.ichg = 1; } 
                            else { payload.ichg = 0; } 
         // enter a perpetual sweep to monitor for ack from renderer
         while( istate == 2 ) {   // allow for exit from loop
            // adding socket descriptor to the set to monitor with select
            FD_ZERO( &rfds );
            FD_SET( p->pipefd[0], &rfds );

            // setting timeout values for select
            tv.tv_sec = 1;
            tv.tv_usec = 0;

            int isel = select( nfds, &rfds, NULL, NULL, &tv );
            if( isel > 0 ) {
               char buffer[4];
               isel = (int) read( payload.pipefd[0], buffer, 4 );
               // ignore outcome; assume what is expected

               // clean buffers not being rendered
               cleanScene( p );

               // return to the state of generating graphics
               istate = 1;
            } else {
               printf("Miss. (Misses should be rare!) \n");
            }
         }
      } else {
         //. potential to do something before exiting
      }

   }

   // thread must exit; no need to return anything meanigful
   return NULL;
}


//
// function to initialize all threading constructs and spawn a thread
//

void init_threads( void* arg )
{
   //
   // POSIX threads related data, communication constructs, etc
   //
   pthread_attr_init( &(payload.tattr) );
// pthread_mutex_init( &(payload.mtx), NULL );

   // open a (communication) pipe
// pipe2( payload.pipefd, O_DIRECT | O_NONBLOCK );
   pipe( payload.pipefd );
   // state that rendering thread uses
   payload.iuse = 0;    // initial set to use is "0" (the first one)
   // state the rendering thread must switch to as dictated by the scene maker
   payload.ichg = 0;    // make it the same; no change initially

   //
   // data for what is to be rendered
   //
   size_t isize = 3*3;    // grid of tiles
   // first state arrays
   payload.grid_VAO[0] = (GLuint*) malloc( isize*sizeof(GLuint) );
   payload.grid_VAO[1] = (GLuint*) malloc( isize*sizeof(GLuint) );
   payload.grid_VBO[0] = (GLuint*) malloc( isize*sizeof(GLuint) );
   payload.grid_VBO[1] = (GLuint*) malloc( isize*sizeof(GLuint) );
   payload.bstat[0] = (unsigned char*) malloc( isize*sizeof(unsigned char) );
   payload.bstat[1] = (unsigned char*) malloc( isize*sizeof(unsigned char) );
   for(size_t n=0;n<isize;++n) {
      payload.grid_VAO[0][n] = 0;
      payload.grid_VAO[1][n] = 0;
      payload.grid_VBO[0][n] = 0;
      payload.grid_VBO[1][n] = 0;
      payload.bstat[0][n] = 0;
      payload.bstat[1][n] = 0;
   }
   payload.im = 110 *  1;
   payload.jm = 110 *  1;
   payload.grid_vertex_count = 3 * 2 * (payload.im-1) * (payload.jm-1);
   // array of data for setting up each VBO dynamically
   isize = (size_t) payload.grid_vertex_count;
   payload.grid_vdata = (GLfloat*) malloc( 12*isize*sizeof(GLfloat) );

   // assign the 2nd GLX context access variable(s)
   struct my_xwin_vars* xvars = (struct my_xwin_vars*) arg;
   payload.xdisplay = xvars->xdisplay;
   payload.xwindow = xvars->xwindow;
   payload.glxwin = xvars->glxwin;
   payload.glxc = xvars->glxc2;

/********** build all VBOs on the rendering thread and keep them *******
for(int i=0;i<9;++i) {
   makeTileData( payload.grid_vdata, payload.im, payload.jm, i );
            makeVBO( &( payload.grid_VBO[0][i] ),
                        payload.grid_vertex_count, payload.grid_vdata );
            payload.grid_VBO[1][i] = payload.grid_VBO[0][i];
} **/

   // spawn the scene maker thread
   pthread_create( &(payload.tid), &(payload.tattr),
                   &sceneMakerThread, (void*) &payload );
}

