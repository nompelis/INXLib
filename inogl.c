#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

//
// Absolutely set this when compiling!
//
// COPTS += -D GL_GLEXT_PROTOTYPES

#include <GL/gl.h>
#include <GL/glu.h>

#include "inogl.h"


//
// an injected code macro to retrieve OpenGL errors
//

#define INDEBUG_BLOCK( STRING ) {\
   char ic=0; GLenum glerr1; \
   while(( glerr1 = glGetError() ) != GL_NO_ERROR ) {\
      const char* errorString = inGetGLErrorString(glerr1);\
      printf("INdebug block errors: %d \"%s\"\n", glerr1, errorString );\
      ++ic;\
   }\
  if(ic) exit(1); else printf("INdebug cleared: \"%s\"\n", STRING );\
}

//
// a function that ChatGPT wrote to get errors
//

const GLchar* inGetGLErrorString( GLenum err ) {
   switch( err ) {
      case GL_NO_ERROR:
         return "[OpenGL] No error";
      case GL_INVALID_ENUM:
         return "[OpenGL] Invalid enum";
      case GL_INVALID_VALUE:
         return "[OpenGL] Invalid value";
      case GL_INVALID_OPERATION:
         return "[OpenGL] Invalid operation";
      case GL_INVALID_FRAMEBUFFER_OPERATION:
         return "[OpenGL] Invalid framebuffer operation";
      case GL_OUT_OF_MEMORY:
         return "[OpenGL] Out of memory";
      case GL_STACK_UNDERFLOW:
         return "[OpenGL] Stack underflow";
      case GL_STACK_OVERFLOW:
         return "[OpenGL] Stack overflow";
      default:
         return "[OpenGL] Error is non-trapped!";
   }
}

//
// Function to return a pointer to a printable string that indicates what
// type of "GLenum" a particular "Uniform variable" of a GLSL shader is.
// The list of shader atomic variable types was built from here:
// https://registry.khronos.org/OpenGL-Refpages/gl4/html/glGetActiveUniform.xhtml
//

const char* inGetStringOfUniformType( GLenum type )
{
   const char* s = NULL;

if( type == GL_FLOAT ) s = "float";
if( type == GL_FLOAT_VEC2 ) s = "vec2";
if( type == GL_FLOAT_VEC3 ) s = "vec3";
if( type == GL_FLOAT_VEC4 ) s = "vec4";
if( type == GL_DOUBLE ) s = "double";
if( type == GL_DOUBLE_VEC2 ) s = "dvec2";
if( type == GL_DOUBLE_VEC3 ) s = "dvec3";
if( type == GL_DOUBLE_VEC4 ) s = "dvec4";
if( type == GL_INT ) s = "int";
if( type == GL_INT_VEC2 ) s = "ivec2";
if( type == GL_INT_VEC3 ) s = "ivec3";
if( type == GL_INT_VEC4 ) s = "ivec4";
if( type == GL_UNSIGNED_INT ) s = "unsigned int";
if( type == GL_UNSIGNED_INT_VEC2 ) s = "uvec2";
if( type == GL_UNSIGNED_INT_VEC3 ) s = "uvec3";
if( type == GL_UNSIGNED_INT_VEC4 ) s = "uvec4";
if( type == GL_BOOL ) s = "bool";
if( type == GL_BOOL_VEC2 ) s = "bvec2";
if( type == GL_BOOL_VEC3 ) s = "bvec3";
if( type == GL_BOOL_VEC4 ) s = "bvec4";
if( type == GL_FLOAT_MAT2 ) s = "mat2";
if( type == GL_FLOAT_MAT3 ) s = "mat3";
if( type == GL_FLOAT_MAT4 ) s = "mat4";
if( type == GL_FLOAT_MAT2x3 ) s = "mat2x3";
if( type == GL_FLOAT_MAT2x4 ) s = "mat2x4";
if( type == GL_FLOAT_MAT3x2 ) s = "mat3x2";
if( type == GL_FLOAT_MAT3x4 ) s = "mat3x4";
if( type == GL_FLOAT_MAT4x2 ) s = "mat4x2";
if( type == GL_FLOAT_MAT4x3 ) s = "mat4x3";
if( type == GL_DOUBLE_MAT2 ) s = "dmat2";
if( type == GL_DOUBLE_MAT3 ) s = "dmat3";
if( type == GL_DOUBLE_MAT4 ) s = "dmat4";
if( type == GL_DOUBLE_MAT2x3 ) s = "dmat2x3";
if( type == GL_DOUBLE_MAT2x4 ) s = "dmat2x4";
if( type == GL_DOUBLE_MAT3x2 ) s = "dmat3x2";
if( type == GL_DOUBLE_MAT3x4 ) s = "dmat3x4";
if( type == GL_DOUBLE_MAT4x2 ) s = "dmat4x2";
if( type == GL_DOUBLE_MAT4x3 ) s = "dmat4x3";
if( type == GL_SAMPLER_1D ) s = "sampler1D";
if( type == GL_SAMPLER_2D ) s = "sampler2D";
if( type == GL_SAMPLER_3D ) s = "sampler3D";
if( type == GL_SAMPLER_CUBE ) s = "samplerCube";
if( type == GL_SAMPLER_1D_SHADOW ) s = "sampler1DShadow";
if( type == GL_SAMPLER_2D_SHADOW ) s = "sampler2DShadow";
if( type == GL_SAMPLER_1D_ARRAY ) s = "sampler1DArray";
if( type == GL_SAMPLER_2D_ARRAY ) s = "sampler2DArray";
if( type == GL_SAMPLER_1D_ARRAY_SHADOW ) s = "sampler1DArrayShadow";
if( type == GL_SAMPLER_2D_ARRAY_SHADOW ) s = "sampler2DArrayShadow";
if( type == GL_SAMPLER_2D_MULTISAMPLE ) s = "sampler2DMS";
if( type == GL_SAMPLER_2D_MULTISAMPLE_ARRAY ) s = "sampler2DMSArray";
if( type == GL_SAMPLER_CUBE_SHADOW ) s = "samplerCubeShadow";
if( type == GL_SAMPLER_BUFFER ) s = "samplerBuffer";
if( type == GL_SAMPLER_2D_RECT ) s = "sampler2DRect";
if( type == GL_SAMPLER_2D_RECT_SHADOW ) s = "sampler2DRectShadow";
if( type == GL_INT_SAMPLER_1D ) s = "isampler1D";
if( type == GL_INT_SAMPLER_2D ) s = "isampler2D";
if( type == GL_INT_SAMPLER_3D ) s = "isampler3D";
if( type == GL_INT_SAMPLER_CUBE ) s = "isamplerCube";
if( type == GL_INT_SAMPLER_1D_ARRAY ) s = "isampler1DArray";
if( type == GL_INT_SAMPLER_2D_ARRAY ) s = "isampler2DArray";
if( type == GL_INT_SAMPLER_2D_MULTISAMPLE ) s = "isampler2DMS";
if( type == GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY ) s = "isampler2DMSArray";
if( type == GL_INT_SAMPLER_BUFFER ) s = "isamplerBuffer";
if( type == GL_INT_SAMPLER_2D_RECT ) s = "isampler2DRect";
if( type == GL_UNSIGNED_INT_SAMPLER_1D ) s = "usampler1D";
if( type == GL_UNSIGNED_INT_SAMPLER_2D ) s = "usampler2D";
if( type == GL_UNSIGNED_INT_SAMPLER_3D ) s = "usampler3D";
if( type == GL_UNSIGNED_INT_SAMPLER_CUBE ) s = "usamplerCube";
if( type == GL_UNSIGNED_INT_SAMPLER_1D_ARRAY ) s = "usampler2DArray";
if( type == GL_UNSIGNED_INT_SAMPLER_2D_ARRAY ) s = "usampler2DArray";
if( type == GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE ) s = "usampler2DMS";
if( type == GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY ) s = "usampler2DMSArray";
if( type == GL_UNSIGNED_INT_SAMPLER_BUFFER ) s = "usamplerBuffer";
if( type == GL_UNSIGNED_INT_SAMPLER_2D_RECT ) s = "usampler2DRect";
if( type == GL_IMAGE_1D ) s = "image1D";
if( type == GL_IMAGE_2D ) s = "image2D";
if( type == GL_IMAGE_3D ) s = "image3D";
if( type == GL_IMAGE_2D_RECT ) s = "image2DRect";
if( type == GL_IMAGE_CUBE ) s = "imageCube";
if( type == GL_IMAGE_BUFFER ) s = "imageBuffer";
if( type == GL_IMAGE_1D_ARRAY ) s = "image1DArray";
if( type == GL_IMAGE_2D_ARRAY ) s = "image2DArray";
if( type == GL_IMAGE_2D_MULTISAMPLE ) s = "image2DMS";
if( type == GL_IMAGE_2D_MULTISAMPLE_ARRAY ) s = "image2DMSArray";
if( type == GL_INT_IMAGE_1D ) s = "iimage1D";
if( type == GL_INT_IMAGE_2D ) s = "iimage2D";
if( type == GL_INT_IMAGE_3D ) s = "iimage3D";
if( type == GL_INT_IMAGE_2D_RECT ) s = "iimage2DRect";
if( type == GL_INT_IMAGE_CUBE ) s = "iimageCube";
if( type == GL_INT_IMAGE_BUFFER ) s = "iimageBuffer";
if( type == GL_INT_IMAGE_1D_ARRAY ) s = "iimage1DArray";
if( type == GL_INT_IMAGE_2D_ARRAY ) s = "iimage2DArray";
if( type == GL_INT_IMAGE_2D_MULTISAMPLE ) s = "iimage2DMS";
if( type == GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY ) s = "iimage2DMSArray";
if( type == GL_UNSIGNED_INT_IMAGE_1D ) s = "uimage1D";
if( type == GL_UNSIGNED_INT_IMAGE_2D ) s = "uimage2D";
if( type == GL_UNSIGNED_INT_IMAGE_3D ) s = "uimage3D";
if( type == GL_UNSIGNED_INT_IMAGE_2D_RECT ) s = "uimage2DRect";
if( type == GL_UNSIGNED_INT_IMAGE_CUBE ) s = "uimageCube";
if( type == GL_UNSIGNED_INT_IMAGE_BUFFER ) s = "uimageBuffer";
if( type == GL_UNSIGNED_INT_IMAGE_1D_ARRAY ) s = "uimage1DArray";
if( type == GL_UNSIGNED_INT_IMAGE_2D_ARRAY ) s = "uimage2DArray";
if( type == GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE ) s = "uimage2DMS";
if( type == GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY ) s = "uimage2DMSArray";
if( type == GL_FLOAT ) s = "float";
if( type == GL_FLOAT_VEC2 ) s = "vec2";
if( type == GL_FLOAT_VEC3 ) s = "vec3";
if( type == GL_FLOAT_VEC4 ) s = "vec4";
if( type == GL_DOUBLE ) s = "double";
if( type == GL_DOUBLE_VEC2 ) s = "dvec2";
if( type == GL_DOUBLE_VEC3 ) s = "dvec3";
if( type == GL_DOUBLE_VEC4 ) s = "dvec4";
if( type == GL_INT ) s = "int";
if( type == GL_INT_VEC2 ) s = "ivec2";
if( type == GL_INT_VEC3 ) s = "ivec3";
if( type == GL_INT_VEC4 ) s = "ivec4";
if( type == GL_UNSIGNED_INT ) s = "unsigned int";
if( type == GL_UNSIGNED_INT_VEC2 ) s = "uvec2";
if( type == GL_UNSIGNED_INT_VEC3 ) s = "uvec3";
if( type == GL_UNSIGNED_INT_VEC4 ) s = "uvec4";
if( type == GL_BOOL ) s = "bool";
if( type == GL_BOOL_VEC2 ) s = "bvec2";
if( type == GL_BOOL_VEC3 ) s = "bvec3";
if( type == GL_BOOL_VEC4 ) s = "bvec4";
if( type == GL_FLOAT_MAT2 ) s = "mat2";
if( type == GL_FLOAT_MAT3 ) s = "mat3";
if( type == GL_FLOAT_MAT4 ) s = "mat4";
if( type == GL_FLOAT_MAT2x3 ) s = "mat2x3";
if( type == GL_FLOAT_MAT2x4 ) s = "mat2x4";
if( type == GL_FLOAT_MAT3x2 ) s = "mat3x2";
if( type == GL_FLOAT_MAT3x4 ) s = "mat3x4";
if( type == GL_FLOAT_MAT4x2 ) s = "mat4x2";
if( type == GL_FLOAT_MAT4x3 ) s = "mat4x3";
if( type == GL_DOUBLE_MAT2 ) s = "dmat2";
if( type == GL_DOUBLE_MAT3 ) s = "dmat3";
if( type == GL_DOUBLE_MAT4 ) s = "dmat4";
if( type == GL_DOUBLE_MAT2x3 ) s = "dmat2x3";
if( type == GL_DOUBLE_MAT2x4 ) s = "dmat2x4";
if( type == GL_DOUBLE_MAT3x2 ) s = "dmat3x2";
if( type == GL_DOUBLE_MAT3x4 ) s = "dmat3x4";
if( type == GL_DOUBLE_MAT4x2 ) s = "dmat4x2";
if( type == GL_DOUBLE_MAT4x3 ) s = "dmat4x3";
if( type == GL_SAMPLER_1D ) s = "sampler1D";
if( type == GL_SAMPLER_2D ) s = "sampler2D";
if( type == GL_SAMPLER_3D ) s = "sampler3D";
if( type == GL_SAMPLER_CUBE ) s = "samplerCube";
if( type == GL_SAMPLER_1D_SHADOW ) s = "sampler1DShadow";
if( type == GL_SAMPLER_2D_SHADOW ) s = "sampler2DShadow";
if( type == GL_SAMPLER_1D_ARRAY ) s = "sampler1DArray";
if( type == GL_SAMPLER_2D_ARRAY ) s = "sampler2DArray";
if( type == GL_SAMPLER_1D_ARRAY_SHADOW ) s = "sampler1DArrayShadow";
if( type == GL_SAMPLER_2D_ARRAY_SHADOW ) s = "sampler2DArrayShadow";
if( type == GL_SAMPLER_2D_MULTISAMPLE ) s = "sampler2DMS";
if( type == GL_SAMPLER_2D_MULTISAMPLE_ARRAY ) s = "sampler2DMSArray";
if( type == GL_SAMPLER_CUBE_SHADOW ) s = "samplerCubeShadow";
if( type == GL_SAMPLER_BUFFER ) s = "samplerBuffer";
if( type == GL_SAMPLER_2D_RECT ) s = "sampler2DRect";
if( type == GL_SAMPLER_2D_RECT_SHADOW ) s = "sampler2DRectShadow";
if( type == GL_INT_SAMPLER_1D ) s = "isampler1D";
if( type == GL_INT_SAMPLER_2D ) s = "isampler2D";
if( type == GL_INT_SAMPLER_3D ) s = "isampler3D";
if( type == GL_INT_SAMPLER_CUBE ) s = "isamplerCube";
if( type == GL_INT_SAMPLER_1D_ARRAY ) s = "isampler1DArray";
if( type == GL_INT_SAMPLER_2D_ARRAY ) s = "isampler2DArray";
if( type == GL_INT_SAMPLER_2D_MULTISAMPLE ) s = "isampler2DMS";
if( type == GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY ) s = "isampler2DMSArray";
if( type == GL_INT_SAMPLER_BUFFER ) s = "isamplerBuffer";
if( type == GL_INT_SAMPLER_2D_RECT ) s = "isampler2DRect";
if( type == GL_UNSIGNED_INT_SAMPLER_1D ) s = "usampler1D";
if( type == GL_UNSIGNED_INT_SAMPLER_2D ) s = "usampler2D";
if( type == GL_UNSIGNED_INT_SAMPLER_3D ) s = "usampler3D";
if( type == GL_UNSIGNED_INT_SAMPLER_CUBE ) s = "usamplerCube";
if( type == GL_UNSIGNED_INT_SAMPLER_1D_ARRAY ) s = "usampler2DArray";
if( type == GL_UNSIGNED_INT_SAMPLER_2D_ARRAY ) s = "usampler2DArray";
if( type == GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE ) s = "usampler2DMS";
if( type == GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY ) s = "usampler2DMSArray";
if( type == GL_UNSIGNED_INT_SAMPLER_BUFFER ) s = "usamplerBuffer";
if( type == GL_UNSIGNED_INT_SAMPLER_2D_RECT ) s = "usampler2DRect";
if( type == GL_IMAGE_1D ) s = "image1D";
if( type == GL_IMAGE_2D ) s = "image2D";
if( type == GL_IMAGE_3D ) s = "image3D";
if( type == GL_IMAGE_2D_RECT ) s = "image2DRect";
if( type == GL_IMAGE_CUBE ) s = "imageCube";
if( type == GL_IMAGE_BUFFER ) s = "imageBuffer";
if( type == GL_IMAGE_1D_ARRAY ) s = "image1DArray";
if( type == GL_IMAGE_2D_ARRAY ) s = "image2DArray";
if( type == GL_IMAGE_2D_MULTISAMPLE ) s = "image2DMS";
if( type == GL_IMAGE_2D_MULTISAMPLE_ARRAY ) s = "image2DMSArray";
if( type == GL_INT_IMAGE_1D ) s = "iimage1D";
if( type == GL_INT_IMAGE_2D ) s = "iimage2D";
if( type == GL_INT_IMAGE_3D ) s = "iimage3D";
if( type == GL_INT_IMAGE_2D_RECT ) s = "iimage2DRect";
if( type == GL_INT_IMAGE_CUBE ) s = "iimageCube";
if( type == GL_INT_IMAGE_BUFFER ) s = "iimageBuffer";
if( type == GL_INT_IMAGE_1D_ARRAY ) s = "iimage1DArray";
if( type == GL_INT_IMAGE_2D_ARRAY ) s = "iimage2DArray";
if( type == GL_INT_IMAGE_2D_MULTISAMPLE ) s = "iimage2DMS";
if( type == GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY ) s = "iimage2DMSArray";
if( type == GL_UNSIGNED_INT_IMAGE_1D ) s = "uimage1D";
if( type == GL_UNSIGNED_INT_IMAGE_2D ) s = "uimage2D";
if( type == GL_UNSIGNED_INT_IMAGE_3D ) s = "uimage3D";
if( type == GL_UNSIGNED_INT_IMAGE_2D_RECT ) s = "uimage2DRect";
if( type == GL_UNSIGNED_INT_IMAGE_CUBE ) s = "uimageCube";
if( type == GL_UNSIGNED_INT_IMAGE_BUFFER ) s = "uimageBuffer";
if( type == GL_UNSIGNED_INT_IMAGE_1D_ARRAY ) s = "uimage1DArray";
if( type == GL_UNSIGNED_INT_IMAGE_2D_ARRAY ) s = "uimage2DArray";
if( type == GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE ) s = "uimage2DMS";
if( type == GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY ) s = "uimage2DMSArray";
if( type == GL_UNSIGNED_INT_ATOMIC_COUNTER ) s = "atomic_uint";
if( type == GL_FLOAT ) s = "float";
if( type == GL_FLOAT_VEC2 ) s = "vec2";
if( type == GL_FLOAT_VEC3 ) s = "vec3";
if( type == GL_FLOAT_VEC4 ) s = "vec4";
if( type == GL_DOUBLE ) s = "double";
if( type == GL_DOUBLE_VEC2 ) s = "dvec2";
if( type == GL_DOUBLE_VEC3 ) s = "dvec3";
if( type == GL_DOUBLE_VEC4 ) s = "dvec4";
if( type == GL_INT ) s = "int";
if( type == GL_INT_VEC2 ) s = "ivec2";
if( type == GL_INT_VEC3 ) s = "ivec3";
if( type == GL_INT_VEC4 ) s = "ivec4";
if( type == GL_UNSIGNED_INT ) s = "unsigned int";
if( type == GL_UNSIGNED_INT_VEC2 ) s = "uvec2";
if( type == GL_UNSIGNED_INT_VEC3 ) s = "uvec3";
if( type == GL_UNSIGNED_INT_VEC4 ) s = "uvec4";
if( type == GL_BOOL ) s = "bool";
if( type == GL_BOOL_VEC2 ) s = "bvec2";
if( type == GL_BOOL_VEC3 ) s = "bvec3";
if( type == GL_BOOL_VEC4 ) s = "bvec4";
if( type == GL_FLOAT_MAT2 ) s = "mat2";
if( type == GL_FLOAT_MAT3 ) s = "mat3";
if( type == GL_FLOAT_MAT4 ) s = "mat4";
if( type == GL_FLOAT_MAT2x3 ) s = "mat2x3";
if( type == GL_FLOAT_MAT2x4 ) s = "mat2x4";
if( type == GL_FLOAT_MAT3x2 ) s = "mat3x2";
if( type == GL_FLOAT_MAT3x4 ) s = "mat3x4";
if( type == GL_FLOAT_MAT4x2 ) s = "mat4x2";
if( type == GL_FLOAT_MAT4x3 ) s = "mat4x3";
if( type == GL_DOUBLE_MAT2 ) s = "dmat2";
if( type == GL_DOUBLE_MAT3 ) s = "dmat3";
if( type == GL_DOUBLE_MAT4 ) s = "dmat4";
if( type == GL_DOUBLE_MAT2x3 ) s = "dmat2x3";
if( type == GL_DOUBLE_MAT2x4 ) s = "dmat2x4";
if( type == GL_DOUBLE_MAT3x2 ) s = "dmat3x2";
if( type == GL_DOUBLE_MAT3x4 ) s = "dmat3x4";
if( type == GL_DOUBLE_MAT4x2 ) s = "dmat4x2";
if( type == GL_DOUBLE_MAT4x3 ) s = "dmat4x3";
if( type == GL_SAMPLER_1D ) s = "sampler1D";
if( type == GL_SAMPLER_2D ) s = "sampler2D";
if( type == GL_SAMPLER_3D ) s = "sampler3D";
if( type == GL_SAMPLER_CUBE ) s = "samplerCube";
if( type == GL_SAMPLER_1D_SHADOW ) s = "sampler1DShadow";
if( type == GL_SAMPLER_2D_SHADOW ) s = "sampler2DShadow";
if( type == GL_SAMPLER_1D_ARRAY ) s = "sampler1DArray";
if( type == GL_SAMPLER_2D_ARRAY ) s = "sampler2DArray";
if( type == GL_SAMPLER_1D_ARRAY_SHADOW ) s = "sampler1DArrayShadow";
if( type == GL_SAMPLER_2D_ARRAY_SHADOW ) s = "sampler2DArrayShadow";
if( type == GL_SAMPLER_2D_MULTISAMPLE ) s = "sampler2DMS";
if( type == GL_SAMPLER_2D_MULTISAMPLE_ARRAY ) s = "sampler2DMSArray";
if( type == GL_SAMPLER_CUBE_SHADOW ) s = "samplerCubeShadow";
if( type == GL_SAMPLER_BUFFER ) s = "samplerBuffer";
if( type == GL_SAMPLER_2D_RECT ) s = "sampler2DRect";
if( type == GL_SAMPLER_2D_RECT_SHADOW ) s = "sampler2DRectShadow";
if( type == GL_INT_SAMPLER_1D ) s = "isampler1D";
if( type == GL_INT_SAMPLER_2D ) s = "isampler2D";
if( type == GL_INT_SAMPLER_3D ) s = "isampler3D";
if( type == GL_INT_SAMPLER_CUBE ) s = "isamplerCube";
if( type == GL_INT_SAMPLER_1D_ARRAY ) s = "isampler1DArray";
if( type == GL_INT_SAMPLER_2D_ARRAY ) s = "isampler2DArray";
if( type == GL_INT_SAMPLER_2D_MULTISAMPLE ) s = "isampler2DMS";
if( type == GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY ) s = "isampler2DMSArray";
if( type == GL_INT_SAMPLER_BUFFER ) s = "isamplerBuffer";
if( type == GL_INT_SAMPLER_2D_RECT ) s = "isampler2DRect";
if( type == GL_UNSIGNED_INT_SAMPLER_1D ) s = "usampler1D";
if( type == GL_UNSIGNED_INT_SAMPLER_2D ) s = "usampler2D";
if( type == GL_UNSIGNED_INT_SAMPLER_3D ) s = "usampler3D";
if( type == GL_UNSIGNED_INT_SAMPLER_CUBE ) s = "usamplerCube";
if( type == GL_UNSIGNED_INT_SAMPLER_1D_ARRAY ) s = "usampler2DArray";
if( type == GL_UNSIGNED_INT_SAMPLER_2D_ARRAY ) s = "usampler2DArray";
if( type == GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE ) s = "usampler2DMS";
if( type == GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY ) s = "usampler2DMSArray";
if( type == GL_UNSIGNED_INT_SAMPLER_BUFFER ) s = "usamplerBuffer";
if( type == GL_UNSIGNED_INT_SAMPLER_2D_RECT ) s = "usampler2DRect";
if( type == GL_IMAGE_1D ) s = "image1D";
if( type == GL_IMAGE_2D ) s = "image2D";
if( type == GL_IMAGE_3D ) s = "image3D";
if( type == GL_IMAGE_2D_RECT ) s = "image2DRect";
if( type == GL_IMAGE_CUBE ) s = "imageCube";
if( type == GL_IMAGE_BUFFER ) s = "imageBuffer";
if( type == GL_IMAGE_1D_ARRAY ) s = "image1DArray";
if( type == GL_IMAGE_2D_ARRAY ) s = "image2DArray";
if( type == GL_IMAGE_2D_MULTISAMPLE ) s = "image2DMS";
if( type == GL_IMAGE_2D_MULTISAMPLE_ARRAY ) s = "image2DMSArray";
if( type == GL_INT_IMAGE_1D ) s = "iimage1D";
if( type == GL_INT_IMAGE_2D ) s = "iimage2D";
if( type == GL_INT_IMAGE_3D ) s = "iimage3D";
if( type == GL_INT_IMAGE_2D_RECT ) s = "iimage2DRect";
if( type == GL_INT_IMAGE_CUBE ) s = "iimageCube";
if( type == GL_INT_IMAGE_BUFFER ) s = "iimageBuffer";
if( type == GL_INT_IMAGE_1D_ARRAY ) s = "iimage1DArray";
if( type == GL_INT_IMAGE_2D_ARRAY ) s = "iimage2DArray";
if( type == GL_INT_IMAGE_2D_MULTISAMPLE ) s = "iimage2DMS";
if( type == GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY ) s = "iimage2DMSArray";
if( type == GL_UNSIGNED_INT_IMAGE_1D ) s = "uimage1D";
if( type == GL_UNSIGNED_INT_IMAGE_2D ) s = "uimage2D";
if( type == GL_UNSIGNED_INT_IMAGE_3D ) s = "uimage3D";
if( type == GL_UNSIGNED_INT_IMAGE_2D_RECT ) s = "uimage2DRect";
if( type == GL_UNSIGNED_INT_IMAGE_CUBE ) s = "uimageCube";
if( type == GL_UNSIGNED_INT_IMAGE_BUFFER ) s = "uimageBuffer";
if( type == GL_UNSIGNED_INT_IMAGE_1D_ARRAY ) s = "uimage1DArray";
if( type == GL_UNSIGNED_INT_IMAGE_2D_ARRAY ) s = "uimage2DArray";
if( type == GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE ) s = "uimage2DMS";
if( type == GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY ) s = "uimage2DMSArray";
if( type == GL_UNSIGNED_INT_ATOMIC_COUNTER ) s = "atomic_uint";
if( type == GL_UNSIGNED_INT_ATOMIC_COUNTER ) s = "atomic_uint";

   return s;
}


//
// Function to print OpenGL version and capabilities
//

void inoglCapabilities( struct inogl_s* p )
{
   //GLenum glerr;
   p->oglRenderer = glGetString( GL_RENDERER );
   p->oglVersion = glGetString( GL_VERSION );
   p->oglVendor = glGetString( GL_VENDOR );
   p->oglExtensions = glGetString( GL_EXTENSIONS );
   fprintf( stdout, " [OpenGL]  GL_RENDERER   %s\n", (char*) p->oglRenderer);
   fprintf( stdout, " [OpenGL]  GL_VERSION    %s\n", (char*) p->oglVersion );
   fprintf( stdout, " [OpenGL]  GL_VENDOR     %s\n", (char*) p->oglVendor );
// fprintf( stdout, " [OpenGL]  GL_EXTENSIONS %s\n", (char*) p->oglExtensions );

   GLint maxVertexAttrib;
   glGetIntegerv( GL_MAX_VERTEX_ATTRIBS, &maxVertexAttrib );
   fprintf( stdout, " [OpenGL]  Maximum attributes: %d \n", maxVertexAttrib );
   p->maxVertexAttrib  = maxVertexAttrib;

   GLint maxVertexUniform;
   glGetIntegerv( GL_MAX_VERTEX_UNIFORM_COMPONENTS, &maxVertexUniform );
   fprintf( stdout, " [OpenGL]  Max Vertex Uniforms: %d\n", maxVertexUniform );
   p->maxVertexUniform = maxVertexUniform;

   GLint maxFragmentUniform;
   glGetIntegerv( GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, &maxFragmentUniform );
   fprintf( stdout, " [OpenGL]  Max Fragment Uniforms: %d\n",
            maxFragmentUniform );
   p->maxFragmentUniform = maxFragmentUniform;

   GLint maxUniformBlockSize;
   glGetIntegerv( GL_MAX_UNIFORM_BLOCK_SIZE, &maxUniformBlockSize );
   fprintf( stdout, " [OpenGL]  Max Uniform Block Size: %d\n",
            maxUniformBlockSize );
   p->maxUniformBlockSize = maxUniformBlockSize;
}


//
// a function to tabulate all uniforms exposed in a shader program
// (I build this from what I read in a Stack Exchange post.)
//

void inoglDisplayUniforms( GLuint shaderProgram )
{
   GLint count;
   glGetProgramiv( shaderProgram, GL_ACTIVE_UNIFORMS, &count );
   fprintf( stdout, " [OpenGL]  Active uniforms: %d \n", count );
   for(GLint i=0;i<count;++i) {
      GLsizei bufSize=32, length;   // our buffer size, length of bytes written
      GLchar name[32];
      GLint size;
      GLenum type;
      glGetActiveUniform( shaderProgram, i, 
                          bufSize, &length, &size, &type, name );
       GLint loc = glGetUniformLocation(shaderProgram, name); 
       printf(" - Uniform %d, name: \"%s\" location: %d, type %d (\"%s\"), size: %d \n",
              i ,name ,loc, type, inGetStringOfUniformType( type ), size );
   }
}



//-------------- Vertex/Fragment Shader source codes info -------------------

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


//
// Function to create a shader program
//

int inoglMakeProgram1( struct inogl_shader_s *p,
                      const GLchar* vertexShaderSource,
                      const GLchar* fragmentShaderSource )
{
   //
   // Build and compile the shader program (vertex and fragment shaders)
   //
   GLint prgStatus;
   GLenum glerr;

   // Vertex shader
   GLuint vertexShader = glCreateShader( GL_VERTEX_SHADER );
   glShaderSource( vertexShader, 1, &vertexShaderSource, NULL );
   glCompileShader( vertexShader );
   glGetShaderiv( vertexShader, GL_COMPILE_STATUS, &prgStatus );
   if( prgStatus != GL_TRUE ) {
      char infoLog[512];
      glGetShaderInfoLog( vertexShader, 512, NULL, infoLog );
      fprintf( stdout, " [OpenGL]  Vertex compile error: \"%s\"\n", infoLog );
      return 1;
   }

   // Fragment shader
   GLuint fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );
   glShaderSource( fragmentShader, 1, &fragmentShaderSource, NULL );
   glCompileShader( fragmentShader );
   glGetShaderiv( fragmentShader, GL_COMPILE_STATUS, &prgStatus );
   if( prgStatus != GL_TRUE ) {
      char infoLog[512];
      glGetShaderInfoLog( fragmentShader, 512, NULL, infoLog );
      fprintf( stdout, " [OpenGL]  Fragment compile error: \"%s\"\n", infoLog );
      glDeleteShader( vertexShader );
      return 1;
   }

   GLuint shaderProgram = glCreateProgram();

   // Link shaders
   glAttachShader( shaderProgram, vertexShader );
   while(( glerr = glGetError() ) != GL_NO_ERROR ) {
      const char* errorString = inGetGLErrorString(glerr);
      fprintf( stdout, " [OpenGL]  Attach Vertex Shader errors: %d \"%s\"\n",
               glerr, errorString );
      glDeleteShader( vertexShader );
      glDeleteShader( fragmentShader );
      return 2;
   }
   fprintf( stdout, " [OpenGL]  Attached Vertex Shader\n" );

   glAttachShader( shaderProgram, fragmentShader );
   while(( glerr = glGetError() ) != GL_NO_ERROR ) {
      const char* errorString = inGetGLErrorString(glerr);
      fprintf( stdout, " [OpenGL]  Attach Fragment Shader errors: %d \"%s\"\n",
               glerr, errorString );
      glDeleteShader( vertexShader );
      glDeleteShader( fragmentShader );
      return 2;
   }
   fprintf( stdout, " [OpenGL]  Attached Fragment Shader\n" );

   fprintf( stdout, " [OpenGL]  Linking program: %d \n", shaderProgram );
   glLinkProgram( shaderProgram );
   glGetProgramiv( shaderProgram, GL_LINK_STATUS, &prgStatus );
   if( prgStatus != GL_TRUE ) {
      char infoLog[512];
      glGetShaderInfoLog( shaderProgram, 512, NULL, infoLog );
      fprintf( stdout, " [OpenGL]  Shader linking error: \"%s\" \n", infoLog );
      glDeleteShader( vertexShader );
      glDeleteShader( fragmentShader );
      return 3;
   }

   p->vertexShader = vertexShader;
   p->fragmentShader = fragmentShader;
   p->shaderProgram = shaderProgram;
{ // allow for deleting those compiled object files
  // Delete the shaders as they're linked into our program now
   glDeleteShader( vertexShader );
   glDeleteShader( fragmentShader );
}

   while(( glerr = glGetError() ) != GL_NO_ERROR ) {
      const char* errorString = inGetGLErrorString(glerr);
      printf("Link Shader Prog errors: %d \"%s\"\n", glerr, errorString );
   }

   glGetProgramiv( shaderProgram, GL_VALIDATE_STATUS, &prgStatus );
   if( prgStatus == GL_TRUE ) {
      fprintf( stdout, " [OpenGL]  Program linking successful \n" );
   } else {
      fprintf( stdout, " [OpenGL]  Program linking FAILED! \n" );
INDEBUG_BLOCK( "inoglMakeProgram1()" )
   }

   glGetProgramiv( shaderProgram, GL_ACTIVE_UNIFORM_MAX_LENGTH, &prgStatus );
   fprintf( stdout, " [OpenGL]  Active uniform max. length: %d \n", prgStatus );
   glGetProgramiv( shaderProgram, GL_ATTACHED_SHADERS, &prgStatus );
   fprintf( stdout, " [OpenGL]  Attached shaders: %d \n", prgStatus );

   // get locations of the attribute variables in the shader program...
   // ...and bind them (tell the program to use them)
   p->positionAttr = glGetAttribLocation( shaderProgram, "inPosition" );
   glBindAttribLocation( shaderProgram, p->positionAttr, "inPosition" );

   p->colorAttr = glGetAttribLocation( shaderProgram, "inColor" );
   glBindAttribLocation( shaderProgram, p->colorAttr, "inColor" );

   p->normalAttr = glGetAttribLocation( shaderProgram, "inNormal" );
   glBindAttribLocation( shaderProgram, p->normalAttr, "inNormal" );

// p->texelAttr = glGetAttribLocation( shaderProgram, "inTexel" );
// glBindAttribLocation( shaderProgram, p->texelAttr, "inTexel" );
   p->texelAttr = -999;

   fprintf( stdout, " [OpenGL]  Program %d attrib locations: position %d, color %d, normal %d, texel %d \n",
           p->shaderProgram,
           p->positionAttr,  p->colorAttr,  p->normalAttr, p->texelAttr );

   // get locations of uniform variables
   p->lposLoc = glGetUniformLocation( shaderProgram, "lightPos" );
INDEBUG_BLOCK( "init() lposLoc" )
   p->transLoc = glGetUniformLocation( shaderProgram, "vertTrans" );
INDEBUG_BLOCK( "init() transLoc" )
   p->rotLoc = glGetUniformLocation( shaderProgram, "vertRot" );
INDEBUG_BLOCK( "init() rotLoc" )

   p->modelLoc = glGetUniformLocation( shaderProgram, "model" );
   if( p->modelLoc == -1 ) {
      printf("FAILED to get modelview matrix location!\n");
   } //else printf("SUCCESS! modelview matrix location: %d \n", p->modelLoc );
   fprintf( stdout, " [OpenGL]  Get Uniform (modelLoc): %d \n", p->modelLoc );

   p->viewLoc = glGetUniformLocation( shaderProgram, "view" );
   if( p->viewLoc == -1 ) {
      printf("FAILED to get view matrix location!\n");
   } //else printf("SUCCESS! view matrix location: %d \n", p->viewLoc );
   fprintf( stdout, " [OpenGL]  Get Uniform viewLoc: %d \n", p->viewLoc );

   p->projLoc = glGetUniformLocation( shaderProgram, "projection" );
   if( p->projLoc == -1 ) {
      printf("FAILED to get projection matrix location!\n");
   } //else printf("SUCCESS! proj matrix location: %d \n", p->projLoc );
   fprintf( stdout, " [OpenGL]  Get Uniform projLoc: %d \n", p->projLoc );

   glUseProgram(0);

   inoglDisplayUniforms( shaderProgram );

   return 0;
}

//
// Function to create the VAO and VBO of a group of triangles (made of vertices)
// This function requires 3 position and 3 normal vector components, 3 texel
// coordinates, and 4 color components (RGBA).
// GLsizei nglm = 3 + 3 + 2 + 4;
// GLsizei moff[4] = {0, 3, 6, 8};
//

int inoglMakeGroupVAOVBO( struct inogl_grp_s *gp )
{
   GLsizei nglm = gp->nglm;
   GLsizei moff[4];
   memcpy( moff, gp->moff, sizeof(moff) );

   // create vertex buffer object...
   glGenVertexArrays( 1, &( gp->VAO ) );   // later delete with glDelete...()
   glBindVertexArray( gp->VAO );

   glGenBuffers( 1, &( gp->VBO ) );   // later delete with glDelete...()
   glBindBuffer( GL_ARRAY_BUFFER, gp->VBO );

   // The order here is how they appear in my OBJ reader's returned data.
   // The actual position index is dictated by the incoming offsets array.
   glBufferData( GL_ARRAY_BUFFER,
                 ((size_t) gp->vertex_count) * nglm * sizeof(float),
                 gp->vdata, GL_STATIC_DRAW );

   if( gp->exist & 0x01 ) {    // lsb [____ 0001]
      glVertexAttribPointer( aPosition, 3, GL_FLOAT, GL_FALSE,
                             nglm * sizeof(float),
                             (void*) (moff[0] * sizeof(float)) );
      glEnableVertexAttribArray( aPosition );
   }

   if( gp->exist & (0x01 << 1) ) {    // [____ 0010]
      glVertexAttribPointer( aNormal, 3, GL_FLOAT, GL_FALSE,
                             nglm * sizeof(float),
                             (void*) (moff[1] * sizeof(float)) );
      glEnableVertexAttribArray( aNormal );
   }

   if( gp->exist & (0x01 << 2) ) {   // [____ 0100]
      glVertexAttribPointer( aTexel, 2, GL_FLOAT, GL_FALSE,
                             nglm * sizeof(float),
                             (void*) (moff[2] * sizeof(float)) );
      glEnableVertexAttribArray( aTexel );
   }

   if( gp->exist & (0x01 << 3) ) {   // [____ 1000]
      glVertexAttribPointer( aColor, 4, GL_FLOAT, GL_FALSE,
                             nglm * sizeof(float),
                             (void*) (moff[3] * sizeof(float)) );
      glEnableVertexAttribArray( aColor );
   }

   // unbind VBO and VAO
   glBindBuffer( GL_ARRAY_BUFFER, 0 );
   glBindVertexArray(0);

   // trap left-over issues with the GL context
// GLenum glerr;
// while(( glerr = glGetError() ) != GL_NO_ERROR ) {
//    const char* errorString = inGetGLErrorString(glerr);
//    printf("End of init() errors: %d \"%s\"\n", glerr, errorString );
// }

   return 0;
}


