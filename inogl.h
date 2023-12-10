#ifndef _INOGL_H_
#define _INOGL_H_

//
// Absolutely set this when compiling!
//
// COPTS += -D GL_GLEXT_PROTOTYPES

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <GL/gl.h>
#include <GL/glu.h>

//
// vertex attribute designations
// (These are GLSL 3.0 "v130" fixed attribute indixes; adjustable in v330.)
//
enum Attrib_IDs {
   aPosition = 0,
   aColor = 1,
   aNormal = 2,
   aTexel = 3,
};


struct inogl_s {
   const GLubyte *oglRenderer, *oglVersion, *oglVendor, *oglExtensions;
   GLint maxVertexAttrib;
   GLint maxVertexUniform;
   GLint maxFragmentUniform;
   GLint maxUniformBlockSize;
};

struct inogl_shader_s {
   GLuint shaderProgram;
   GLuint vertexShader, fragmentShader;
   GLint positionAttr, colorAttr, normalAttr, texelAttr;
   GLint modelLoc, viewLoc, projLoc;
   GLint lposLoc, transLoc, rotLoc;
   GLfloat lpos[3], tvec[3], rmat[9];
};

struct inogl_grp_s {
   GLuint VAO, VBO;
   GLsizei nglm;         // number of float members per vertex
   GLsizei moff[4];      // offset of float members per vertex
                         // The order of the components is not as above; the
                         // order is: position, normal, texel, color. If they
                         // do not appear in this order in your array, offsets
                         // allow for accommodating an adjustment.
   unsigned char exist;  // 4-bit switches for pos,normal,texel,color components
                         // Switches allow for uniform size arrays, with some
                         // functionality skipped (e.g. texels but no normals).
   int vertex_count;     // assumes "num_tri * 3" for drawing purposes
   GLfloat* vdata;
};

struct inogl_obj_s {
};



//-------------- Vertex Shader source codes -------------------
// The following vertex and fragment shader samples help understand the
// uniform variables assocaited with the shader program structure...

/****************************************************************************
const GLchar* vertexShaderSource130 = R"glsl(
#version 130
#pragma optimize(off)
uniform mat4 model; // Model matrix, includes the rotation
uniform mat4 view; // View matrix
uniform mat4 projection; // Projection matrix
uniform mat3 vertRot;
uniform vec3 vertTrans;
uniform vec3 lightPos;
in vec3 inPosition;
in vec4 inColor;
in vec3 inNormal;
in vec2 inTexel;
out vec4 vertexColor;
void main() {
   vec3 tPos = vertTrans + vertRot * inPosition;
   vec4 viewPos = view * model * vec4(tPos, 1.0);
   gl_Position = projection * viewPos;
   vec3 lightDirection = normalize( lightPos - tPos );
   float d = max( dot( inNormal, lightDirection ), 0.0 );
   vertexColor = d * inColor;
}
)glsl";

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
                 + (0.90 * vertexColor);
 // gl_FragColor = (0.07 * ambColor) + (0.93 * uniColor * vertexColor);
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
 ****************************************************************************/

//-------------- Endof Vertex Shader source codes -------------------

const GLchar* inGetGLErrorString( GLenum err );

void inoglDisplayUniforms( GLuint shaderProgram );

void inoglCapabilities( struct inogl_s* p );

int inoglMakeProgram1( struct inogl_shader_s *p,
                       const GLchar* vertexShaderSource,
                       const GLchar* fragmentShaderSource );

int inoglMakeGroupVAOVBO( struct inogl_grp_s *gp );


#endif
