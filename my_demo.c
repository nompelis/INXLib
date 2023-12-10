#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <GL/gl.h>
#include <GL/glu.h>

//
// hard-including some functions to setup the programmable pipeline
//
#include "inogl.h"

//
// hard-including some convenience functions that create a sphere
//
#include "axissphere.c"

//
// a struct to allow us to access everything we need to draw
//
struct my_payload {
   struct inogl_s ogl;
   struct inogl_grp_s *groups;
   struct inogl_shader_s prg;

   void* obj;
   int num_groups;

   // for a special object
   GLuint VAO, VBO;
   GLfloat* vdata;
   int vertex_count;
};
struct my_payload payload;   // global in scope

//
// a function to create a "VAO" of some triangles
// 1. local storage of the data is copied to the "GL's memory" (graphics card)
// 2. the programmable pipeline attribute locations are from v130 (v3.0)
//
void createTriangleVAO( GLuint *vao, GLuint *vbo, int* vertex_count )
{
   GLfloat vertices[] = {
       -0.5f, -0.5f, +0.0f, 0.0,0.0,1.0, 0.0,0.0, 1.0,0.0,0.0,1.0,
        0.5f, -0.5f, +0.0f, 0.0,0.0,1.0, 0.0,0.0, 0.0,1.0,0.0,1.0,
        0.0f,  0.5f, +0.0f, 0.0,0.0,1.0, 0.0,0.0, 0.0,0.0,1.0,1.0,
       -0.5f, -0.5f, +0.1f, 0.0,0.0,1.0, 0.0,0.0, 1.0,0.0,0.0,1.0,
        0.5f, -0.5f, +0.1f, 0.0,0.0,1.0, 0.0,0.0, 0.0,1.0,0.0,1.0,
        0.0f,  0.5f, +0.1f, 0.0,0.0,1.0, 0.0,0.0, 0.0,0.0,1.0,1.0,
       -0.5f, -0.5f, +0.4f, 0.0,0.0,1.0, 0.0,0.0, 1.0,0.0,0.0,1.0,
        0.5f, -0.5f, +0.4f, 0.0,0.0,1.0, 0.0,0.0, 0.0,1.0,0.0,1.0,
        0.0f,  0.5f, +0.4f, 0.0,0.0,1.0, 0.0,0.0, 0.0,0.0,1.0,1.0
   };
   *vertex_count = 3*3;
   printf("SIZEOF(VERTICES) in bytes: %ld \n",sizeof(vertices));//HACK
    
   glGenVertexArrays(1, vao);   // later delete with glDelete...()
   glBindVertexArray(*vao);
    
   glGenBuffers(1, vbo);   // later delete with glDelete...()
   glBindBuffer(GL_ARRAY_BUFFER, *vbo);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
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
    
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindVertexArray(0);
}


//-------------- Vertex/Fragment Shader source codes -------------------
// There is a "vertex shader" and a "fragment shader" to be compiled in the
// (single) shader program to be used in the programmable pipeline. Both are
// of older versions of the OpenGL Shader Language (GLSL) such that they are
// guaranteed to work on old software and a number of X11 DRI drivers. The
// codes should be studied in the order they appear here, as this is also the
// order in which they process vertex data and fragments. A clever way of
// setting up the projetion for the vertex shader is to establish the matrices
// needed using the fixed pipeline, then extract the matrices, and then push
// those matrices to the programmable pipeline as "uniform" variables. The
// table given in the "Redbook" (found below) explains the use of variables in
// the shader code(s). A number of uniform variables are passed to the vertex
// shader to handle the projection but also positioning and orientation of the
// objects in the scene. The fragment position is written directly to the
// "gl_Position" variable -- which appears to be standard. The fragment shader
// renders directly to the "gl_FragColor" variable; this program shows how to
// manipulate color, and re-using the prior value of gl_FragColor" allows for
// blending (translucency). (I have left some commented out code to try a few
// things on how this works. Both codes are given as strings, attached to the
// corresponding global variables in this file; the comments are cleaned-up at
// the C compiler level, not the graphics driver's compiler!

// Table 2.5 GLSL Type Modifiers
// const - Labels a variable as a read-only. It will also be a compile-time
//         constant if its initializer is a compile-time constant.
// in - Specifies that the variable is an input to the shader stage.
// out - Specifies that the variable is an output from a shader stage.
// uniform - Specifies that the value is passed to the shader from the
//           application and is constant across a given primitive.
// buffer - Specifies read-write memory shared with the application. This
//          memory is also referred to as a shader storage buffer.
// shared - Specifies that the variables are shared within a local work group.
//          This is only used in compute shaders


// Uniforms's names in the program need to be consistent with what is used in
// the C program for querying their location (an integer), such that uniform
// variables can be set (and queried) during drawing operations.

const GLchar* vertexShaderSource130 = R"glsl(
#version 130
#pragma optimize(off)
uniform mat4 model; // Modelview matrix
uniform mat4 view; // View matrix
uniform mat4 projection; // Projection matrix
uniform mat3 vtxRot; // a matrix used to rotate wrt the viewer
uniform vec3 vtxTrans; // a matrix used to translate wrt the viewer
uniform vec3 lightPos; // the 3D position of the single light in the scene
in vec3 inPosition;
in vec4 inColor;
in vec3 inNormal;
in vec2 inTexel;
out vec4 vertexColor;
void main() {
   vec3 tPos = vtxTrans + vtxRot * inPosition;
   vec4 viewPos = view * model * vec4(tPos, 1.0);
   gl_Position = projection * viewPos;
   vec3 lightDirection = normalize( lightPos - tPos );
   float d = max( dot( inNormal, lightDirection ), 0.0 );
   vertexColor = d * inColor;
}
)glsl";

// Uniform variable names need to be consistent here as well

const GLchar* fragmentShaderSource130 = R"glsl(
#version 130
#pragma optimize(off)
//uniform vec2  u_resolution;
//uniform vec2  u_mouse;
//uniform float u_time;
uniform vec4 uniColor;
uniform vec4 ambColor;
in vec4 vertexColor;
void main() {
 // gl_FragColor = vec4( 0.1, 0.1, 0.2, 0.0 );   // fixed color (this works)
 // gl_FragColor = vertexColor;   // pass-through (this works)
    gl_FragColor = (0.05 * ambColor)
                 + (0.05 * (ambColor * vertexColor))
                 + (0.90 * (uniColor * vertexColor));
//--- the following is just an illustration of what is possible
//if( false ) {
//  float t = fract( sin( gl_FragCoord.x ) * gl_FragCoord.x );
//  float s = fract( sin( gl_FragCoord.y ) * gl_FragCoord.y );
//  gl_FragColor = vec4(
//                       abs( sin( t ) ) * 1.0,
//                       abs( cos( s ) ) * 1.0,
//                       abs( sin( t ) ) * 1.0,
//                       1.0 );
//} else {
//}
}
)glsl";

//-------------- Endof Vertex/Fragment Shader source codes -------------------

//
// an initialization function that establishes all that is needed for the
// programmable pipeline (including creating some object to render)
//
void init()
{
   printf("================== INITIALIZING ======================= \n");

   // some structures to create a vertex data array
   int im=40,jm=20;
   float* tmp;
   int ierr=inMakeAxisSphereshell3( im, jm, &tmp );
   payload.num_groups = 6;  // increase this to repeat the object in the scene
   payload.groups = (struct inogl_grp_s*)
               malloc( ((size_t) payload.num_groups) *
                                               sizeof( struct inogl_grp_s ) );
   for(short n=0;n<payload.num_groups;++n) {
      payload.groups[n].VAO = 0; // NO NEED
      payload.groups[n].VBO = 0; // NO NEED
      // to understand the following, you need to look into the code...
      int tri_count = 2*(im-1)*(jm-3) + (im-1) + (im-1);
          tri_count -= 0;  // use this to subtract triangles from the sphere
      payload.groups[n].vertex_count = 3 * tri_count;
      payload.groups[n].vdata = (GLfloat*) tmp;
   }

   //---- setting up the OpenGL rendering "programmable pipeline"
   inoglCapabilities( &( payload.ogl ) );

   inoglMakeProgram1( &( payload.prg ),
                      vertexShaderSource130, fragmentShaderSource130 );


   // construction of objects to draw with the programmable pipeline
   for(short n=0;n<payload.num_groups;++n) {
      struct inogl_grp_s* gp = &( payload.groups[n] );

      // create vertex buffer object...
      glGenVertexArrays( 1, &( gp->VAO ) );   // later delete with glDelete...()
      glBindVertexArray( gp->VAO );

      glGenBuffers( 1, &( gp->VBO ) );   // later delete with glDelete...()
      glBindBuffer( GL_ARRAY_BUFFER, gp->VBO );

      // The order here is how they appear in my OBJ reader's returned data.
      // The actual position index is dictated by the first argument.
      GLsizei nglb = 3 + 3 + 2 + 4;
      glBufferData( GL_ARRAY_BUFFER,
                    ((size_t) gp->vertex_count) * nglb * sizeof(float),
                    gp->vdata, GL_STATIC_DRAW);

      glVertexAttribPointer( aPosition, 3, GL_FLOAT, GL_FALSE,
                             nglb * sizeof(float),
                             (void*)0);
      glEnableVertexAttribArray( aPosition );

      glVertexAttribPointer( aNormal, 3, GL_FLOAT, GL_FALSE,
                             nglb * sizeof(float),
                             (void*)(3 * sizeof(float)));
      glEnableVertexAttribArray( aNormal );

      glVertexAttribPointer( aTexel, 2, GL_FLOAT, GL_FALSE,
                             nglb * sizeof(float),
                             (void*)(6 * sizeof(float)));
      glEnableVertexAttribArray( aTexel );

      glVertexAttribPointer( aColor, 4, GL_FLOAT, GL_FALSE,
                             nglb * sizeof(float),
                             (void*)(8 * sizeof(float)));
      glEnableVertexAttribArray( aColor );

      // unbind VBO and VAO
      glBindBuffer( GL_ARRAY_BUFFER, 0 );
      glBindVertexArray(0);
   }

   // make an intuitive object (3 triangles)
   createTriangleVAO( &( payload.VAO ), &(payload.VBO),
                      &( payload.vertex_count ) );

   // trap left-over issues with the GL context
{char ic=0; GLenum glerr;
   while(( glerr = glGetError() ) != GL_NO_ERROR ) {
      const char* errorString = inGetGLErrorString(glerr);
      printf("End of init() errors: %d \"%s\"\n", glerr, errorString );
      ++ic;
   }
if(ic)exit(1);}
   printf("================== INIT ENDED ========================= \n");
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


// ----- the programmable pipeline rasterization function -----
void program_display()
{
   printf("====== DISPLAY =====\n");
   GLfloat matrix[16] = { 1.0, 0.0, 0.0, 0.0,
                          0.0, 1.0, 0.0, 0.0,
                          0.0, 0.0, 1.0, 0.0,
                          0.0, 0.0, 0.0, 1.0 };
   GLfloat Pmatrix[16], Vmatrix[16], Mmatrix[16];

   // Get the Projection Matrix from the direct pipeline
   glGetFloatv( GL_PROJECTION_MATRIX, Pmatrix );
   // Form the View Matrix (presently just a naive copy of the identity matr.)
   memcpy( Vmatrix, matrix, 16*sizeof(GLfloat) );
   // Get the ModelView Matrix from the direct pipeline
   glGetFloatv( GL_MODELVIEW_MATRIX, Mmatrix );

   struct inogl_shader_s* prg = &( payload.prg );

   GLenum glerr;
   printf("Using program: %d \n", prg->shaderProgram );
   glUseProgram( prg->shaderProgram );
   while(( glerr = glGetError() ) != GL_NO_ERROR ) {
      const char* errorString = inGetGLErrorString(glerr);
      printf("Use-program error: %d \"%s\"\n", glerr, errorString );
   }

// UNIFORMS
   // Get the location of the 'model' uniform variable
   GLint modelLoc = glGetUniformLocation( prg->shaderProgram, "model" );
   if( modelLoc == -1 ) {
      printf("FAILED to get modelview matrix location!\n");
   } //else printf("SUCCESS! modelview matrix location: %d \n", modelLoc );
   printf("Get Uniform (modelLoc): %d \n", modelLoc );
   // Set the value of the 'model' uniform variable
   glUniformMatrix4fv( modelLoc, 1, GL_FALSE, Mmatrix );
   while(( glerr = glGetError() ) != GL_NO_ERROR ) {
      const char* errorString = inGetGLErrorString(glerr);
      printf("Set Uniform (model) error: %d \"%s\"\n", glerr, errorString );
   }

   GLint viewLoc = glGetUniformLocation( prg->shaderProgram, "view" );
   if( viewLoc == -1 ) {
      printf("FAILED to get view matrix location!\n");
   } //else printf("SUCCESS! view matrix location: %d \n", viewLoc );
   printf("Get Uniform (viewLoc): %d \n", viewLoc );
   glUniformMatrix4fv( viewLoc, 1, GL_FALSE, Vmatrix );
   while(( glerr = glGetError() ) != GL_NO_ERROR ) {
      const char* errorString = inGetGLErrorString(glerr);
      printf("Set Uniform (view) error: %d \"%s\"\n", glerr, errorString );
   }

   GLint projLoc = glGetUniformLocation( prg->shaderProgram, "projection" );
   if( projLoc == -1 ) {
      printf("FAILED to get projection matrix location!\n");
   } //else printf("SUCCESS! proj matrix location: %d \n", projLoc );
   printf("Get Uniform (projLoc): %d \n", projLoc );
   glUniformMatrix4fv( projLoc, 1, GL_FALSE, Pmatrix );
   while(( glerr = glGetError() ) != GL_NO_ERROR ) {
      const char* errorString = inGetGLErrorString(glerr);
      printf("Set Uniform (proj) error: %d \"%s\"\n", glerr, errorString );
   }

// UNIFORMS
   GLfloat vals[4] = {+1.0f, +1.0f, +1.0f, +1.0f};
   GLint colorLoc = glGetUniformLocation( prg->shaderProgram, "uniColor" );
   if( colorLoc == -1 ) {
      printf("FAILED to get uniColor location!\n");
   } else printf("SUCCESS! uniColor location: %d \n", colorLoc );
   glUniform4f( colorLoc, vals[0], vals[1], vals[2], vals[3] );
   while(( glerr = glGetError() ) != GL_NO_ERROR ) {
      const char* errorString = inGetGLErrorString(glerr);
      printf("Set Uniform (color) error: %d \"%s\"\n", glerr, errorString );
   }
   glGetUniformfv( prg->shaderProgram, colorLoc, vals );
   while(( glerr = glGetError() ) != GL_NO_ERROR ) {
      const char* errorString = inGetGLErrorString(glerr);
      printf("Get Uniform (colorloc) error: %d \"%s\"\n", glerr, errorString );
   }
   printf("Queried uniColor: %f %f %f %f \n",vals[0],vals[1],vals[2],vals[3] );

   GLint ambLoc = glGetUniformLocation( prg->shaderProgram, "ambColor" );
   if( ambLoc == -1 ) {
      printf("FAILED to get ambColor location!\n");
   } else printf("SUCCESS! ambColor location: %d \n", ambLoc );
   glUniform4f( ambLoc, 0.91f, 0.91f, 0.91f, 1.0f );
   while(( glerr = glGetError() ) != GL_NO_ERROR ) {
      const char* errorString = inGetGLErrorString(glerr);
      printf("Set Uniform (color2) error: %d \"%s\"\n", glerr, errorString );
   }
   vals[0] = vals[1] = vals[2] = 3.0f;//HACK (make then something to overwrite)
   glGetUniformfv( prg->shaderProgram, ambLoc, vals );
   while(( glerr = glGetError() ) != GL_NO_ERROR ) {
      const char* errorString = inGetGLErrorString(glerr);
      printf("Get Uniform (colorloc2) error: %d \"%s\"\n", glerr, errorString );
   }
   printf("Queried ambColor: %f %f %f %f \n",vals[0],vals[1],vals[2],vals[3] );

   GLint lposLoc = glGetUniformLocation( prg->shaderProgram, "lightPos" );
   if( lposLoc == -1 ) {
      printf("FAILED to get lightPos location!\n");
   } else printf("SUCCESS! lightPos location: %d \n", lposLoc );
static GLfloat t = 0.0; t += 0.05*0.0;  // can move the light around
   GLfloat move[3] = { 0.0, 4.0, 4.0 };
   move[1] += 0.4*cos(t);
   glUniform3f( lposLoc, move[0], move[1], move[2] );
   while(( glerr = glGetError() ) != GL_NO_ERROR ) {
      const char* errorString = inGetGLErrorString(glerr);
      printf("Set Uniform (lposLoc) error: %d \"%s\"\n", glerr, errorString );
   }
   glGetUniformfv( prg->shaderProgram, lposLoc, vals );
   while(( glerr = glGetError() ) != GL_NO_ERROR ) {
      const char* errorString = inGetGLErrorString(glerr);
      printf("Get Uniform (lposLoc) error: %d \"%s\"\n", glerr, errorString );
   }
   printf("Queried lightPos: %f %f %f \n",vals[0],vals[1],vals[2] );

// UNIFORMS (for object drawing)
   GLint transLoc = glGetUniformLocation( prg->shaderProgram, "vtxTrans" );
   glUniform3f( transLoc, 1.5f, 0.0f, 1.5f );  // for no reason
   GLint rotLoc = glGetUniformLocation( prg->shaderProgram, "vtxRot" );
   GLfloat Rmatrix[9] = { 1.0, 0.0, 0.0,  0.0, 1.0, 0.0, -0.0, 0.0, 1.0 };
   glUniformMatrix3fv( rotLoc, 1, GL_FALSE, Rmatrix );

   inoglDisplayUniforms( prg->shaderProgram );

   vals[0] = 0.0f; vals[1] = 0.0f; vals[2] = 0.0f;
   for(int n=0;n<payload.num_groups;++n) {
      struct inogl_grp_s* gp = &( payload.groups[n] );
      glBindVertexArray( gp->VAO );
      // move the object around
      vals[0] += 1.5f; vals[1] += 0.5f; vals[2] += 1.5f;
      glUniform3f( transLoc, vals[0], vals[1], vals[2] );
      // this call takes vertices but drawstriangles in groups of 3
      glDrawArrays( GL_TRIANGLES, 0, gp->vertex_count );
      glBindVertexArray(0);
   }

   glUniform3f( transLoc, 0.0f, 0.0f, 0.0f );   // return to datum
   glBindVertexArray( payload.VAO );
   glDrawArrays( GL_TRIANGLES, 0, payload.vertex_count );
   glBindVertexArray(0);

   glUseProgram(0);  // unbind shader program
}

