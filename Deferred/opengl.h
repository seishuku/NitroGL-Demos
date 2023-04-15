#ifndef __OPENGL_H__
#define __OPENGL_H__

#include <windows.h>
#include <GL/gl.h>

#define GL_MULTISAMPLE_ARB								0x809D
#define GL_SAMPLE_ALPHA_TO_COVERAGE_ARB					0x809E
#define GL_SAMPLE_ALPHA_TO_ONE_ARB						0x809F
#define GL_SAMPLE_COVERAGE_ARB							0x80A0
#define GL_SAMPLE_BUFFERS_ARB							0x80A8
#define GL_SAMPLES_ARB									0x80A9
#define GL_SAMPLE_COVERAGE_VALUE_ARB					0x80AA
#define GL_SAMPLE_COVERAGE_INVERT_ARB					0x80AB
#define GL_MULTISAMPLE_BIT_ARB							0x20000000

extern GLvoid (APIENTRY *glSampleCoverageARB)(GLclampf value, GLboolean invert);
extern int GL_ARB_multisample_Flag;

#define GL_ACTIVE_TEXTURE_ARB							0x84E0
#define GL_CLIENT_ACTIVE_TEXTURE_ARB					0x84E1
#define GL_MAX_TEXTURE_UNITS_ARB						0x84E2
#define GL_TEXTURE0_ARB									0x84C0
#define GL_TEXTURE1_ARB									0x84C1
#define GL_TEXTURE2_ARB									0x84C2
#define GL_TEXTURE3_ARB									0x84C3
#define GL_TEXTURE4_ARB									0x84C4
#define GL_TEXTURE5_ARB									0x84C5
#define GL_TEXTURE6_ARB									0x84C6
#define GL_TEXTURE7_ARB									0x84C7
#define GL_TEXTURE8_ARB									0x84C8
#define GL_TEXTURE9_ARB									0x84C9
#define GL_TEXTURE10_ARB								0x84CA
#define GL_TEXTURE11_ARB								0x84CB
#define GL_TEXTURE12_ARB								0x84CC
#define GL_TEXTURE13_ARB								0x84CD
#define GL_TEXTURE14_ARB								0x84CE
#define GL_TEXTURE15_ARB								0x84CF
#define GL_TEXTURE16_ARB								0x84D0
#define GL_TEXTURE17_ARB								0x84D1
#define GL_TEXTURE18_ARB								0x84D2
#define GL_TEXTURE19_ARB								0x84D3
#define GL_TEXTURE20_ARB								0x84D4
#define GL_TEXTURE21_ARB								0x84D5
#define GL_TEXTURE22_ARB								0x84D6
#define GL_TEXTURE23_ARB								0x84D7
#define GL_TEXTURE24_ARB								0x84D8
#define GL_TEXTURE25_ARB								0x84D9
#define GL_TEXTURE26_ARB								0x84DA
#define GL_TEXTURE27_ARB								0x84DB
#define GL_TEXTURE28_ARB								0x84DC
#define GL_TEXTURE29_ARB								0x84DD
#define GL_TEXTURE30_ARB								0x84DE
#define GL_TEXTURE31_ARB								0x84DF

extern GLvoid (APIENTRY *glActiveTextureARB)(GLenum texture);
extern GLvoid (APIENTRY *glClientActiveTextureARB)(GLenum texture);
extern GLvoid (APIENTRY *glMultiTexCoord1dARB)(GLenum texture, GLdouble s);
extern GLvoid (APIENTRY *glMultiTexCoord1dvARB)(GLenum texture, const GLdouble *v);
extern GLvoid (APIENTRY *glMultiTexCoord1fARB)(GLenum texture, GLfloat s);
extern GLvoid (APIENTRY *glMultiTexCoord1fvARB)(GLenum texture, const GLfloat *v);
extern GLvoid (APIENTRY *glMultiTexCoord1iARB)(GLenum texture, GLint s);
extern GLvoid (APIENTRY *glMultiTexCoord1ivARB)(GLenum texture, const GLint *v);
extern GLvoid (APIENTRY *glMultiTexCoord1sARB)(GLenum texture, GLshort s);
extern GLvoid (APIENTRY *glMultiTexCoord1svARB)(GLenum texture, const GLshort *v);
extern GLvoid (APIENTRY *glMultiTexCoord2dARB)(GLenum texture, GLdouble s, GLdouble t);
extern GLvoid (APIENTRY *glMultiTexCoord2dvARB)(GLenum texture, const GLdouble *v);
extern GLvoid (APIENTRY *glMultiTexCoord2fARB)(GLenum texture, GLfloat s, GLfloat t);
extern GLvoid (APIENTRY *glMultiTexCoord2fvARB)(GLenum texture, const GLfloat *v);
extern GLvoid (APIENTRY *glMultiTexCoord2iARB)(GLenum texture, GLint s, GLint t);
extern GLvoid (APIENTRY *glMultiTexCoord2ivARB)(GLenum texture, const GLint *v);
extern GLvoid (APIENTRY *glMultiTexCoord2sARB)(GLenum texture, GLshort s, GLshort t);
extern GLvoid (APIENTRY *glMultiTexCoord2svARB)(GLenum texture, const GLshort *v);
extern GLvoid (APIENTRY *glMultiTexCoord3dARB)(GLenum texture, GLdouble s, GLdouble t, GLdouble r);
extern GLvoid (APIENTRY *glMultiTexCoord3dvARB)(GLenum texture, const GLdouble *v);
extern GLvoid (APIENTRY *glMultiTexCoord3fARB)(GLenum texture, GLfloat s, GLfloat t, GLfloat r);
extern GLvoid (APIENTRY *glMultiTexCoord3fvARB)(GLenum texture, const GLfloat *v);
extern GLvoid (APIENTRY *glMultiTexCoord3iARB)(GLenum texture, GLint s, GLint t, GLint r);
extern GLvoid (APIENTRY *glMultiTexCoord3ivARB)(GLenum texture, const GLint *v);
extern GLvoid (APIENTRY *glMultiTexCoord3sARB)(GLenum texture, GLshort s, GLshort t, GLshort r);
extern GLvoid (APIENTRY *glMultiTexCoord3svARB)(GLenum texture, const GLshort *v);
extern GLvoid (APIENTRY *glMultiTexCoord4dARB)(GLenum texture, GLdouble s, GLdouble t, GLdouble r, GLdouble q);
extern GLvoid (APIENTRY *glMultiTexCoord4dvARB)(GLenum texture, const GLdouble *v);
extern GLvoid (APIENTRY *glMultiTexCoord4fARB)(GLenum texture, GLfloat s, GLfloat t, GLfloat r, GLfloat q);
extern GLvoid (APIENTRY *glMultiTexCoord4fvARB)(GLenum texture, const GLfloat *v);
extern GLvoid (APIENTRY *glMultiTexCoord4iARB)(GLenum texture, GLint s, GLint t, GLint r, GLint q);
extern GLvoid (APIENTRY *glMultiTexCoord4ivARB)(GLenum texture, const GLint *v);
extern GLvoid (APIENTRY *glMultiTexCoord4sARB)(GLenum texture, GLshort s, GLshort t, GLshort r, GLshort q);
extern GLvoid (APIENTRY *glMultiTexCoord4svARB)(GLenum texture, const GLshort *v);
extern int GL_ARB_multitexture_Flag;

#define GL_COMPRESSED_ALPHA_ARB							0x84E9
#define GL_COMPRESSED_LUMINANCE_ARB						0x84EA
#define GL_COMPRESSED_LUMINANCE_ALPHA_ARB				0x84EB
#define GL_COMPRESSED_INTENSITY_ARB						0x84EC
#define GL_COMPRESSED_RGB_ARB							0x84ED
#define GL_COMPRESSED_RGBA_ARB							0x84EE
#define GL_TEXTURE_COMPRESSION_HINT_ARB					0x84EF
#define GL_TEXTURE_COMPRESSED_IMAGE_SIZE_ARB			0x86A0
#define GL_TEXTURE_COMPRESSED_ARB						0x86A1
#define GL_NUM_COMPRESSED_TEXTURE_FORMATS_ARB			0x86A2
#define GL_COMPRESSED_TEXTURE_FORMATS_ARB				0x86A3

extern GLvoid (APIENTRY *glCompressedTexImage3DARB)(GLenum target, GLint level, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid *data);
extern GLvoid (APIENTRY *glCompressedTexImage2DARB)(GLenum target, GLint level, GLenum internalFormat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data);
extern GLvoid (APIENTRY *glCompressedTexImage1DARB)(GLenum target, GLint level, GLenum internalFormat, GLsizei width, GLint border, GLsizei imageSize, const GLvoid *data);
extern GLvoid (APIENTRY *glCompressedTexSubImage3DARB)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid *data);
extern GLvoid (APIENTRY *glCompressedTexSubImage2DARB)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid *data);
extern GLvoid (APIENTRY *glCompressedTexSubImage1DARB)(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const GLvoid *data);
extern GLvoid (APIENTRY *glGetCompressedTexImageARB)(GLenum target, GLint lod, GLvoid *img);
extern int GL_ARB_texture_compression_Flag;

#define GL_MAX_VERTEX_UNITS_ARB							0x86A4
#define GL_ACTIVE_VERTEX_UNITS_ARB						0x86A5
#define GL_WEIGHT_SUM_UNITY_ARB							0x86A6
#define GL_VERTEX_BLEND_ARB								0x86A7
#define GL_CURRENT_WEIGHT_ARB							0x86A8
#define GL_WEIGHT_ARRAY_TYPE_ARB						0x86A9
#define GL_WEIGHT_ARRAY_STRIDE_ARB						0x86AA
#define GL_WEIGHT_ARRAY_SIZE_ARB						0x86AB
#define GL_WEIGHT_ARRAY_POINTER_ARB						0x86AC
#define GL_WEIGHT_ARRAY_ARB								0x86AD
#define GL_MODELVIEW0_ARB								0x1700
#define GL_MODELVIEW1_ARB								0x850A
#define GL_MODELVIEW2_ARB								0x8722
#define GL_MODELVIEW3_ARB								0x8723
#define GL_MODELVIEW4_ARB								0x8724
#define GL_MODELVIEW5_ARB								0x8725
#define GL_MODELVIEW6_ARB								0x8726
#define GL_MODELVIEW7_ARB								0x8727
#define GL_MODELVIEW8_ARB								0x8728
#define GL_MODELVIEW9_ARB								0x8729
#define GL_MODELVIEW10_ARB								0x872A
#define GL_MODELVIEW11_ARB								0x872B
#define GL_MODELVIEW12_ARB								0x872C
#define GL_MODELVIEW13_ARB								0x872D
#define GL_MODELVIEW14_ARB								0x872E
#define GL_MODELVIEW15_ARB								0x872F
#define GL_MODELVIEW16_ARB								0x8730
#define GL_MODELVIEW17_ARB								0x8731
#define GL_MODELVIEW18_ARB								0x8732
#define GL_MODELVIEW19_ARB								0x8733
#define GL_MODELVIEW20_ARB								0x8734
#define GL_MODELVIEW21_ARB								0x8735
#define GL_MODELVIEW22_ARB								0x8736
#define GL_MODELVIEW23_ARB								0x8737
#define GL_MODELVIEW24_ARB								0x8738
#define GL_MODELVIEW25_ARB								0x8739
#define GL_MODELVIEW26_ARB								0x873A
#define GL_MODELVIEW27_ARB								0x873B
#define GL_MODELVIEW28_ARB								0x873C
#define GL_MODELVIEW29_ARB								0x873D
#define GL_MODELVIEW30_ARB								0x873E
#define GL_MODELVIEW31_ARB								0x873F

extern GLvoid (APIENTRY *glWeightbvARB)(GLint size, GLbyte *weights);
extern GLvoid (APIENTRY *glWeightsvARB)(GLint size, GLshort *weights);
extern GLvoid (APIENTRY *glWeightivARB)(GLint size, GLint *weights);
extern GLvoid (APIENTRY *glWeightfvARB)(GLint size, GLfloat *weights);
extern GLvoid (APIENTRY *glWeightdvARB)(GLint size, GLdouble *weights);
extern GLvoid (APIENTRY *glWeightubvARB)(GLint size, GLubyte *weights);
extern GLvoid (APIENTRY *glWeightusvARB)(GLint size, GLushort *weights);
extern GLvoid (APIENTRY *glWeightuivARB)(GLint size, GLuint *weights);
extern GLvoid (APIENTRY *glWeightPointerARB)(GLint size, GLenum type, GLsizei stride, GLvoid *pointer);
extern GLvoid (APIENTRY *glVertexBlendARB)(GLint count);
extern int GL_ARB_vertex_blend_Flag;

#define GL_VERTEX_PROGRAM_ARB							0x8620
#define GL_VERTEX_PROGRAM_POINT_SIZE_ARB				0x8642
#define GL_VERTEX_PROGRAM_TWO_SIDE_ARB					0x8643
#define GL_COLOR_SUM_ARB								0x8458
#define GL_PROGRAM_FORMAT_ASCII_ARB						0x8875
#define GL_VERTEX_ATTRIB_ARRAY_ENABLED_ARB				0x8622
#define GL_VERTEX_ATTRIB_ARRAY_SIZE_ARB					0x8623
#define GL_VERTEX_ATTRIB_ARRAY_STRIDE_ARB				0x8624
#define GL_VERTEX_ATTRIB_ARRAY_TYPE_ARB					0x8625
#define GL_VERTEX_ATTRIB_ARRAY_NORMALIZED_ARB			0x886A
#define GL_CURRENT_VERTEX_ATTRIB_ARB					0x8626
#define GL_VERTEX_ATTRIB_ARRAY_POINTER_ARB				0x8645
#define GL_PROGRAM_LENGTH_ARB							0x8627
#define GL_PROGRAM_FORMAT_ARB							0x8876
#define GL_PROGRAM_BINDING_ARB							0x8677
#define GL_PROGRAM_INSTRUCTIONS_ARB						0x88A0
#define GL_MAX_PROGRAM_INSTRUCTIONS_ARB					0x88A1
#define GL_PROGRAM_NATIVE_INSTRUCTIONS_ARB				0x88A2
#define GL_MAX_PROGRAM_NATIVE_INSTRUCTIONS_ARB			0x88A3
#define GL_PROGRAM_TEMPORARIES_ARB						0x88A4
#define GL_MAX_PROGRAM_TEMPORARIES_ARB					0x88A5
#define GL_PROGRAM_NATIVE_TEMPORARIES_ARB				0x88A6
#define GL_MAX_PROGRAM_NATIVE_TEMPORARIES_ARB			0x88A7
#define GL_PROGRAM_PARAMETERS_ARB						0x88A8
#define GL_MAX_PROGRAM_PARAMETERS_ARB					0x88A9
#define GL_PROGRAM_NATIVE_PARAMETERS_ARB				0x88AA
#define GL_MAX_PROGRAM_NATIVE_PARAMETERS_ARB			0x88AB
#define GL_PROGRAM_ATTRIBS_ARB							0x88AC
#define GL_MAX_PROGRAM_ATTRIBS_ARB						0x88AD
#define GL_PROGRAM_NATIVE_ATTRIBS_ARB					0x88AE
#define GL_MAX_PROGRAM_NATIVE_ATTRIBS_ARB				0x88AF
#define GL_PROGRAM_ADDRESS_REGISTERS_ARB				0x88B0
#define GL_MAX_PROGRAM_ADDRESS_REGISTERS_ARB			0x88B1
#define GL_PROGRAM_NATIVE_ADDRESS_REGISTERS_ARB			0x88B2
#define GL_MAX_PROGRAM_NATIVE_ADDRESS_REGISTERS_ARB		0x88B3
#define GL_MAX_PROGRAM_LOCAL_PARAMETERS_ARB				0x88B4
#define GL_MAX_PROGRAM_ENV_PARAMETERS_ARB				0x88B5
#define GL_PROGRAM_UNDER_NATIVE_LIMITS_ARB				0x88B6
#define GL_PROGRAM_STRING_ARB							0x8628
#define GL_PROGRAM_ERROR_POSITION_ARB					0x864B
#define GL_CURRENT_MATRIX_ARB							0x8641
#define GL_TRANSPOSE_CURRENT_MATRIX_ARB					0x88B7
#define GL_CURRENT_MATRIX_STACK_DEPTH_ARB				0x8640
#define GL_MAX_VERTEX_ATTRIBS_ARB						0x8869
#define GL_MAX_PROGRAM_MATRICES_ARB						0x862F
#define GL_MAX_PROGRAM_MATRIX_STACK_DEPTH_ARB			0x862E
#define GL_PROGRAM_ERROR_STRING_ARB						0x8874
#define GL_MATRIX0_ARB									0x88C0
#define GL_MATRIX1_ARB									0x88C1
#define GL_MATRIX2_ARB									0x88C2
#define GL_MATRIX3_ARB									0x88C3
#define GL_MATRIX4_ARB									0x88C4
#define GL_MATRIX5_ARB									0x88C5
#define GL_MATRIX6_ARB									0x88C6
#define GL_MATRIX7_ARB									0x88C7
#define GL_MATRIX8_ARB									0x88C8
#define GL_MATRIX9_ARB									0x88C9
#define GL_MATRIX10_ARB									0x88CA
#define GL_MATRIX11_ARB									0x88CB
#define GL_MATRIX12_ARB									0x88CC
#define GL_MATRIX13_ARB									0x88CD
#define GL_MATRIX14_ARB									0x88CE
#define GL_MATRIX15_ARB									0x88CF
#define GL_MATRIX16_ARB									0x88D0
#define GL_MATRIX17_ARB									0x88D1
#define GL_MATRIX18_ARB									0x88D2
#define GL_MATRIX19_ARB									0x88D3
#define GL_MATRIX20_ARB									0x88D4
#define GL_MATRIX21_ARB									0x88D5
#define GL_MATRIX22_ARB									0x88D6
#define GL_MATRIX23_ARB									0x88D7
#define GL_MATRIX24_ARB									0x88D8
#define GL_MATRIX25_ARB									0x88D9
#define GL_MATRIX26_ARB									0x88DA
#define GL_MATRIX27_ARB									0x88DB
#define GL_MATRIX28_ARB									0x88DC
#define GL_MATRIX29_ARB									0x88DD
#define GL_MATRIX30_ARB									0x88DE
#define GL_MATRIX31_ARB									0x88DF

extern GLvoid (APIENTRY *glVertexAttrib1sARB)(GLuint index, GLshort x);
extern GLvoid (APIENTRY *glVertexAttrib1fARB)(GLuint index, GLfloat x);
extern GLvoid (APIENTRY *glVertexAttrib1dARB)(GLuint index, GLdouble x);
extern GLvoid (APIENTRY *glVertexAttrib2sARB)(GLuint index, GLshort x, GLshort y);
extern GLvoid (APIENTRY *glVertexAttrib2fARB)(GLuint index, GLfloat x, GLfloat y);
extern GLvoid (APIENTRY *glVertexAttrib2dARB)(GLuint index, GLdouble x, GLdouble y);
extern GLvoid (APIENTRY *glVertexAttrib3sARB)(GLuint index, GLshort x, GLshort y, GLshort z);
extern GLvoid (APIENTRY *glVertexAttrib3fARB)(GLuint index, GLfloat x, GLfloat y, GLfloat z);
extern GLvoid (APIENTRY *glVertexAttrib3dARB)(GLuint index, GLdouble x, GLdouble y, GLdouble z);
extern GLvoid (APIENTRY *glVertexAttrib4sARB)(GLuint index, GLshort x, GLshort y, GLshort z, GLshort w);
extern GLvoid (APIENTRY *glVertexAttrib4fARB)(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
extern GLvoid (APIENTRY *glVertexAttrib4dARB)(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
extern GLvoid (APIENTRY *glVertexAttrib4NubARB)(GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w);
extern GLvoid (APIENTRY *glVertexAttrib1svARB)(GLuint index, const GLshort *v);
extern GLvoid (APIENTRY *glVertexAttrib1fvARB)(GLuint index, const GLfloat *v);
extern GLvoid (APIENTRY *glVertexAttrib1dvARB)(GLuint index, const GLdouble *v);
extern GLvoid (APIENTRY *glVertexAttrib2svARB)(GLuint index, const GLshort *v);
extern GLvoid (APIENTRY *glVertexAttrib2fvARB)(GLuint index, const GLfloat *v);
extern GLvoid (APIENTRY *glVertexAttrib2dvARB)(GLuint index, const GLdouble *v);
extern GLvoid (APIENTRY *glVertexAttrib3svARB)(GLuint index, const GLshort *v);
extern GLvoid (APIENTRY *glVertexAttrib3fvARB)(GLuint index, const GLfloat *v);
extern GLvoid (APIENTRY *glVertexAttrib3dvARB)(GLuint index, const GLdouble *v);
extern GLvoid (APIENTRY *glVertexAttrib4svARB)(GLuint index, const GLbyte *v);
extern GLvoid (APIENTRY *glVertexAttrib4fvARB)(GLuint index, const GLshort *v);
extern GLvoid (APIENTRY *glVertexAttrib4dvARB)(GLuint index, const GLint *v);
extern GLvoid (APIENTRY *glVertexAttrib4ivARB)(GLuint index, const GLubyte *v);
extern GLvoid (APIENTRY *glVertexAttrib4bvARB)(GLuint index, const GLushort *v);
extern GLvoid (APIENTRY *glVertexAttrib4usvARB)(GLuint index, const GLuint *v);
extern GLvoid (APIENTRY *glVertexAttrib4ubvARB)(GLuint index, const GLfloat *v);
extern GLvoid (APIENTRY *glVertexAttrib4uivARB)(GLuint index, const GLdouble *v);
extern GLvoid (APIENTRY *glVertexAttrib4NbvARB)(GLuint index, const GLbyte *v);
extern GLvoid (APIENTRY *glVertexAttrib4NsvARB)(GLuint index, const GLshort *v);
extern GLvoid (APIENTRY *glVertexAttrib4NivARB)(GLuint index, const GLint *v);
extern GLvoid (APIENTRY *glVertexAttrib4NubvARB)(GLuint index, const GLubyte *v);
extern GLvoid (APIENTRY *glVertexAttrib4NusvARB)(GLuint index, const GLushort *v);
extern GLvoid (APIENTRY *glVertexAttrib4NuivARB)(GLuint index, const GLuint *v);
extern GLvoid (APIENTRY *glVertexAttribPointerARB)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer);
extern GLvoid (APIENTRY *glEnableVertexAttribArrayARB)(GLuint index);
extern GLvoid (APIENTRY *glDisableVertexAttribArrayARB)(GLuint index);
extern GLvoid (APIENTRY *glProgramStringARB)(GLenum target, GLenum format, GLsizei len, const GLvoid *string);
extern GLvoid (APIENTRY *glBindProgramARB)(GLenum target, GLuint program);
extern GLvoid (APIENTRY *glDeleteProgramsARB)(GLsizei n, const GLuint *programs);
extern GLvoid (APIENTRY *glGenProgramsARB)(GLsizei n, GLuint *programs);
extern GLvoid (APIENTRY *glProgramEnvParameter4dARB)(GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
extern GLvoid (APIENTRY *glProgramEnvParameter4dvARB)(GLenum target, GLuint index, const GLdouble *params);
extern GLvoid (APIENTRY *glProgramEnvParameter4fARB)(GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
extern GLvoid (APIENTRY *glProgramEnvParameter4fvARB)(GLenum target, GLuint index, const GLfloat *params);
extern GLvoid (APIENTRY *glProgramLocalParameter4dARB)(GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
extern GLvoid (APIENTRY *glProgramLocalParameter4dvARB)(GLenum target, GLuint index, const GLdouble *params);
extern GLvoid (APIENTRY *glProgramLocalParameter4fARB)(GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
extern GLvoid (APIENTRY *glProgramLocalParameter4fvARB)(GLenum target, GLuint index, const GLfloat *params);
extern GLvoid (APIENTRY *glGetProgramEnvParameterdvARB)(GLenum target, GLuint index, GLdouble *params);
extern GLvoid (APIENTRY *glGetProgramEnvParameterfvARB)(GLenum target, GLuint index, GLfloat *params);
extern GLvoid (APIENTRY *glGetProgramLocalParameterdvARB)(GLenum target, GLuint index, GLdouble *params);
extern GLvoid (APIENTRY *glGetProgramLocalParameterfvARB)(GLenum target, GLuint index, GLfloat *params);
extern GLvoid (APIENTRY *glGetProgramivARB)(GLenum target, GLenum pname, GLint *params);
extern GLvoid (APIENTRY *glGetProgramStringARB)(GLenum target, GLenum pname, GLvoid *string);
extern GLvoid (APIENTRY *glGetVertexAttribdvARB)(GLuint index, GLenum pname, GLdouble *params);
extern GLvoid (APIENTRY *glGetVertexAttribfvARB)(GLuint index, GLenum pname, GLfloat *params);
extern GLvoid (APIENTRY *glGetVertexAttribivARB)(GLuint index, GLenum pname, GLint *params);
extern GLvoid (APIENTRY *glGetVertexAttribPointervARB)(GLuint index, GLenum pname, GLvoid **pointer);
extern GLboolean (APIENTRY *glIsProgramARB)(GLuint program);
extern int GL_ARB_vertex_program_Flag;

#define GL_FRAGMENT_PROGRAM_ARB							0x8804
#define GL_PROGRAM_ALU_INSTRUCTIONS_ARB					0x8805
#define GL_PROGRAM_TEX_INSTRUCTIONS_ARB					0x8806
#define GL_PROGRAM_TEX_INDIRECTIONS_ARB					0x8807
#define GL_PROGRAM_NATIVE_ALU_INSTRUCTIONS_ARB			0x8808
#define GL_PROGRAM_NATIVE_TEX_INSTRUCTIONS_ARB			0x8809
#define GL_PROGRAM_NATIVE_TEX_INDIRECTIONS_ARB			0x880A
#define GL_MAX_PROGRAM_ALU_INSTRUCTIONS_ARB				0x880B
#define GL_MAX_PROGRAM_TEX_INSTRUCTIONS_ARB				0x880C
#define GL_MAX_PROGRAM_TEX_INDIRECTIONS_ARB				0x880D
#define GL_MAX_PROGRAM_NATIVE_ALU_INSTRUCTIONS_ARB		0x880E
#define GL_MAX_PROGRAM_NATIVE_TEX_INSTRUCTIONS_ARB		0x880F
#define GL_MAX_PROGRAM_NATIVE_TEX_INDIRECTIONS_ARB		0x8810
#define GL_MAX_TEXTURE_COORDS_ARB						0x8871
#define GL_MAX_TEXTURE_IMAGE_UNITS_ARB					0x8872
extern int GL_ARB_fragment_program_Flag;

#define GL_DEPTH_COMPONENT16_ARB						0x81A5
#define GL_DEPTH_COMPONENT24_ARB						0x81A6
#define GL_DEPTH_COMPONENT32_ARB						0x81A7
#define GL_TEXTURE_DEPTH_SIZE_ARB						0x884A
#define GL_DEPTH_TEXTURE_MODE_ARB						0x884B
extern int GL_ARB_depth_texture_Flag;

#define GL_TEXTURE_COMPARE_MODE_ARB						0x884C
#define GL_TEXTURE_COMPARE_FUNC_ARB						0x884D
#define GL_COMPARE_R_TO_TEXTURE_ARB						0x884E
extern int GL_ARB_shadow_Flag;

#define GL_TEXTURE_COMPARE_FAIL_VALUE_ARB				0x80BF
extern int GL_ARB_shadow_ambient_Flag;

#define GL_NORMAL_MAP_ARB								0x8511
#define GL_REFLECTION_MAP_ARB							0x8512
#define GL_TEXTURE_CUBE_MAP_ARB							0x8513
#define GL_TEXTURE_BINDING_CUBE_MAP_ARB					0x8514
#define GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB				0x8515
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB				0x8516
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB				0x8517
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB				0x8518
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB				0x8519
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB				0x851A
#define GL_PROXY_TEXTURE_CUBE_MAP_ARB					0x851B
#define GL_MAX_CUBE_MAP_TEXTURE_SIZE_ARB				0x851C
extern int GL_ARB_texture_cube_map_Flag;

extern int GL_ARB_texture_non_power_of_two_Flag;

extern int GL_ARB_texture_env_add_Flag;

#define GL_COMBINE_EXT									0x8570
#define GL_COMBINE_RGB_EXT								0x8571
#define GL_COMBINE_ALPHA_EXT							0x8572
#define GL_SOURCE0_RGB_EXT								0x8580
#define GL_SOURCE1_RGB_EXT								0x8581
#define GL_SOURCE2_RGB_EXT								0x8582
#define GL_SOURCE0_ALPHA_EXT							0x8588
#define GL_SOURCE1_ALPHA_EXT							0x8589
#define GL_SOURCE2_ALPHA_EXT							0x858A
#define GL_OPERAND0_RGB_EXT								0x8590
#define GL_OPERAND1_RGB_EXT								0x8591
#define GL_OPERAND2_RGB_EXT								0x8592
#define GL_OPERAND0_ALPHA_EXT							0x8598
#define GL_OPERAND1_ALPHA_EXT							0x8599
#define GL_OPERAND2_ALPHA_EXT							0x859A
#define GL_RGB_SCALE_EXT								0x8573
#define GL_ADD_SIGNED_EXT								0x8574
#define GL_INTERPOLATE_EXT								0x8575
#define GL_CONSTANT_EXT									0x8576
#define GL_PRIMARY_COLOR_EXT							0x8577
#define GL_PREVIOUS_EXT									0x8578
extern int GL_EXT_texture_env_combine_Flag;

#define GL_COMBINE_ARB									0x8570
#define GL_COMBINE_RGB_ARB								0x8571
#define GL_COMBINE_ALPHA_ARB							0x8572
#define GL_SOURCE0_RGB_ARB								0x8580
#define GL_SOURCE1_RGB_ARB								0x8581
#define GL_SOURCE2_RGB_ARB								0x8582
#define GL_SOURCE0_ALPHA_ARB							0x8588
#define GL_SOURCE1_ALPHA_ARB							0x8589
#define GL_SOURCE2_ALPHA_ARB							0x858A
#define GL_OPERAND0_RGB_ARB								0x8590
#define GL_OPERAND1_RGB_ARB								0x8591
#define GL_OPERAND2_RGB_ARB								0x8592
#define GL_OPERAND0_ALPHA_ARB							0x8598
#define GL_OPERAND1_ALPHA_ARB							0x8599
#define GL_OPERAND2_ALPHA_ARB							0x859A
#define GL_RGB_SCALE_ARB								0x8573
#define GL_ADD_SIGNED_ARB								0x8574
#define GL_INTERPOLATE_ARB								0x8575
#define GL_CONSTANT_ARB									0x8576
#define GL_PRIMARY_COLOR_ARB							0x8577
#define GL_PREVIOUS_ARB									0x8578
#define GL_SUBTRACT_ARB									0x84E7
extern int GL_ARB_texture_env_combine_Flag;

extern int GL_ARB_texture_env_crossbar_Flag;

#define GL_DOT3_RGB_ARB									0x86AE
#define GL_DOT3_RGBA_ARB								0x86AF
extern int GL_ARB_texture_env_dot3_Flag;

#define GL_ARRAY_BUFFER_ARB								0x8892
#define GL_ELEMENT_ARRAY_BUFFER_ARB						0x8893
#define GL_ARRAY_BUFFER_BINDING_ARB						0x8894
#define GL_ELEMENT_ARRAY_BUFFER_BINDING_ARB				0x8895
#define GL_VERTEX_ARRAY_BUFFER_BINDING_ARB				0x8896
#define GL_NORMAL_ARRAY_BUFFER_BINDING_ARB				0x8897
#define GL_COLOR_ARRAY_BUFFER_BINDING_ARB				0x8898
#define GL_INDEX_ARRAY_BUFFER_BINDING_ARB				0x8899
#define GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING_ARB		0x889A
#define GL_EDGE_FLAG_ARRAY_BUFFER_BINDING_ARB			0x889B
#define GL_SECONDARY_COLOR_ARRAY_BUFFER_BINDING_ARB		0x889C
#define GL_FOG_COORDINATE_ARRAY_BUFFER_BINDING_ARB		0x889D
#define GL_WEIGHT_ARRAY_BUFFER_BINDING_ARB				0x889E
#define GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING_ARB		0x889F
#define GL_STREAM_DRAW_ARB								0x88E0
#define GL_STREAM_READ_ARB								0x88E1
#define GL_STREAM_COPY_ARB								0x88E2
#define GL_STATIC_DRAW_ARB								0x88E4
#define GL_STATIC_READ_ARB								0x88E5
#define GL_STATIC_COPY_ARB								0x88E6
#define GL_DYNAMIC_DRAW_ARB								0x88E8
#define GL_DYNAMIC_READ_ARB								0x88E9
#define GL_DYNAMIC_COPY_ARB								0x88EA
#define GL_READ_ONLY_ARB								0x88B8
#define GL_WRITE_ONLY_ARB								0x88B9
#define GL_READ_WRITE_ARB								0x88BA
#define GL_BUFFER_SIZE_ARB								0x8764
#define GL_BUFFER_USAGE_ARB								0x8765
#define GL_BUFFER_ACCESS_ARB							0x88BB
#define GL_BUFFER_MAPPED_ARB							0x88BC
#define GL_BUFFER_MAP_POINTER_ARB						0x88BD

#if defined(_WIN64)
typedef __int64 GLintptrARB;
typedef __int64 GLsizeiptrARB;
#elif defined(__ia64__)||defined(__x86_64__)
typedef long int GLintptrARB;
typedef long int GLsizeiptrARB;
#else
typedef int GLintptrARB;
typedef int GLsizeiptrARB;
#endif

extern GLvoid (APIENTRY *glBindBufferARB)(GLenum target, GLuint buffer);
extern GLvoid (APIENTRY *glDeleteBuffersARB)(GLsizei n, const GLuint *buffers);
extern GLvoid (APIENTRY *glGenBuffersARB)(GLsizei n, GLuint *buffers);
extern GLboolean (APIENTRY *glIsBufferARB)(GLuint buffer);
extern GLvoid (APIENTRY *glBufferDataARB)(GLenum target, GLsizeiptrARB size, const GLvoid *data, GLenum usage);
extern GLvoid (APIENTRY *glBufferSubDataARB)(GLenum target, GLintptrARB offset, GLsizeiptrARB size, const GLvoid *data);
extern GLvoid (APIENTRY *glGetBufferSubDataARB)(GLenum target, GLintptrARB offset, GLsizeiptrARB size, GLvoid *data);
extern GLvoid *(APIENTRY *glMapBufferARB)(GLenum target, GLenum access);
extern GLboolean (APIENTRY *glUnmapBufferARB)(GLenum target);
extern GLvoid (APIENTRY *glGetBufferParameterivARB)(GLenum target, GLenum pname, GLint *params);
extern GLvoid (APIENTRY *glGetBufferPointervARB)(GLenum target, GLenum pname, GLvoid **params);
extern int GL_ARB_vertex_buffer_object_Flag;

#define GL_TEXTURE_BINDING_3D_EXT						0x806A
#define GL_PACK_SKIP_IMAGES_EXT							0x806B
#define GL_PACK_IMAGE_HEIGHT_EXT						0x806C
#define GL_UNPACK_SKIP_IMAGES_EXT						0x806D
#define GL_UNPACK_IMAGE_HEIGHT_EXT						0x806E
#define GL_TEXTURE_3D_EXT								0x806F
#define GL_PROXY_TEXTURE_3D_EXT							0x8070
#define GL_TEXTURE_DEPTH_EXT							0x8071
#define GL_TEXTURE_WRAP_R_EXT							0x8072
#define GL_MAX_3D_TEXTURE_SIZE_EXT						0x8073

extern GLvoid (APIENTRY *glTexImage3DEXT)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
extern GLvoid (APIENTRY *glTexSubImage3DEXT)(GLenum target, GLint lod, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei w, GLsizei h, GLsizei d, GLenum format, GLenum type, const GLvoid *buf);
extern GLvoid (APIENTRY *glCopyTexSubImage3DEXT)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
extern int GL_EXT_texture3D_Flag;

#define GL_POINT_SIZE_MIN_ARB							0x8126
#define GL_POINT_SIZE_MAX_ARB							0x8127
#define GL_POINT_FADE_THRESHOLD_SIZE_ARB				0x8128
#define GL_POINT_DISTANCE_ATTENUATION_ARB				0x8129

extern GLvoid (APIENTRY *glPointParameterfARB)(GLenum pname, GLfloat param);
extern GLvoid (APIENTRY *glPointParameterfvARB)(GLenum pname, GLfloat *params);
extern int GL_ARB_point_parameters_Flag;

#define GL_POINT_SPRITE_ARB								0x8861
#define GL_COORD_REPLACE_ARB							0x8862
extern int GL_ARB_point_sprite_Flag;

#define GL_ABGR_EXT										0x8000
extern int GL_EXT_abgr_Flag;

#define GL_BGR_EXT										0x80E0
#define GL_BGRA_EXT										0x80E1
extern int GL_EXT_bgra_Flag;

#define GL_UNSIGNED_BYTE_3_3_2_EXT						0x8032
#define GL_UNSIGNED_BYTE_2_3_3_REV_EXT					0x8362
#define GL_UNSIGNED_SHORT_4_4_4_4_EXT					0x8033
#define GL_UNSIGNED_SHORT_4_4_4_4_REV_EXT				0x8365
#define GL_UNSIGNED_SHORT_5_6_5_EXT						0x8363
#define GL_UNSIGNED_SHORT_5_6_5_REV_EXT					0x8364
#define GL_UNSIGNED_SHORT_5_5_5_1_EXT					0x8034
#define GL_UNSIGNED_SHORT_1_5_5_5_REV_EXT				0x8366
#define GL_UNSIGNED_INT_8_8_8_8_EXT						0x8035
#define GL_UNSIGNED_INT_8_8_8_8_REV_EXT					0x8367
#define GL_UNSIGNED_INT_10_10_10_2_EXT					0x8036
#define GL_UNSIGNED_INT_2_10_10_10_REV_EXT				0x8368
extern int GL_EXT_packed_pixels_Flag;

#define GL_COMPRESSED_RGB_S3TC_DXT1_EXT					0x83F0
#define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT				0x83F1
#define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT				0x83F2
#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT				0x83F3
extern int GL_EXT_texture_compression_s3tc_Flag;

#define GL_CLAMP_TO_EDGE_EXT							0x812F
extern int GL_EXT_texture_edge_clamp_Flag;

#define GL_TEXTURE_MAX_ANISOTROPY_EXT					0x84FE
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT				0x84FF
extern int GL_EXT_texture_filter_anisotropic_Flag;

#define GL_INCR_WRAP_EXT								0x8507
#define GL_DECR_WRAP_EXT								0x8508
extern int GL_EXT_stencil_wrap_Flag;

#define GL_TEXTURE_RECTANGLE_EXT						0x84F5
#define GL_TEXTURE_BINDING_RECTANGLE_EXT				0x84F6
#define GL_PROXY_TEXTURE_RECTANGLE_EXT					0x84F7
#define GL_MAX_RECTANGLE_TEXTURE_SIZE_EXT				0x84F8
extern int GL_EXT_texture_rectangle_Flag;

#define GL_FRAGMENT_SHADER_ATI							0x8920
#define GL_REG_0_ATI									0x8921
#define GL_REG_1_ATI									0x8922
#define GL_REG_2_ATI									0x8923
#define GL_REG_3_ATI									0x8924
#define GL_REG_4_ATI									0x8925
#define GL_REG_5_ATI									0x8926
#define GL_REG_6_ATI									0x8927
#define GL_REG_7_ATI									0x8928
#define GL_REG_8_ATI									0x8929
#define GL_REG_9_ATI									0x892A
#define GL_REG_10_ATI									0x892B
#define GL_REG_11_ATI									0x892C
#define GL_REG_12_ATI									0x892D
#define GL_REG_13_ATI									0x892E
#define GL_REG_14_ATI									0x892F
#define GL_REG_15_ATI									0x8930
#define GL_REG_16_ATI									0x8931
#define GL_REG_17_ATI									0x8932
#define GL_REG_18_ATI									0x8933
#define GL_REG_19_ATI									0x8934
#define GL_REG_20_ATI									0x8935
#define GL_REG_21_ATI									0x8936
#define GL_REG_22_ATI									0x8937
#define GL_REG_23_ATI									0x8938
#define GL_REG_24_ATI									0x8939
#define GL_REG_25_ATI									0x893A
#define GL_REG_26_ATI									0x893B
#define GL_REG_27_ATI									0x893C
#define GL_REG_28_ATI									0x893D
#define GL_REG_29_ATI									0x893E
#define GL_REG_30_ATI									0x893F
#define GL_REG_31_ATI									0x8940
#define GL_CON_0_ATI									0x8941
#define GL_CON_1_ATI									0x8942
#define GL_CON_2_ATI									0x8943
#define GL_CON_3_ATI									0x8944
#define GL_CON_4_ATI									0x8945
#define GL_CON_5_ATI									0x8946
#define GL_CON_6_ATI									0x8947
#define GL_CON_7_ATI									0x8948
#define GL_CON_8_ATI									0x8949
#define GL_CON_9_ATI									0x894A
#define GL_CON_10_ATI									0x894B
#define GL_CON_11_ATI									0x894C
#define GL_CON_12_ATI									0x894D
#define GL_CON_13_ATI									0x894E
#define GL_CON_14_ATI									0x894F
#define GL_CON_15_ATI									0x8950
#define GL_CON_16_ATI									0x8951
#define GL_CON_17_ATI									0x8952
#define GL_CON_18_ATI									0x8953
#define GL_CON_19_ATI									0x8954
#define GL_CON_20_ATI									0x8955
#define GL_CON_21_ATI									0x8956
#define GL_CON_22_ATI									0x8957
#define GL_CON_23_ATI									0x8958
#define GL_CON_24_ATI									0x8959
#define GL_CON_25_ATI									0x895A
#define GL_CON_26_ATI									0x895B
#define GL_CON_27_ATI									0x895C
#define GL_CON_28_ATI									0x895D
#define GL_CON_29_ATI									0x895E
#define GL_CON_30_ATI									0x895F
#define GL_CON_31_ATI									0x8960
#define GL_MOV_ATI										0x8961
#define GL_ADD_ATI										0x8963
#define GL_MUL_ATI										0x8964
#define GL_SUB_ATI										0x8965
#define GL_DOT3_ATI										0x8966
#define GL_DOT4_ATI										0x8967
#define GL_MAD_ATI										0x8968
#define GL_LERP_ATI										0x8969
#define GL_CND_ATI										0x896A
#define GL_CND0_ATI										0x896B
#define GL_DOT2_ADD_ATI									0x896C
#define GL_SECONDARY_INTERPOLATOR_ATI					0x896D
#define GL_NUM_FRAGMENT_REGISTERS_ATI					0x896E
#define GL_NUM_FRAGMENT_CONSTANTS_ATI					0x896F
#define GL_NUM_PASSES_ATI								0x8970
#define GL_NUM_INSTRUCTIONS_PER_PASS_ATI				0x8971
#define GL_NUM_INSTRUCTIONS_TOTAL_ATI					0x8972
#define GL_NUM_INPUT_INTERPOLATOR_COMPONENTS_ATI		0x8973
#define GL_NUM_LOOPBACK_COMPONENTS_ATI					0x8974
#define GL_COLOR_ALPHA_PAIRING_ATI						0x8975
#define GL_SWIZZLE_STR_ATI								0x8976
#define GL_SWIZZLE_STQ_ATI								0x8977
#define GL_SWIZZLE_STR_DR_ATI							0x8978
#define GL_SWIZZLE_STQ_DQ_ATI							0x8979
#define GL_SWIZZLE_STRQ_ATI								0x897A
#define GL_SWIZZLE_STRQ_DQ_ATI							0x897B
#define GL_RED_BIT_ATI									0x00000001
#define GL_GREEN_BIT_ATI								0x00000002
#define GL_BLUE_BIT_ATI									0x00000004
#define GL_2X_BIT_ATI									0x00000001
#define GL_4X_BIT_ATI									0x00000002
#define GL_8X_BIT_ATI									0x00000004
#define GL_HALF_BIT_ATI									0x00000008
#define GL_QUARTER_BIT_ATI								0x00000010
#define GL_EIGHTH_BIT_ATI								0x00000020
#define GL_SATURATE_BIT_ATI								0x00000040
#define GL_COMP_BIT_ATI									0x00000002
#define GL_NEGATE_BIT_ATI								0x00000004
#define GL_BIAS_BIT_ATI									0x00000008

extern GLuint (APIENTRY *glGenFragmentShadersATI)(GLuint range);
extern GLvoid (APIENTRY *glBindFragmentShaderATI)(GLuint id);
extern GLvoid (APIENTRY *glDeleteFragmentShaderATI)(GLuint id);
extern GLvoid (APIENTRY *glBeginFragmentShaderATI)(GLvoid);
extern GLvoid (APIENTRY *glEndFragmentShaderATI)(GLvoid);
extern GLvoid (APIENTRY *glPassTexCoordATI)(GLuint dst, GLuint coord, GLenum swizzle);
extern GLvoid (APIENTRY *glSampleMapATI)(GLuint dst, GLuint interp, GLenum swizzle);
extern GLvoid (APIENTRY *glColorFragmentOp1ATI)(GLenum op, GLuint dst, GLuint dstMask, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod);
extern GLvoid (APIENTRY *glColorFragmentOp2ATI)(GLenum op, GLuint dst, GLuint dstMask, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod);
extern GLvoid (APIENTRY *glColorFragmentOp3ATI)(GLenum op, GLuint dst, GLuint dstMask, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod, GLuint arg3, GLuint arg3Rep, GLuint arg3Mod);
extern GLvoid (APIENTRY *glAlphaFragmentOp1ATI)(GLenum op, GLuint dst, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod);
extern GLvoid (APIENTRY *glAlphaFragmentOp2ATI)(GLenum op, GLuint dst, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod);
extern GLvoid (APIENTRY *glAlphaFragmentOp3ATI)(GLenum op, GLuint dst, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod, GLuint arg3, GLuint arg3Rep, GLuint arg3Mod);
extern GLvoid (APIENTRY *glSetFragmentShaderConstantATI)(GLuint dst, const GLfloat *value);
extern int GL_ATI_fragment_shader_Flag;

#define GL_STENCIL_BACK_FUNC_ATI						0x8800
#define GL_STENCIL_BACK_FAIL_ATI						0x8801
#define GL_STENCIL_BACK_PASS_DEPTH_FAIL_ATI				0x8802
#define GL_STENCIL_BACK_PASS_DEPTH_PASS_ATI				0x8803

extern GLvoid (APIENTRY *glStencilOpSeparateATI)(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass);
extern GLvoid (APIENTRY *glStencilFuncSeparateATI)(GLenum frontfunc, GLenum backfunc, GLint ref, GLuint mask);
extern int GL_ATI_separate_stencil_Flag;

#define GL_MAX_DRAW_BUFFERS_ATI							0x8824
#define GL_DRAW_BUFFER0_ATI								0x8825
#define GL_DRAW_BUFFER1_ATI								0x8826
#define GL_DRAW_BUFFER2_ATI								0x8827
#define GL_DRAW_BUFFER3_ATI								0x8828
#define GL_DRAW_BUFFER4_ATI								0x8829
#define GL_DRAW_BUFFER5_ATI								0x882A
#define GL_DRAW_BUFFER6_ATI								0x882B
#define GL_DRAW_BUFFER7_ATI								0x882C
#define GL_DRAW_BUFFER8_ATI								0x882D
#define GL_DRAW_BUFFER9_ATI								0x882E
#define GL_DRAW_BUFFER10_ATI							0x882F
#define GL_DRAW_BUFFER11_ATI							0x8830
#define GL_DRAW_BUFFER12_ATI							0x8831
#define GL_DRAW_BUFFER13_ATI							0x8832
#define GL_DRAW_BUFFER14_ATI							0x8833
#define GL_DRAW_BUFFER15_ATI							0x8834

extern GLvoid (APIENTRY *glDrawBuffersATI)(GLsizei n, const GLenum *bufs);
extern int GL_ATI_draw_buffers_Flag;

#define GL_RGBA_FLOAT32_ATI								0x8814
#define GL_RGB_FLOAT32_ATI								0x8815
#define GL_ALPHA_FLOAT32_ATI							0x8816
#define GL_INTENSITY_FLOAT32_ATI						0x8817
#define GL_LUMINANCE_FLOAT32_ATI						0x8818
#define GL_LUMINANCE_ALPHA_FLOAT32_ATI					0x8819
#define GL_RGBA_FLOAT16_ATI								0x881A
#define GL_RGB_FLOAT16_ATI								0x881B
#define GL_ALPHA_FLOAT16_ATI							0x881C
#define GL_INTENSITY_FLOAT16_ATI						0x881D
#define GL_LUMINANCE_FLOAT16_ATI						0x881E
#define GL_LUMINANCE_ALPHA_FLOAT16_ATI					0x881F
extern int GL_ATI_texture_float_Flag;

#define GL_OCCLUSION_TEST_HP							0x8165
#define GL_OCCLUSION_TEST_RESULT_HP						0x8166
extern int GL_HP_occlusion_test_Flag;

#define GL_SAMPLES_PASSED_ARB							0x8914
#define GL_QUERY_COUNTER_BITS_ARB						0x8864
#define GL_CURRENT_QUERY_ARB							0x8865
#define GL_QUERY_RESULT_ARB								0x8866
#define GL_QUERY_RESULT_AVAILABLE_ARB					0x8867

extern GLvoid (APIENTRY *glGenQueriesARB)(GLsizei n, GLuint *ids);
extern GLvoid (APIENTRY *glDeleteQueriesARB)(GLsizei n, const GLuint *ids);
extern GLboolean (APIENTRY *glIsQueryARB)(GLuint id);
extern GLvoid (APIENTRY *glBeginQueryARB)(GLenum target, GLuint id);
extern GLvoid (APIENTRY *glEndQueryARB)(GLenum target);
extern GLvoid (APIENTRY *glGetQueryivARB)(GLenum target, GLenum pname, GLint *params);
extern GLvoid (APIENTRY *glGetQueryObjectivARB)(GLuint id, GLenum pname, GLint *params);
extern GLvoid (APIENTRY *glGetQueryObjectuivARB)(GLuint id, GLenum pname, GLuint *params);

extern int GL_ARB_occlusion_query_Flag;

#define GL_GENERATE_MIPMAP_SGIS							0x8191
#define GL_GENERATE_MIPMAP_HINT_SGIS					0x8192
extern int GL_SGIS_generate_mipmap_Flag;

#define GL_PROGRAM_OBJECT_ARB							0x8B40
#define GL_OBJECT_TYPE_ARB								0x8B4E
#define GL_OBJECT_SUBTYPE_ARB							0x8B4F
#define GL_OBJECT_DELETE_STATUS_ARB						0x8B80
#define GL_OBJECT_COMPILE_STATUS_ARB					0x8B81
#define GL_OBJECT_LINK_STATUS_ARB						0x8B82
#define GL_OBJECT_VALIDATE_STATUS_ARB					0x8B83
#define GL_OBJECT_INFO_LOG_LENGTH_ARB					0x8B84
#define GL_OBJECT_ATTACHED_OBJECTS_ARB					0x8B85
#define GL_OBJECT_ACTIVE_UNIFORMS_ARB					0x8B86
#define GL_OBJECT_ACTIVE_UNIFORM_MAX_LENGTH_ARB			0x8B87
#define GL_OBJECT_SHADER_SOURCE_LENGTH_ARB				0x8B88
#define GL_SHADER_OBJECT_ARB							0x8B48
#define GL_FLOAT_VEC2_ARB								0x8B50
#define GL_FLOAT_VEC3_ARB								0x8B51
#define GL_FLOAT_VEC4_ARB								0x8B52
#define GL_INT_VEC2_ARB									0x8B53
#define GL_INT_VEC3_ARB									0x8B54
#define GL_INT_VEC4_ARB									0x8B55
#define GL_BOOL_ARB										0x8B56
#define GL_BOOL_VEC2_ARB								0x8B57
#define GL_BOOL_VEC3_ARB								0x8B58
#define GL_BOOL_VEC4_ARB								0x8B59
#define GL_FLOAT_MAT2_ARB								0x8B5A
#define GL_FLOAT_MAT3_ARB								0x8B5B
#define GL_FLOAT_MAT4_ARB								0x8B5C

typedef char GLcharARB;
typedef unsigned int GLhandleARB;

extern GLvoid (APIENTRY *glDeleteObjectARB)(GLhandleARB obj);
extern GLhandleARB (APIENTRY *glGetHandleARB)(GLenum pname);
extern GLvoid (APIENTRY *glDetachObjectARB)(GLhandleARB containerObj, GLhandleARB attachedObj);
extern GLhandleARB (APIENTRY *glCreateShaderObjectARB)(GLenum shaderType);
extern GLvoid (APIENTRY *glShaderSourceARB)(GLhandleARB shaderObj, GLsizei count, const GLcharARB **string, const GLint *length);
extern GLvoid (APIENTRY *glCompileShaderARB)(GLhandleARB shaderObj);
extern GLhandleARB (APIENTRY *glCreateProgramObjectARB)(GLvoid);
extern GLvoid (APIENTRY *glAttachObjectARB)(GLhandleARB containerObj, GLhandleARB obj);
extern GLvoid (APIENTRY *glLinkProgramARB)(GLhandleARB programObj);
extern GLvoid (APIENTRY *glUseProgramObjectARB)(GLhandleARB programObj);
extern GLvoid (APIENTRY *glValidateProgramARB)(GLhandleARB programObj);
extern GLvoid (APIENTRY *glUniform1fARB)(GLint location, GLfloat v0);
extern GLvoid (APIENTRY *glUniform2fARB)(GLint location, GLfloat v0, GLfloat v1);
extern GLvoid (APIENTRY *glUniform3fARB)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
extern GLvoid (APIENTRY *glUniform4fARB)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
extern GLvoid (APIENTRY *glUniform1iARB)(GLint location, GLint v0);
extern GLvoid (APIENTRY *glUniform2iARB)(GLint location, GLint v0, GLint v1);
extern GLvoid (APIENTRY *glUniform3iARB)(GLint location, GLint v0, GLint v1, GLint v2);
extern GLvoid (APIENTRY *glUniform4iARB)(GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
extern GLvoid (APIENTRY *glUniform1fvARB)(GLint location, GLsizei count, GLfloat *value);
extern GLvoid (APIENTRY *glUniform2fvARB)(GLint location, GLsizei count, GLfloat *value);
extern GLvoid (APIENTRY *glUniform3fvARB)(GLint location, GLsizei count, GLfloat *value);
extern GLvoid (APIENTRY *glUniform4fvARB)(GLint location, GLsizei count, GLfloat *value);
extern GLvoid (APIENTRY *glUniform1ivARB)(GLint location, GLsizei count, GLint *value);
extern GLvoid (APIENTRY *glUniform2ivARB)(GLint location, GLsizei count, GLint *value);
extern GLvoid (APIENTRY *glUniform3ivARB)(GLint location, GLsizei count, GLint *value);
extern GLvoid (APIENTRY *glUniform4ivARB)(GLint location, GLsizei count, GLint *value);
extern GLvoid (APIENTRY *glUniformMatrix2fvARB)(GLint location, GLsizei count, GLboolean transpose, GLfloat *value);
extern GLvoid (APIENTRY *glUniformMatrix3fvARB)(GLint location, GLsizei count, GLboolean transpose, GLfloat *value);
extern GLvoid (APIENTRY *glUniformMatrix4fvARB)(GLint location, GLsizei count, GLboolean transpose, GLfloat *value);
extern GLvoid (APIENTRY *glGetObjectParameterfvARB)(GLhandleARB obj, GLenum pname, GLfloat *params);
extern GLvoid (APIENTRY *glGetObjectParameterivARB)(GLhandleARB obj, GLenum pname, GLint *params);
extern GLvoid (APIENTRY *glGetInfoLogARB)(GLhandleARB obj, GLsizei maxLength, GLsizei *length, GLcharARB *infoLog);
extern GLvoid (APIENTRY *glGetAttachedObjectsARB)(GLhandleARB containerObj, GLsizei maxCount, GLsizei *count, GLhandleARB *obj);
extern GLint (APIENTRY *glGetUniformLocationARB)(GLhandleARB programObj, const GLcharARB *name);
extern GLvoid (APIENTRY *glGetActiveUniformARB)(GLhandleARB programObj, GLuint index, GLsizei maxLength, GLsizei *length, GLint *size, GLenum *type, GLcharARB *name);
extern GLvoid (APIENTRY *glGetUniformfvARB)(GLhandleARB programObj, GLint location, GLfloat *params);
extern GLvoid (APIENTRY *glGetUniformivARB)(GLhandleARB programObj, GLint location, GLint *params);
extern GLvoid (APIENTRY *glGetShaderSourceARB)(GLhandleARB obj, GLsizei maxLength, GLsizei *length, GLcharARB *source);
extern int GL_ARB_shader_objects_Flag;

#define GL_VERTEX_SHADER_ARB							0x8B31
#define GL_MAX_VERTEX_UNIFORM_COMPONENTS_ARB			0x8B4A
#define GL_MAX_VARYING_FLOATS_ARB						0x8B4B
#define GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS_ARB			0x8B4C
#define GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS_ARB			0x8B4D
#define GL_OBJECT_ACTIVE_ATTRIBUTES_ARB					0x8B89
#define GL_OBJECT_ACTIVE_ATTRIBUTE_MAX_LENGTH_ARB		0x8B8A

extern GLvoid (APIENTRY *glBindAttribLocationARB)(GLhandleARB programObj, GLuint index, const GLcharARB *name);
extern GLvoid (APIENTRY *glGetActiveAttribARB)(GLhandleARB programObj, GLuint index, GLsizei maxLength, GLsizei *length, GLint *size, GLenum *type, GLcharARB *name);
extern GLint (APIENTRY *glGetAttribLocationARB)(GLhandleARB programObj, const GLcharARB *name);
extern int GL_ARB_vertex_shader_Flag;

#define GL_FRAGMENT_SHADER_ARB							0x8B30
#define GL_MAX_FRAGMENT_UNIFORM_COMPONENTS_ARB			0x8B49
extern int GL_ARB_fragment_shader_Flag;

extern const char *(WINAPI *wglGetExtensionsStringARB)(HDC hDC);
extern int WGL_ARB_extensions_string_Flag;

extern BOOL (WINAPI *wglSwapIntervalEXT)(int interval);
extern int (WINAPI *wglGetSwapIntervalEXT)(GLvoid);
extern int WGL_EXT_swap_control_Flag;

#define WGL_SAMPLE_BUFFERS_ARB							0x2041
#define WGL_SAMPLES_ARB									0x2042
extern int WGL_ARB_multisample_Flag;

#define WGL_NUMBER_PIXEL_FORMATS_ARB					0x2000
#define WGL_DRAW_TO_WINDOW_ARB							0x2001
#define WGL_DRAW_TO_BITMAP_ARB							0x2002
#define WGL_ACCELERATION_ARB							0x2003
#define WGL_NEED_PALETTE_ARB							0x2004
#define WGL_NEED_SYSTEM_PALETTE_ARB						0x2005
#define WGL_SWAP_LAYER_BUFFERS_ARB						0x2006
#define WGL_SWAP_METHOD_ARB								0x2007
#define WGL_NUMBER_OVERLAYS_ARB							0x2008
#define WGL_NUMBER_UNDERLAYS_ARB						0x2009
#define WGL_TRANSPARENT_ARB								0x200A
#define WGL_TRANSPARENT_RED_VALUE_ARB					0x2037
#define WGL_TRANSPARENT_GREEN_VALUE_ARB					0x2038
#define WGL_TRANSPARENT_BLUE_VALUE_ARB					0x2039
#define WGL_TRANSPARENT_ALPHA_VALUE_ARB					0x203A
#define WGL_TRANSPARENT_INDEX_VALUE_ARB					0x203B
#define WGL_SHARE_DEPTH_ARB								0x200C
#define WGL_SHARE_STENCIL_ARB							0x200D
#define WGL_SHARE_ACCUM_ARB								0x200E
#define WGL_SUPPORT_GDI_ARB								0x200F
#define WGL_SUPPORT_OPENGL_ARB							0x2010
#define WGL_DOUBLE_BUFFER_ARB							0x2011
#define WGL_STEREO_ARB									0x2012
#define WGL_PIXEL_TYPE_ARB								0x2013
#define WGL_COLOR_BITS_ARB								0x2014
#define WGL_RED_BITS_ARB								0x2015
#define WGL_RED_SHIFT_ARB								0x2016
#define WGL_GREEN_BITS_ARB								0x2017
#define WGL_GREEN_SHIFT_ARB								0x2018
#define WGL_BLUE_BITS_ARB								0x2019
#define WGL_BLUE_SHIFT_ARB								0x201A
#define WGL_ALPHA_BITS_ARB								0x201B
#define WGL_ALPHA_SHIFT_ARB								0x201C
#define WGL_ACCUM_BITS_ARB								0x201D
#define WGL_ACCUM_RED_BITS_ARB							0x201E
#define WGL_ACCUM_GREEN_BITS_ARB						0x201F
#define WGL_ACCUM_BLUE_BITS_ARB							0x2020
#define WGL_ACCUM_ALPHA_BITS_ARB						0x2021
#define WGL_DEPTH_BITS_ARB								0x2022
#define WGL_STENCIL_BITS_ARB							0x2023
#define WGL_AUX_BUFFERS_ARB								0x2024
#define WGL_NO_ACCELERATION_ARB							0x2025
#define WGL_GENERIC_ACCELERATION_ARB					0x2026
#define WGL_FULL_ACCELERATION_ARB						0x2027
#define WGL_SWAP_EXCHANGE_ARB							0x2028
#define WGL_SWAP_COPY_ARB								0x2029
#define WGL_SWAP_UNDEFINED_ARB							0x202A
#define WGL_TYPE_RGBA_ARB								0x202B
#define WGL_TYPE_COLORINDEX_ARB							0x202C

extern BOOL (WINAPI *wglGetPixelFormatAttribivARB)(HDC hDC, int iPixelFormat, int iLayerPlane, UINT nAttributes, const int *piAttributes, int *piValues);
extern BOOL (WINAPI *wglGetPixelFormatAttribfvARB)(HDC hDC, int iPixelFormat, int iLayerPlane, UINT nAttributes, const int *piAttributes, FLOAT *pfValues);
extern BOOL (WINAPI *wglChoosePixelFormatARB)(HDC hDC, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);
extern int WGL_ARB_pixel_format_Flag;

extern BOOL (WINAPI *wglMakeContextCurrentARB)(HDC hDrawDC, HDC hReadDC, HGLRC hGLRC);
extern HDC (WINAPI *wglGetCurrentReadDCARB)(GLvoid);
extern int WGL_ARB_make_current_read_Flag;

#define WGL_DRAW_TO_PBUFFER_ARB							0x202D
#define WGL_MAX_PBUFFER_PIXELS_ARB						0x202E
#define WGL_MAX_PBUFFER_WIDTH_ARB						0x202F
#define WGL_MAX_PBUFFER_HEIGHT_ARB						0x2030
#define WGL_PBUFFER_LARGEST_ARB							0x2033
#define WGL_PBUFFER_WIDTH_ARB							0x2034
#define WGL_PBUFFER_HEIGHT_ARB							0x2035
#define WGL_PBUFFER_LOST_ARB							0x2036

DECLARE_HANDLE(HPBUFFERARB);

extern HPBUFFERARB (WINAPI *wglCreatePbufferARB)(HDC hDC, int iPixelFormat, int iWidth, int iHeight, const int *piAttribList);
extern HDC (WINAPI *wglGetPbufferDCARB)(HPBUFFERARB hPbuffer);
extern int (WINAPI *wglReleasePbufferDCARB)(HPBUFFERARB hPbuffer, HDC hDC);
extern BOOL (WINAPI *wglDestroyPbufferARB)(HPBUFFERARB hPbuffer);
extern BOOL (WINAPI *wglQueryPbufferARB)(HPBUFFERARB hPbuffer, int iAttribute, int *piValue);
extern int WGL_ARB_pbuffer_Flag;

#define WGL_BIND_TO_TEXTURE_RGB_ARB						0x2070
#define WGL_BIND_TO_TEXTURE_RGBA_ARB					0x2071
#define WGL_TEXTURE_FORMAT_ARB							0x2072
#define WGL_TEXTURE_TARGET_ARB							0x2073
#define WGL_MIPMAP_TEXTURE_ARB							0x2074
#define WGL_TEXTURE_RGB_ARB								0x2075
#define WGL_TEXTURE_RGBA_ARB							0x2076
#define WGL_NO_TEXTURE_ARB								0x2077
#define WGL_TEXTURE_CUBE_MAP_ARB						0x2078
#define WGL_TEXTURE_1D_ARB								0x2079
#define WGL_TEXTURE_2D_ARB								0x207A
#define WGL_MIPMAP_LEVEL_ARB							0x207B
#define WGL_CUBE_MAP_FACE_ARB							0x207C
#define WGL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB				0x207D
#define WGL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB				0x207E
#define WGL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB				0x207F
#define WGL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB				0x2080
#define WGL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB				0x2081
#define WGL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB				0x2082
#define WGL_FRONT_LEFT_ARB								0x2083
#define WGL_FRONT_RIGHT_ARB								0x2084
#define WGL_BACK_LEFT_ARB								0x2085
#define WGL_BACK_RIGHT_ARB								0x2086
#define WGL_AUX0_ARB									0x2087
#define WGL_AUX1_ARB									0x2088
#define WGL_AUX2_ARB									0x2089
#define WGL_AUX3_ARB									0x208A
#define WGL_AUX4_ARB									0x208B
#define WGL_AUX5_ARB									0x208C
#define WGL_AUX6_ARB									0x208D
#define WGL_AUX7_ARB									0x208E
#define WGL_AUX8_ARB									0x208F
#define WGL_AUX9_ARB									0x2090

extern BOOL (WINAPI *wglBindTexImageARB)(HPBUFFERARB hPbuffer, int iBuffer);
extern BOOL (WINAPI *wglReleaseTexImageARB)(HPBUFFERARB hPbuffer, int iBuffer);
extern BOOL (WINAPI *wglSetPbufferAttribARB)(HPBUFFERARB hPbuffer, const int *piAttribList);
extern int WGL_ARB_render_texture_Flag;

#define WGL_TYPE_RGBA_FLOAT_ATI							0x21A0
#define GL_TYPE_RGBA_FLOAT_ATI							0x8820
#define GL_COLOR_CLEAR_UNCLAMPED_VALUE_ATI				0x8835
extern int WGL_ATI_pixel_format_float_Flag;

#define WGL_FLOAT_COMPONENTS_NV							0x20B0
#define WGL_BIND_TO_TEXTURE_RECTANGLE_FLOAT_R_NV		0x20B1
#define WGL_BIND_TO_TEXTURE_RECTANGLE_FLOAT_RG_NV		0x20B2
#define WGL_BIND_TO_TEXTURE_RECTANGLE_FLOAT_RGB_NV		0x20B3
#define WGL_BIND_TO_TEXTURE_RECTANGLE_FLOAT_RGBA_NV		0x20B4
#define WGL_TEXTURE_FLOAT_R_NV							0x20B5
#define WGL_TEXTURE_FLOAT_RG_NV							0x20B6
#define WGL_TEXTURE_FLOAT_RGB_NV						0x20B7
#define WGL_TEXTURE_FLOAT_RGBA_NV						0x20B8
extern int WGL_NV_float_buffer_Flag;

typedef struct
{
	HWND hWnd;
	HDC hDC;
	HPBUFFERARB hPbuffer;
	HGLRC hRC;
} GLContext_t;

int CreateContext(GLContext_t *Context, int DoubleBuffer, int Color, int Depth, int Stencil, int SampleBuffers);
int CreatePBuffer(GLContext_t *PBuffer, int Width, int Height, int DoubleBuffer, int FloatBuffer, int Color, int Depth, int Stencil, int SampleBuffers, int AuxBuffers, int RenderTexture);
void DestroyContext(GLContext_t *Context);

#endif
