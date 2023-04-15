#include "opengl.h"
#include <stdio.h>
#include <string.h>

#ifdef __linux__
extern Display *dpy;
#endif

GLvoid (APIENTRY *glSampleCoverageARB)(GLclampf value, GLboolean invert)=NULL;
int GL_ARB_multisample_Flag=0;

GLvoid (APIENTRY *glActiveTextureARB)(GLenum texture)=NULL;
GLvoid (APIENTRY *glClientActiveTextureARB)(GLenum texture)=NULL;
GLvoid (APIENTRY *glMultiTexCoord1dARB)(GLenum texture, GLdouble s)=NULL;
GLvoid (APIENTRY *glMultiTexCoord1dvARB)(GLenum texture, const GLdouble *v)=NULL;
GLvoid (APIENTRY *glMultiTexCoord1fARB)(GLenum texture, GLfloat s)=NULL;
GLvoid (APIENTRY *glMultiTexCoord1fvARB)(GLenum texture, const GLfloat *v)=NULL;
GLvoid (APIENTRY *glMultiTexCoord1iARB)(GLenum texture, GLint s)=NULL;
GLvoid (APIENTRY *glMultiTexCoord1ivARB)(GLenum texture, const GLint *v)=NULL;
GLvoid (APIENTRY *glMultiTexCoord1sARB)(GLenum texture, GLshort s)=NULL;
GLvoid (APIENTRY *glMultiTexCoord1svARB)(GLenum texture, const GLshort *v)=NULL;
GLvoid (APIENTRY *glMultiTexCoord2dARB)(GLenum texture, GLdouble s, GLdouble t)=NULL;
GLvoid (APIENTRY *glMultiTexCoord2dvARB)(GLenum texture, const GLdouble *v)=NULL;
GLvoid (APIENTRY *glMultiTexCoord2fARB)(GLenum texture, GLfloat s, GLfloat t)=NULL;
GLvoid (APIENTRY *glMultiTexCoord2fvARB)(GLenum texture, const GLfloat *v)=NULL;
GLvoid (APIENTRY *glMultiTexCoord2iARB)(GLenum texture, GLint s, GLint t)=NULL;
GLvoid (APIENTRY *glMultiTexCoord2ivARB)(GLenum texture, const GLint *v)=NULL;
GLvoid (APIENTRY *glMultiTexCoord2sARB)(GLenum texture, GLshort s, GLshort t)=NULL;
GLvoid (APIENTRY *glMultiTexCoord2svARB)(GLenum texture, const GLshort *v)=NULL;
GLvoid (APIENTRY *glMultiTexCoord3dARB)(GLenum texture, GLdouble s, GLdouble t, GLdouble r)=NULL;
GLvoid (APIENTRY *glMultiTexCoord3dvARB)(GLenum texture, const GLdouble *v)=NULL;
GLvoid (APIENTRY *glMultiTexCoord3fARB)(GLenum texture, GLfloat s, GLfloat t, GLfloat r)=NULL;
GLvoid (APIENTRY *glMultiTexCoord3fvARB)(GLenum texture, const GLfloat *v)=NULL;
GLvoid (APIENTRY *glMultiTexCoord3iARB)(GLenum texture, GLint s, GLint t, GLint r)=NULL;
GLvoid (APIENTRY *glMultiTexCoord3ivARB)(GLenum texture, const GLint *v)=NULL;
GLvoid (APIENTRY *glMultiTexCoord3sARB)(GLenum texture, GLshort s, GLshort t, GLshort r)=NULL;
GLvoid (APIENTRY *glMultiTexCoord3svARB)(GLenum texture, const GLshort *v)=NULL;
GLvoid (APIENTRY *glMultiTexCoord4dARB)(GLenum texture, GLdouble s, GLdouble t, GLdouble r, GLdouble q)=NULL;
GLvoid (APIENTRY *glMultiTexCoord4dvARB)(GLenum texture, const GLdouble *v)=NULL;
GLvoid (APIENTRY *glMultiTexCoord4fARB)(GLenum texture, GLfloat s, GLfloat t, GLfloat r, GLfloat q)=NULL;
GLvoid (APIENTRY *glMultiTexCoord4fvARB)(GLenum texture, const GLfloat *v)=NULL;
GLvoid (APIENTRY *glMultiTexCoord4iARB)(GLenum texture, GLint s, GLint t, GLint r, GLint q)=NULL;
GLvoid (APIENTRY *glMultiTexCoord4ivARB)(GLenum texture, const GLint *v)=NULL;
GLvoid (APIENTRY *glMultiTexCoord4sARB)(GLenum texture, GLshort s, GLshort t, GLshort r, GLshort q)=NULL;
GLvoid (APIENTRY *glMultiTexCoord4svARB)(GLenum texture, const GLshort *v)=NULL;
int GL_ARB_multitexture_Flag=0;

GLvoid (APIENTRY *glCompressedTexImage3DARB)(GLenum target, GLint level, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid *data)=NULL;
GLvoid (APIENTRY *glCompressedTexImage2DARB)(GLenum target, GLint level, GLenum internalFormat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data)=NULL;
GLvoid (APIENTRY *glCompressedTexImage1DARB)(GLenum target, GLint level, GLenum internalFormat, GLsizei width, GLint border, GLsizei imageSize, const GLvoid *data)=NULL;
GLvoid (APIENTRY *glCompressedTexSubImage3DARB)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid *data)=NULL;
GLvoid (APIENTRY *glCompressedTexSubImage2DARB)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid *data)=NULL;
GLvoid (APIENTRY *glCompressedTexSubImage1DARB)(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const GLvoid *data)=NULL;
GLvoid (APIENTRY *glGetCompressedTexImageARB)(GLenum target, GLint lod, GLvoid *img)=NULL;
int GL_ARB_texture_compression_Flag=0;

GLvoid (APIENTRY *glWeightbvARB)(GLint size, GLbyte *weights)=NULL;
GLvoid (APIENTRY *glWeightsvARB)(GLint size, GLshort *weights)=NULL;
GLvoid (APIENTRY *glWeightivARB)(GLint size, GLint *weights)=NULL;
GLvoid (APIENTRY *glWeightfvARB)(GLint size, GLfloat *weights)=NULL;
GLvoid (APIENTRY *glWeightdvARB)(GLint size, GLdouble *weights)=NULL;
GLvoid (APIENTRY *glWeightubvARB)(GLint size, GLubyte *weights)=NULL;
GLvoid (APIENTRY *glWeightusvARB)(GLint size, GLushort *weights)=NULL;
GLvoid (APIENTRY *glWeightuivARB)(GLint size, GLuint *weights)=NULL;
GLvoid (APIENTRY *glWeightPointerARB)(GLint size, GLenum type, GLsizei stride, GLvoid *pointer)=NULL;
GLvoid (APIENTRY *glVertexBlendARB)(GLint count)=NULL;
int GL_ARB_vertex_blend_Flag=0;

GLvoid (APIENTRY *glVertexAttrib1sARB)(GLuint index, GLshort x)=NULL;
GLvoid (APIENTRY *glVertexAttrib1fARB)(GLuint index, GLfloat x)=NULL;
GLvoid (APIENTRY *glVertexAttrib1dARB)(GLuint index, GLdouble x)=NULL;
GLvoid (APIENTRY *glVertexAttrib2sARB)(GLuint index, GLshort x, GLshort y)=NULL;
GLvoid (APIENTRY *glVertexAttrib2fARB)(GLuint index, GLfloat x, GLfloat y)=NULL;
GLvoid (APIENTRY *glVertexAttrib2dARB)(GLuint index, GLdouble x, GLdouble y)=NULL;
GLvoid (APIENTRY *glVertexAttrib3sARB)(GLuint index, GLshort x, GLshort y, GLshort z)=NULL;
GLvoid (APIENTRY *glVertexAttrib3fARB)(GLuint index, GLfloat x, GLfloat y, GLfloat z)=NULL;
GLvoid (APIENTRY *glVertexAttrib3dARB)(GLuint index, GLdouble x, GLdouble y, GLdouble z)=NULL;
GLvoid (APIENTRY *glVertexAttrib4sARB)(GLuint index, GLshort x, GLshort y, GLshort z, GLshort w)=NULL;
GLvoid (APIENTRY *glVertexAttrib4fARB)(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w)=NULL;
GLvoid (APIENTRY *glVertexAttrib4dARB)(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w)=NULL;
GLvoid (APIENTRY *glVertexAttrib4NubARB)(GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w)=NULL;
GLvoid (APIENTRY *glVertexAttrib1svARB)(GLuint index, const GLshort *v)=NULL;
GLvoid (APIENTRY *glVertexAttrib1fvARB)(GLuint index, const GLfloat *v)=NULL;
GLvoid (APIENTRY *glVertexAttrib1dvARB)(GLuint index, const GLdouble *v)=NULL;
GLvoid (APIENTRY *glVertexAttrib2svARB)(GLuint index, const GLshort *v)=NULL;
GLvoid (APIENTRY *glVertexAttrib2fvARB)(GLuint index, const GLfloat *v)=NULL;
GLvoid (APIENTRY *glVertexAttrib2dvARB)(GLuint index, const GLdouble *v)=NULL;
GLvoid (APIENTRY *glVertexAttrib3svARB)(GLuint index, const GLshort *v)=NULL;
GLvoid (APIENTRY *glVertexAttrib3fvARB)(GLuint index, const GLfloat *v)=NULL;
GLvoid (APIENTRY *glVertexAttrib3dvARB)(GLuint index, const GLdouble *v)=NULL;
GLvoid (APIENTRY *glVertexAttrib4svARB)(GLuint index, const GLbyte *v)=NULL;
GLvoid (APIENTRY *glVertexAttrib4fvARB)(GLuint index, const GLshort *v)=NULL;
GLvoid (APIENTRY *glVertexAttrib4dvARB)(GLuint index, const GLint *v)=NULL;
GLvoid (APIENTRY *glVertexAttrib4ivARB)(GLuint index, const GLubyte *v)=NULL;
GLvoid (APIENTRY *glVertexAttrib4bvARB)(GLuint index, const GLushort *v)=NULL;
GLvoid (APIENTRY *glVertexAttrib4usvARB)(GLuint index, const GLuint *v)=NULL;
GLvoid (APIENTRY *glVertexAttrib4ubvARB)(GLuint index, const GLfloat *v)=NULL;
GLvoid (APIENTRY *glVertexAttrib4uivARB)(GLuint index, const GLdouble *v)=NULL;
GLvoid (APIENTRY *glVertexAttrib4NbvARB)(GLuint index, const GLbyte *v)=NULL;
GLvoid (APIENTRY *glVertexAttrib4NsvARB)(GLuint index, const GLshort *v)=NULL;
GLvoid (APIENTRY *glVertexAttrib4NivARB)(GLuint index, const GLint *v)=NULL;
GLvoid (APIENTRY *glVertexAttrib4NubvARB)(GLuint index, const GLubyte *v)=NULL;
GLvoid (APIENTRY *glVertexAttrib4NusvARB)(GLuint index, const GLushort *v)=NULL;
GLvoid (APIENTRY *glVertexAttrib4NuivARB)(GLuint index, const GLuint *v)=NULL;
GLvoid (APIENTRY *glVertexAttribPointerARB)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer)=NULL;
GLvoid (APIENTRY *glEnableVertexAttribArrayARB)(GLuint index)=NULL;
GLvoid (APIENTRY *glDisableVertexAttribArrayARB)(GLuint index)=NULL;
GLvoid (APIENTRY *glProgramStringARB)(GLenum target, GLenum format, GLsizei len, const GLvoid *string)=NULL;
GLvoid (APIENTRY *glBindProgramARB)(GLenum target, GLuint program)=NULL;
GLvoid (APIENTRY *glDeleteProgramsARB)(GLsizei n, const GLuint *programs)=NULL;
GLvoid (APIENTRY *glGenProgramsARB)(GLsizei n, GLuint *programs)=NULL;
GLvoid (APIENTRY *glProgramEnvParameter4dARB)(GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w)=NULL;
GLvoid (APIENTRY *glProgramEnvParameter4dvARB)(GLenum target, GLuint index, const GLdouble *params)=NULL;
GLvoid (APIENTRY *glProgramEnvParameter4fARB)(GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w)=NULL;
GLvoid (APIENTRY *glProgramEnvParameter4fvARB)(GLenum target, GLuint index, const GLfloat *params)=NULL;
GLvoid (APIENTRY *glProgramLocalParameter4dARB)(GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w)=NULL;
GLvoid (APIENTRY *glProgramLocalParameter4dvARB)(GLenum target, GLuint index, const GLdouble *params)=NULL;
GLvoid (APIENTRY *glProgramLocalParameter4fARB)(GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w)=NULL;
GLvoid (APIENTRY *glProgramLocalParameter4fvARB)(GLenum target, GLuint index, const GLfloat *params)=NULL;
GLvoid (APIENTRY *glGetProgramEnvParameterdvARB)(GLenum target, GLuint index, GLdouble *params)=NULL;
GLvoid (APIENTRY *glGetProgramEnvParameterfvARB)(GLenum target, GLuint index, GLfloat *params)=NULL;
GLvoid (APIENTRY *glGetProgramLocalParameterdvARB)(GLenum target, GLuint index, GLdouble *params)=NULL;
GLvoid (APIENTRY *glGetProgramLocalParameterfvARB)(GLenum target, GLuint index, GLfloat *params)=NULL;
GLvoid (APIENTRY *glGetProgramivARB)(GLenum target, GLenum pname, GLint *params)=NULL;
GLvoid (APIENTRY *glGetProgramStringARB)(GLenum target, GLenum pname, GLvoid *string)=NULL;
GLvoid (APIENTRY *glGetVertexAttribdvARB)(GLuint index, GLenum pname, GLdouble *params)=NULL;
GLvoid (APIENTRY *glGetVertexAttribfvARB)(GLuint index, GLenum pname, GLfloat *params)=NULL;
GLvoid (APIENTRY *glGetVertexAttribivARB)(GLuint index, GLenum pname, GLint *params)=NULL;
GLvoid (APIENTRY *glGetVertexAttribPointervARB)(GLuint index, GLenum pname, GLvoid **pointer)=NULL;
GLboolean (APIENTRY *glIsProgramARB)(GLuint program)=NULL;
int GL_ARB_vertex_program_Flag=0;

int GL_ARB_fragment_program_Flag=0;

int GL_ARB_depth_texture_Flag=0;

int GL_ARB_shadow_Flag=0;

int GL_ARB_shadow_ambient_Flag=0;

int GL_ARB_texture_cube_map_Flag=0;

int GL_ARB_texture_non_power_of_two_Flag=0;

int GL_ARB_texture_env_add_Flag=0;

int GL_ARB_texture_env_combine_Flag=0;

int GL_ARB_texture_env_crossbar_Flag=0;

int GL_ARB_texture_env_dot3_Flag=0;

GLvoid (APIENTRY *glBindBufferARB)(GLenum target, GLuint buffer)=NULL;
GLvoid (APIENTRY *glDeleteBuffersARB)(GLsizei n, const GLuint *buffers)=NULL;
GLvoid (APIENTRY *glGenBuffersARB)(GLsizei n, GLuint *buffers)=NULL;
GLboolean (APIENTRY *glIsBufferARB)(GLuint buffer)=NULL;
GLvoid (APIENTRY *glBufferDataARB)(GLenum target, GLsizeiptrARB size, const GLvoid *data, GLenum usage)=NULL;
GLvoid (APIENTRY *glBufferSubDataARB)(GLenum target, GLintptrARB offset, GLsizeiptrARB size, const GLvoid *data)=NULL;
GLvoid (APIENTRY *glGetBufferSubDataARB)(GLenum target, GLintptrARB offset, GLsizeiptrARB size, GLvoid *data)=NULL;
GLvoid *(APIENTRY *glMapBufferARB)(GLenum target, GLenum access)=NULL;
GLboolean (APIENTRY *glUnmapBufferARB)(GLenum target)=NULL;
GLvoid (APIENTRY *glGetBufferParameterivARB)(GLenum target, GLenum pname, GLint *params)=NULL;
GLvoid (APIENTRY *glGetBufferPointervARB)(GLenum target, GLenum pname, GLvoid **params)=NULL;
int GL_ARB_vertex_buffer_object_Flag=0;

int GL_ARB_pixel_buffer_object_Flag=0;

GLboolean (APIENTRY *glIsRenderbufferEXT)(GLuint renderbuffer)=NULL;
GLvoid (APIENTRY *glBindRenderbufferEXT)(GLenum target, GLuint renderbuffer)=NULL;
GLvoid (APIENTRY *glDeleteRenderbuffersEXT)(GLsizei n, const GLuint *renderbuffers)=NULL;
GLvoid (APIENTRY *glGenRenderbuffersEXT)(GLsizei n, GLuint *renderbuffers)=NULL;
GLvoid (APIENTRY *glRenderbufferStorageEXT)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height)=NULL;
GLvoid (APIENTRY *glGetRenderbufferParameterivEXT)(GLenum target, GLenum pname, GLint *params)=NULL;
GLboolean (APIENTRY *glIsFramebufferEXT)(GLuint framebuffer)=NULL;
GLvoid (APIENTRY *glBindFramebufferEXT)(GLenum target, GLuint framebuffer)=NULL;
GLvoid (APIENTRY *glDeleteFramebuffersEXT)(GLsizei n, const GLuint *framebuffers)=NULL;
GLvoid (APIENTRY *glGenFramebuffersEXT)(GLsizei n, GLuint *framebuffers)=NULL;
GLenum (APIENTRY *glCheckFramebufferStatusEXT)(GLenum target)=NULL;
GLvoid (APIENTRY *glFramebufferTexture1DEXT)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)=NULL;
GLvoid (APIENTRY *glFramebufferTexture2DEXT)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)=NULL;
GLvoid (APIENTRY *glFramebufferTexture3DEXT)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset)=NULL;
GLvoid (APIENTRY *glFramebufferRenderbufferEXT)(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)=NULL;
GLvoid (APIENTRY *glGetFramebufferAttachmentParameterivEXT)(GLenum target, GLenum attachment, GLenum pname, GLint *params)=NULL;
GLvoid (APIENTRY *glGenerateMipmapEXT)(GLenum target)=NULL;
int GL_EXT_framebuffer_object_Flag=0;

GLvoid (APIENTRY *glTexImage3DEXT)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid *pixels)=NULL;
GLvoid (APIENTRY *glTexSubImage3DEXT)(GLenum target, GLint lod, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei w, GLsizei h, GLsizei d, GLenum format, GLenum type, const GLvoid *buf)=NULL;
GLvoid (APIENTRY *glCopyTexSubImage3DEXT)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height)=NULL;
int GL_EXT_texture3D_Flag=0;

GLvoid (APIENTRY *glPointParameterfARB)(GLenum pname, GLfloat param)=NULL;
GLvoid (APIENTRY *glPointParameterfvARB)(GLenum pname, GLfloat *params)=NULL;
int GL_ARB_point_parameters_Flag=0;

int GL_ARB_point_sprite_Flag=0;

int GL_EXT_abgr_Flag=0;

int GL_EXT_bgra_Flag=0;

int GL_EXT_packed_pixels_Flag=0;

int GL_EXT_texture_compression_s3tc_Flag=0;

int GL_EXT_texture_edge_clamp_Flag=0;

int GL_EXT_texture_env_combine_Flag=0;

int GL_EXT_texture_filter_anisotropic_Flag=0;

int GL_EXT_stencil_wrap_Flag=0;

int GL_EXT_texture_rectangle_Flag=0;

GLuint (APIENTRY *glGenFragmentShadersATI)(GLuint range)=NULL;
GLvoid (APIENTRY *glBindFragmentShaderATI)(GLuint id)=NULL;
GLvoid (APIENTRY *glDeleteFragmentShaderATI)(GLuint id)=NULL;
GLvoid (APIENTRY *glBeginFragmentShaderATI)(GLvoid)=NULL;
GLvoid (APIENTRY *glEndFragmentShaderATI)(GLvoid)=NULL;
GLvoid (APIENTRY *glPassTexCoordATI)(GLuint dst, GLuint coord, GLenum swizzle)=NULL;
GLvoid (APIENTRY *glSampleMapATI)(GLuint dst, GLuint interp, GLenum swizzle)=NULL;
GLvoid (APIENTRY *glColorFragmentOp1ATI)(GLenum op, GLuint dst, GLuint dstMask, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod)=NULL;
GLvoid (APIENTRY *glColorFragmentOp2ATI)(GLenum op, GLuint dst, GLuint dstMask, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod)=NULL;
GLvoid (APIENTRY *glColorFragmentOp3ATI)(GLenum op, GLuint dst, GLuint dstMask, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod, GLuint arg3, GLuint arg3Rep, GLuint arg3Mod)=NULL;
GLvoid (APIENTRY *glAlphaFragmentOp1ATI)(GLenum op, GLuint dst, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod)=NULL;
GLvoid (APIENTRY *glAlphaFragmentOp2ATI)(GLenum op, GLuint dst, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod)=NULL;
GLvoid (APIENTRY *glAlphaFragmentOp3ATI)(GLenum op, GLuint dst, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod, GLuint arg3, GLuint arg3Rep, GLuint arg3Mod)=NULL;
GLvoid (APIENTRY *glSetFragmentShaderConstantATI)(GLuint dst, const GLfloat *value)=NULL;
int GL_ATI_fragment_shader_Flag=0;

GLvoid (APIENTRY *glStencilOpSeparateATI)(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass)=NULL;
GLvoid (APIENTRY *glStencilFuncSeparateATI)(GLenum frontfunc, GLenum backfunc, GLint ref, GLuint mask)=NULL;
int GL_ATI_separate_stencil_Flag=0;

GLvoid (APIENTRY *glDrawBuffersATI)(GLsizei n, const GLenum *bufs)=NULL;
int GL_ATI_draw_buffers_Flag=0;

int GL_ATI_texture_float_Flag=0;

int GL_HP_occlusion_test_Flag=0;

GLvoid (APIENTRY *glGenQueriesARB)(GLsizei n, GLuint *ids)=NULL;
GLvoid (APIENTRY *glDeleteQueriesARB)(GLsizei n, const GLuint *ids)=NULL;
GLboolean (APIENTRY *glIsQueryARB)(GLuint id)=NULL;
GLvoid (APIENTRY *glBeginQueryARB)(GLenum target, GLuint id)=NULL;
GLvoid (APIENTRY *glEndQueryARB)(GLenum target)=NULL;
GLvoid (APIENTRY *glGetQueryivARB)(GLenum target, GLenum pname, GLint *params)=NULL;
GLvoid (APIENTRY *glGetQueryObjectivARB)(GLuint id, GLenum pname, GLint *params)=NULL;
GLvoid (APIENTRY *glGetQueryObjectuivARB)(GLuint id, GLenum pname, GLuint *params)=NULL;
int GL_ARB_occlusion_query_Flag=0;

int GL_SGIS_generate_mipmap_Flag=0;

GLvoid (APIENTRY *glDeleteObjectARB)(GLhandleARB obj)=NULL;
GLhandleARB (APIENTRY *glGetHandleARB)(GLenum pname)=NULL;
GLvoid (APIENTRY *glDetachObjectARB)(GLhandleARB containerObj, GLhandleARB attachedObj)=NULL;
GLhandleARB (APIENTRY *glCreateShaderObjectARB)(GLenum shaderType)=NULL;
GLvoid (APIENTRY *glShaderSourceARB)(GLhandleARB shaderObj, GLsizei count, const GLcharARB **string, const GLint *length)=NULL;
GLvoid (APIENTRY *glCompileShaderARB)(GLhandleARB shaderObj)=NULL;
GLhandleARB (APIENTRY *glCreateProgramObjectARB)(GLvoid)=NULL;
GLvoid (APIENTRY *glAttachObjectARB)(GLhandleARB containerObj, GLhandleARB obj)=NULL;
GLvoid (APIENTRY *glLinkProgramARB)(GLhandleARB programObj)=NULL;
GLvoid (APIENTRY *glUseProgramObjectARB)(GLhandleARB programObj)=NULL;
GLvoid (APIENTRY *glValidateProgramARB)(GLhandleARB programObj)=NULL;
GLvoid (APIENTRY *glUniform1fARB)(GLint location, GLfloat v0)=NULL;
GLvoid (APIENTRY *glUniform2fARB)(GLint location, GLfloat v0, GLfloat v1)=NULL;
GLvoid (APIENTRY *glUniform3fARB)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2)=NULL;
GLvoid (APIENTRY *glUniform4fARB)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)=NULL;
GLvoid (APIENTRY *glUniform1iARB)(GLint location, GLint v0)=NULL;
GLvoid (APIENTRY *glUniform2iARB)(GLint location, GLint v0, GLint v1)=NULL;
GLvoid (APIENTRY *glUniform3iARB)(GLint location, GLint v0, GLint v1, GLint v2)=NULL;
GLvoid (APIENTRY *glUniform4iARB)(GLint location, GLint v0, GLint v1, GLint v2, GLint v3)=NULL;
GLvoid (APIENTRY *glUniform1fvARB)(GLint location, GLsizei count, GLfloat *value)=NULL;
GLvoid (APIENTRY *glUniform2fvARB)(GLint location, GLsizei count, GLfloat *value)=NULL;
GLvoid (APIENTRY *glUniform3fvARB)(GLint location, GLsizei count, GLfloat *value)=NULL;
GLvoid (APIENTRY *glUniform4fvARB)(GLint location, GLsizei count, GLfloat *value)=NULL;
GLvoid (APIENTRY *glUniform1ivARB)(GLint location, GLsizei count, GLint *value)=NULL;
GLvoid (APIENTRY *glUniform2ivARB)(GLint location, GLsizei count, GLint *value)=NULL;
GLvoid (APIENTRY *glUniform3ivARB)(GLint location, GLsizei count, GLint *value)=NULL;
GLvoid (APIENTRY *glUniform4ivARB)(GLint location, GLsizei count, GLint *value)=NULL;
GLvoid (APIENTRY *glUniformMatrix2fvARB)(GLint location, GLsizei count, GLboolean transpose, GLfloat *value)=NULL;
GLvoid (APIENTRY *glUniformMatrix3fvARB)(GLint location, GLsizei count, GLboolean transpose, GLfloat *value)=NULL;
GLvoid (APIENTRY *glUniformMatrix4fvARB)(GLint location, GLsizei count, GLboolean transpose, GLfloat *value)=NULL;
GLvoid (APIENTRY *glGetObjectParameterfvARB)(GLhandleARB obj, GLenum pname, GLfloat *params)=NULL;
GLvoid (APIENTRY *glGetObjectParameterivARB)(GLhandleARB obj, GLenum pname, GLint *params)=NULL;
GLvoid (APIENTRY *glGetInfoLogARB)(GLhandleARB obj, GLsizei maxLength, GLsizei *length, GLcharARB *infoLog)=NULL;
GLvoid (APIENTRY *glGetAttachedObjectsARB)(GLhandleARB containerObj, GLsizei maxCount, GLsizei *count, GLhandleARB *obj)=NULL;
GLint (APIENTRY *glGetUniformLocationARB)(GLhandleARB programObj, const GLcharARB *name)=NULL;
GLvoid (APIENTRY *glGetActiveUniformARB)(GLhandleARB programObj, GLuint index, GLsizei maxLength, GLsizei *length, GLint *size, GLenum *type, GLcharARB *name)=NULL;
GLvoid (APIENTRY *glGetUniformfvARB)(GLhandleARB programObj, GLint location, GLfloat *params)=NULL;
GLvoid (APIENTRY *glGetUniformivARB)(GLhandleARB programObj, GLint location, GLint *params)=NULL;
GLvoid (APIENTRY *glGetShaderSourceARB)(GLhandleARB obj, GLsizei maxLength, GLsizei *length, GLcharARB *source)=NULL;
int GL_ARB_shader_objects_Flag=0;

GLvoid (APIENTRY *glBindAttribLocationARB)(GLhandleARB programObj, GLuint index, const GLcharARB *name)=NULL;
GLvoid (APIENTRY *glGetActiveAttribARB)(GLhandleARB programObj, GLuint index, GLsizei maxLength, GLsizei *length, GLint *size, GLenum *type, GLcharARB *name)=NULL;
GLint (APIENTRY *glGetAttribLocationARB)(GLhandleARB programObj, const GLcharARB *name)=NULL;
int GL_ARB_vertex_shader_Flag=0;

int GL_ARB_fragment_shader_Flag=0;

#ifdef WIN32
const char *(WINAPI *wglGetExtensionsStringARB)(HDC hDC)=NULL;
int WGL_ARB_extensions_string_Flag=0;

BOOL (WINAPI *wglSwapIntervalEXT)(int interval)=NULL;
int (WINAPI *wglGetSwapIntervalEXT)(VOID)=NULL;
int WGL_EXT_swap_control_Flag=0;

int WGL_ARB_multisample_Flag=0;

BOOL (WINAPI *wglGetPixelFormatAttribivARB)(HDC hDC, int iPixelFormat, int iLayerPlane, UINT nAttributes, const int *piAttributes, int *piValues)=NULL;
BOOL (WINAPI *wglGetPixelFormatAttribfvARB)(HDC hDC, int iPixelFormat, int iLayerPlane, UINT nAttributes, const int *piAttributes, FLOAT *pfValues)=NULL;
BOOL (WINAPI *wglChoosePixelFormatARB)(HDC hDC, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats)=NULL;
int WGL_ARB_pixel_format_Flag=0;

BOOL (WINAPI *wglMakeContextCurrentARB)(HDC hDrawDC, HDC hReadDC, HGLRC hGLRC)=NULL;
HDC (WINAPI *wglGetCurrentReadDCARB)(VOID)=NULL;
int WGL_ARB_make_current_read_Flag=0;

HPBUFFERARB (WINAPI *wglCreatePbufferARB)(HDC hDC, int iPixelFormat, int iWidth, int iHeight, const int *piAttribList)=NULL;
HDC (WINAPI *wglGetPbufferDCARB)(HPBUFFERARB hPbuffer)=NULL;
int (WINAPI *wglReleasePbufferDCARB)(HPBUFFERARB hPbuffer, HDC hDC)=NULL;
BOOL (WINAPI *wglDestroyPbufferARB)(HPBUFFERARB hPbuffer)=NULL;
BOOL (WINAPI *wglQueryPbufferARB)(HPBUFFERARB hPbuffer, int iAttribute, int *piValue)=NULL;
int WGL_ARB_pbuffer_Flag=0;

BOOL (WINAPI *wglBindTexImageARB)(HPBUFFERARB hPbuffer, int iBuffer)=NULL;
BOOL (WINAPI *wglReleaseTexImageARB)(HPBUFFERARB hPbuffer, int iBuffer)=NULL;
BOOL (WINAPI *wglSetPbufferAttribARB)(HPBUFFERARB hPbuffer, const int *piAttribList)=NULL;
int WGL_ARB_render_texture_Flag=0;

int WGL_ATI_pixel_format_float_Flag=0;

int WGL_NV_float_buffer_Flag=0;

int WGL_NV_render_texture_rectangle_Flag=0;

int WGL_ATI_render_texture_rectangle_Flag=0;
#else
GLvoid (APIENTRY *glXBindTexImageATI)(Display *dpy, GLXPbuffer pbuf, int buffer)=NULL;
GLvoid (APIENTRY *glXReleaseTexImageATI)(Display *dpy, GLXPbuffer pbuf, int buffer)=NULL;
GLvoid (APIENTRY *glXDrawableAttribATI)(Display *dpy, GLXDrawable draw, const int *attrib_list)=NULL;
int GLX_ATI_render_texture_Flag=0;

int GLX_ARB_multisample_Flag=0;

int GLX_ATI_pixel_format_float_Flag=0;

int GLX_NV_float_buffer_Flag=0;
#endif

int GLExtensionsInitialized_Flag=0;

#ifdef WIN32
int WGLExtensionsInitialized_Flag=0;
#else
int GLXExtensionsInitialized_Flag=0;
#endif

void *glGetProcAddress(char *Name, int *Return)
{
	void *FuncPtr=NULL;

#ifdef WIN32
	FuncPtr=(void *)wglGetProcAddress(Name);
#else
	FuncPtr=(void *)glXGetProcAddressARB(Name);
#endif

	if(FuncPtr!=NULL)
		*Return&=1;
	else
		*Return&=0;

	return FuncPtr;
}

int FindSubString(char *string, char *substring)
{
	if(string)
	{
		char *str=string;
		size_t len=strlen(substring);

		while((str=strstr(str, substring))!=NULL)
		{
			str+=len;

			if(*str==' '||*str=='\0')
				return 1;
		}
	}

	return 0;
}

void GLSetupExtensions(void)
{
	int Return=1;
	char *GLExtensionString=NULL;

	// GL Extensions
	GLExtensionString=(char *)glGetString(GL_EXTENSIONS);

	if(FindSubString(GLExtensionString, "GL_ARB_multisample"))
	{
		glSampleCoverageARB=glGetProcAddress("glSampleCoverageARB", &Return);

		if(Return)
			GL_ARB_multisample_Flag=1;
	}

	if(FindSubString(GLExtensionString, "GL_ARB_multitexture"))
	{
		Return=1;

		glActiveTextureARB=glGetProcAddress("glActiveTextureARB", &Return);
		glClientActiveTextureARB=glGetProcAddress("glClientActiveTextureARB", &Return);
		glMultiTexCoord1dARB=glGetProcAddress("glMultiTexCoord1dARB", &Return);
		glMultiTexCoord1dvARB=glGetProcAddress("glMultiTexCoord1dvARB", &Return);
		glMultiTexCoord1fARB=glGetProcAddress("glMultiTexCoord1fARB", &Return);
		glMultiTexCoord1fvARB=glGetProcAddress("glMultiTexCoord1fvARB", &Return);
		glMultiTexCoord1iARB=glGetProcAddress("glMultiTexCoord1iARB", &Return);
		glMultiTexCoord1ivARB=glGetProcAddress("glMultiTexCoord1ivARB", &Return);
		glMultiTexCoord1sARB=glGetProcAddress("glMultiTexCoord1sARB", &Return);
		glMultiTexCoord1svARB=glGetProcAddress("glMultiTexCoord1svARB", &Return);
		glMultiTexCoord2dARB=glGetProcAddress("glMultiTexCoord2dARB", &Return);
		glMultiTexCoord2dvARB=glGetProcAddress("glMultiTexCoord2dvARB", &Return);
		glMultiTexCoord2fARB=glGetProcAddress("glMultiTexCoord2fARB", &Return);
		glMultiTexCoord2fvARB=glGetProcAddress("glMultiTexCoord2fvARB", &Return);
		glMultiTexCoord2iARB=glGetProcAddress("glMultiTexCoord2iARB", &Return);
		glMultiTexCoord2ivARB=glGetProcAddress("glMultiTexCoord2ivARB", &Return);
		glMultiTexCoord2sARB=glGetProcAddress("glMultiTexCoord2sARB", &Return);
		glMultiTexCoord2svARB=glGetProcAddress("glMultiTexCoord2svARB", &Return);
		glMultiTexCoord3dARB=glGetProcAddress("glMultiTexCoord3dARB", &Return);
		glMultiTexCoord3dvARB=glGetProcAddress("glMultiTexCoord3dvARB", &Return);
		glMultiTexCoord3fARB=glGetProcAddress("glMultiTexCoord3fARB", &Return);
		glMultiTexCoord3fvARB=glGetProcAddress("glMultiTexCoord3fvARB", &Return);
		glMultiTexCoord3iARB=glGetProcAddress("glMultiTexCoord3iARB", &Return);
		glMultiTexCoord3ivARB=glGetProcAddress("glMultiTexCoord3ivARB", &Return);
		glMultiTexCoord3sARB=glGetProcAddress("glMultiTexCoord3sARB", &Return);
		glMultiTexCoord3svARB=glGetProcAddress("glMultiTexCoord3svARB", &Return);
		glMultiTexCoord4dARB=glGetProcAddress("glMultiTexCoord4dARB", &Return);
		glMultiTexCoord4dvARB=glGetProcAddress("glMultiTexCoord4dvARB", &Return);
		glMultiTexCoord4fARB=glGetProcAddress("glMultiTexCoord4fARB", &Return);
		glMultiTexCoord4fvARB=glGetProcAddress("glMultiTexCoord4fvARB", &Return);
		glMultiTexCoord4iARB=glGetProcAddress("glMultiTexCoord4iARB", &Return);
		glMultiTexCoord4ivARB=glGetProcAddress("glMultiTexCoord4ivARB", &Return);
		glMultiTexCoord4sARB=glGetProcAddress("glMultiTexCoord4sARB", &Return);
		glMultiTexCoord4svARB=glGetProcAddress("glMultiTexCoord4svARB", &Return);

		if(Return)
			GL_ARB_multitexture_Flag=1;
	}

	if(FindSubString(GLExtensionString, "GL_ARB_texture_compression"))
	{
		Return=1;

		glCompressedTexImage3DARB=glGetProcAddress("glCompressedTexImage3DARB", &Return);
		glCompressedTexImage2DARB=glGetProcAddress("glCompressedTexImage2DARB", &Return);
		glCompressedTexImage1DARB=glGetProcAddress("glCompressedTexImage1DARB", &Return);
		glCompressedTexSubImage3DARB=glGetProcAddress("glCompressedTexSubImage3DARB", &Return);
		glCompressedTexSubImage2DARB=glGetProcAddress("glCompressedTexSubImage2DARB", &Return);
		glCompressedTexSubImage1DARB=glGetProcAddress("glCompressedTexSubImage1DARB", &Return);
		glGetCompressedTexImageARB=glGetProcAddress("glGetCompressedTexImageARB", &Return);

		if(Return)
			GL_ARB_texture_compression_Flag=1;
	}

	if(FindSubString(GLExtensionString, "GL_ARB_vertex_blend"))
	{
		Return=1;

		glWeightbvARB=glGetProcAddress("glWeightbvARB", &Return);
		glWeightsvARB=glGetProcAddress("glWeightsvARB", &Return);
		glWeightivARB=glGetProcAddress("glWeightivARB", &Return);
		glWeightfvARB=glGetProcAddress("glWeightfvARB", &Return);
		glWeightdvARB=glGetProcAddress("glWeightdvARB", &Return);
		glWeightubvARB=glGetProcAddress("glWeightubvARB", &Return);
		glWeightusvARB=glGetProcAddress("glWeightusvARB", &Return);
		glWeightuivARB=glGetProcAddress("glWeightuivARB", &Return);
		glWeightPointerARB=glGetProcAddress("glWeightPointerARB", &Return);
		glVertexBlendARB=glGetProcAddress("glVertexBlendARB", &Return);

		if(Return)
			GL_ARB_vertex_blend_Flag=1;
	}

	if(FindSubString(GLExtensionString,"GL_ARB_vertex_program"))
	{
		Return=1;

		glVertexAttrib1sARB=glGetProcAddress("glVertexAttrib1sARB", &Return);
		glVertexAttrib1fARB=glGetProcAddress("glVertexAttrib1fARB", &Return);
		glVertexAttrib1dARB=glGetProcAddress("glVertexAttrib1dARB", &Return);
		glVertexAttrib2sARB=glGetProcAddress("glVertexAttrib2sARB", &Return);
		glVertexAttrib2fARB=glGetProcAddress("glVertexAttrib2fARB", &Return);
		glVertexAttrib2dARB=glGetProcAddress("glVertexAttrib2dARB", &Return);
		glVertexAttrib3sARB=glGetProcAddress("glVertexAttrib3sARB", &Return);
		glVertexAttrib3fARB=glGetProcAddress("glVertexAttrib3fARB", &Return);
		glVertexAttrib3dARB=glGetProcAddress("glVertexAttrib3dARB", &Return);
		glVertexAttrib4sARB=glGetProcAddress("glVertexAttrib4sARB", &Return);
		glVertexAttrib4fARB=glGetProcAddress("glVertexAttrib4fARB", &Return);
		glVertexAttrib4dARB=glGetProcAddress("glVertexAttrib4dARB", &Return);
		glVertexAttrib4NubARB=glGetProcAddress("glVertexAttrib4NubARB", &Return);
		glVertexAttrib1svARB=glGetProcAddress("glVertexAttrib1svARB", &Return);
		glVertexAttrib1fvARB=glGetProcAddress("glVertexAttrib1fvARB", &Return);
		glVertexAttrib1dvARB=glGetProcAddress("glVertexAttrib1dvARB", &Return);
		glVertexAttrib2svARB=glGetProcAddress("glVertexAttrib2svARB", &Return);
		glVertexAttrib2fvARB=glGetProcAddress("glVertexAttrib2fvARB", &Return);
		glVertexAttrib2dvARB=glGetProcAddress("glVertexAttrib2dvARB", &Return);
		glVertexAttrib3svARB=glGetProcAddress("glVertexAttrib3svARB", &Return);
		glVertexAttrib3fvARB=glGetProcAddress("glVertexAttrib3fvARB", &Return);
		glVertexAttrib3dvARB=glGetProcAddress("glVertexAttrib3dvARB", &Return);
		glVertexAttrib4svARB=glGetProcAddress("glVertexAttrib4svARB", &Return);
		glVertexAttrib4fvARB=glGetProcAddress("glVertexAttrib4fvARB", &Return);
		glVertexAttrib4dvARB=glGetProcAddress("glVertexAttrib4dvARB", &Return);
		glVertexAttrib4ivARB=glGetProcAddress("glVertexAttrib4ivARB", &Return);
		glVertexAttrib4bvARB=glGetProcAddress("glVertexAttrib4bvARB", &Return);
		glVertexAttrib4usvARB=glGetProcAddress("glVertexAttrib4usvARB", &Return);
		glVertexAttrib4ubvARB=glGetProcAddress("glVertexAttrib4ubvARB", &Return);
		glVertexAttrib4uivARB=glGetProcAddress("glVertexAttrib4uivARB", &Return);
		glVertexAttrib4NbvARB=glGetProcAddress("glVertexAttrib4NbvARB", &Return);
		glVertexAttrib4NsvARB=glGetProcAddress("glVertexAttrib4NsvARB", &Return);
		glVertexAttrib4NivARB=glGetProcAddress("glVertexAttrib4NivARB", &Return);
		glVertexAttrib4NubvARB=glGetProcAddress("glVertexAttrib4NubvARB", &Return);
		glVertexAttrib4NusvARB=glGetProcAddress("glVertexAttrib4NusvARB", &Return);
		glVertexAttrib4NuivARB=glGetProcAddress("glVertexAttrib4NuivARB", &Return);
		glVertexAttribPointerARB=glGetProcAddress("glVertexAttribPointerARB", &Return);
		glEnableVertexAttribArrayARB=glGetProcAddress("glEnableVertexAttribArrayARB", &Return);
		glDisableVertexAttribArrayARB=glGetProcAddress("glDisableVertexAttribArrayARB", &Return);
		glProgramStringARB=glGetProcAddress("glProgramStringARB", &Return);
		glBindProgramARB=glGetProcAddress("glBindProgramARB", &Return);
		glDeleteProgramsARB=glGetProcAddress("glDeleteProgramsARB", &Return);
		glGenProgramsARB=glGetProcAddress("glGenProgramsARB", &Return);
		glProgramEnvParameter4dARB=glGetProcAddress("glProgramEnvParameter4dARB", &Return);
		glProgramEnvParameter4dvARB=glGetProcAddress("glProgramEnvParameter4dvARB", &Return);
		glProgramEnvParameter4fARB=glGetProcAddress("glProgramEnvParameter4fARB", &Return);
		glProgramEnvParameter4fvARB=glGetProcAddress("glProgramEnvParameter4fvARB", &Return);
		glProgramLocalParameter4dARB=glGetProcAddress("glProgramLocalParameter4dARB", &Return);
		glProgramLocalParameter4dvARB=glGetProcAddress("glProgramLocalParameter4dvARB", &Return);
		glProgramLocalParameter4fARB=glGetProcAddress("glProgramLocalParameter4fARB", &Return);
		glProgramLocalParameter4fvARB=glGetProcAddress("glProgramLocalParameter4fvARB", &Return);
		glGetProgramEnvParameterdvARB=glGetProcAddress("glGetProgramEnvParameterdvARB", &Return);
		glGetProgramEnvParameterfvARB=glGetProcAddress("glGetProgramEnvParameterfvARB", &Return);
		glGetProgramLocalParameterdvARB=glGetProcAddress("glGetProgramLocalParameterdvARB", &Return);
		glGetProgramLocalParameterfvARB=glGetProcAddress("glGetProgramLocalParameterfvARB", &Return);
		glGetProgramivARB=glGetProcAddress("glGetProgramivARB", &Return);
		glGetProgramStringARB=glGetProcAddress("glGetProgramStringARB", &Return);
		glGetVertexAttribdvARB=glGetProcAddress("glGetVertexAttribdvARB", &Return);
		glGetVertexAttribfvARB=glGetProcAddress("glGetVertexAttribfvARB", &Return);
		glGetVertexAttribivARB=glGetProcAddress("glGetVertexAttribivARB", &Return);
		glGetVertexAttribPointervARB=glGetProcAddress("glGetVertexAttribPointervARB", &Return);
		glIsProgramARB=glGetProcAddress("glIsProgramARB", &Return);

		if(Return)
			GL_ARB_vertex_program_Flag=1;
	}

	if(FindSubString(GLExtensionString, "GL_ARB_fragment_program"))
	{
		Return=1;

		glProgramStringARB=glGetProcAddress("glProgramStringARB", &Return);
		glBindProgramARB=glGetProcAddress("glBindProgramARB", &Return);
		glDeleteProgramsARB=glGetProcAddress("glDeleteProgramsARB", &Return);
		glGenProgramsARB=glGetProcAddress("glGenProgramsARB", &Return);
		glProgramEnvParameter4dARB=glGetProcAddress("glProgramEnvParameter4dARB", &Return);
		glProgramEnvParameter4dvARB=glGetProcAddress("glProgramEnvParameter4dvARB", &Return);
		glProgramEnvParameter4fARB=glGetProcAddress("glProgramEnvParameter4fARB", &Return);
		glProgramEnvParameter4fvARB=glGetProcAddress("glProgramEnvParameter4fvARB", &Return);
		glProgramLocalParameter4dARB=glGetProcAddress("glProgramLocalParameter4dARB", &Return);
		glProgramLocalParameter4dvARB=glGetProcAddress("glProgramLocalParameter4dvARB", &Return);
		glProgramLocalParameter4fARB=glGetProcAddress("glProgramLocalParameter4fARB", &Return);
		glProgramLocalParameter4fvARB=glGetProcAddress("glProgramLocalParameter4fvARB", &Return);
		glGetProgramEnvParameterdvARB=glGetProcAddress("glGetProgramEnvParameterdvARB", &Return);
		glGetProgramEnvParameterfvARB=glGetProcAddress("glGetProgramEnvParameterfvARB", &Return);
		glGetProgramLocalParameterdvARB=glGetProcAddress("glGetProgramLocalParameterdvARB", &Return);
		glGetProgramLocalParameterfvARB=glGetProcAddress("glGetProgramLocalParameterfvARB", &Return);
		glGetProgramivARB=glGetProcAddress("glGetProgramivARB", &Return);
		glGetProgramStringARB=glGetProcAddress("glGetProgramStringARB", &Return);
		glIsProgramARB=glGetProcAddress("glIsProgramARB", &Return);

		if(Return)
			GL_ARB_fragment_program_Flag=1;
	}

	if(FindSubString(GLExtensionString, "GL_ARB_depth_texture"))
		GL_ARB_depth_texture_Flag=1;

	if(FindSubString(GLExtensionString, "GL_ARB_shadow"))
		GL_ARB_shadow_Flag=1;

	if(FindSubString(GLExtensionString, "GL_ARB_shadow_ambient"))
		GL_ARB_shadow_ambient_Flag=1;

	if(FindSubString(GLExtensionString, "GL_ARB_texture_cube_map"))
		GL_ARB_texture_cube_map_Flag=1;

	if(FindSubString(GLExtensionString, "GL_ARB_texture_non_power_of_two"))
		GL_ARB_texture_non_power_of_two_Flag=1;

	if(FindSubString(GLExtensionString, "GL_ARB_texture_env_add"))
		GL_ARB_texture_env_add_Flag=1;

	if(FindSubString(GLExtensionString, "GL_ARB_texture_env_combine"))
		GL_ARB_texture_env_combine_Flag=1;

	if(FindSubString(GLExtensionString, "GL_ARB_texture_env_crossbar"))
		GL_ARB_texture_env_crossbar_Flag=1;

	if(FindSubString(GLExtensionString, "GL_ARB_texture_env_dot3"))
		GL_ARB_texture_env_dot3_Flag=1;

	if(FindSubString(GLExtensionString, "GL_EXT_texture3D"))
	{
		Return=1;

		glTexImage3DEXT=glGetProcAddress("glTexImage3DEXT", &Return);
		glTexSubImage3DEXT=glGetProcAddress("glTexSubImage3DEXT", &Return);
		glCopyTexSubImage3DEXT=glGetProcAddress("glCopyTexSubImage3DEXT", &Return);

		if(Return)
			GL_EXT_texture3D_Flag=1;
	}

	if(FindSubString(GLExtensionString, "GL_ARB_vertex_buffer_object"))
	{
		Return=1;

		glBindBufferARB=glGetProcAddress("glBindBufferARB", &Return);
		glDeleteBuffersARB=glGetProcAddress("glDeleteBuffersARB", &Return);
		glGenBuffersARB=glGetProcAddress("glGenBuffersARB", &Return);
		glIsBufferARB=glGetProcAddress("glIsBufferARB", &Return);
		glBufferDataARB=glGetProcAddress("glBufferDataARB", &Return);
		glBufferSubDataARB=glGetProcAddress("glBufferSubDataARB", &Return);
		glGetBufferSubDataARB=glGetProcAddress("glGetBufferSubDataARB", &Return);
		glMapBufferARB=glGetProcAddress("glMapBufferARB", &Return);
		glUnmapBufferARB=glGetProcAddress("glUnmapBufferARB", &Return);
		glGetBufferParameterivARB=glGetProcAddress("glGetBufferParameterivARB", &Return);
		glGetBufferPointervARB=glGetProcAddress("glGetBufferPointervARB", &Return);

		if(Return)
			GL_ARB_vertex_buffer_object_Flag=1;
	}

	if(FindSubString(GLExtensionString, "GL_ARB_pixel_buffer_object"))
		GL_ARB_pixel_buffer_object_Flag=1;

	if(FindSubString(GLExtensionString, "GL_EXT_framebuffer_object"))
	{
		Return=1;

		glIsRenderbufferEXT=glGetProcAddress("glIsRenderbufferEXT", &Return);
		glBindRenderbufferEXT=glGetProcAddress("glBindRenderbufferEXT", &Return);
		glDeleteRenderbuffersEXT=glGetProcAddress("glDeleteRenderbuffersEXT", &Return);
		glGenRenderbuffersEXT=glGetProcAddress("glGenRenderbuffersEXT", &Return);
		glRenderbufferStorageEXT=glGetProcAddress("glRenderbufferStorageEXT", &Return);
		glGetRenderbufferParameterivEXT=glGetProcAddress("glGetRenderbufferParameterivEXT", &Return);
		glIsFramebufferEXT=glGetProcAddress("glIsFramebufferEXT", &Return);
		glBindFramebufferEXT=glGetProcAddress("glBindFramebufferEXT", &Return);
		glDeleteFramebuffersEXT=glGetProcAddress("glDeleteFramebuffersEXT", &Return);
		glGenFramebuffersEXT=glGetProcAddress("glGenFramebuffersEXT", &Return);
		glCheckFramebufferStatusEXT=glGetProcAddress("glCheckFramebufferStatusEXT", &Return);
		glFramebufferTexture1DEXT=glGetProcAddress("glFramebufferTexture1DEXT", &Return);
		glFramebufferTexture2DEXT=glGetProcAddress("glFramebufferTexture2DEXT", &Return);
		glFramebufferTexture3DEXT=glGetProcAddress("glFramebufferTexture3DEXT", &Return);
		glFramebufferRenderbufferEXT=glGetProcAddress("glFramebufferRenderbufferEXT", &Return);
		glGetFramebufferAttachmentParameterivEXT=glGetProcAddress("glGetFramebufferAttachmentParameterivEXT", &Return);
		glGenerateMipmapEXT=glGetProcAddress("glGenerateMipmapEXT", &Return);

		if(Return)
			GL_EXT_framebuffer_object_Flag=1;
	}


	if(FindSubString(GLExtensionString, "GL_ARB_point_parameters"))
	{
		Return=1;

		glPointParameterfARB=glGetProcAddress("glPointParameterfARB", &Return);
		glPointParameterfvARB=glGetProcAddress("glPointParameterfvARB", &Return);

		if(Return)
			GL_ARB_point_parameters_Flag=1;
	}

	if(FindSubString(GLExtensionString, "GL_ARB_point_sprite"))
		GL_ARB_point_sprite_Flag=1;

	if(FindSubString(GLExtensionString, "GL_EXT_abgr"))
		GL_EXT_abgr_Flag=1;

	if(FindSubString(GLExtensionString, "GL_EXT_bgra"))
		GL_EXT_bgra_Flag=1;

	if(FindSubString(GLExtensionString, "GL_EXT_packed_pixels"))
		GL_EXT_packed_pixels_Flag=1;

	if(FindSubString(GLExtensionString, "GL_EXT_texture_compression_s3tc"))
		GL_EXT_texture_compression_s3tc_Flag=1;

	if(FindSubString(GLExtensionString, "GL_EXT_texture_edge_clamp"))
		GL_EXT_texture_edge_clamp_Flag=1;

	if(FindSubString(GLExtensionString, "GL_EXT_texture_env_combine"))
		GL_EXT_texture_env_combine_Flag=1;

	if(FindSubString(GLExtensionString, "GL_EXT_texture_filter_anisotropic"))
		GL_EXT_texture_filter_anisotropic_Flag=1;

	if(FindSubString(GLExtensionString, "GL_EXT_stencil_wrap"))
		GL_EXT_stencil_wrap_Flag=1;

	if(FindSubString(GLExtensionString, "GL_NV_texture_rectangle")||FindSubString(GLExtensionString, "GL_EXT_texture_rectangle"))
		GL_EXT_texture_rectangle_Flag=1;

	if(FindSubString(GLExtensionString, "GL_EXT_texture_rectangle"))
		GL_EXT_texture_rectangle_Flag=1;

	if(FindSubString(GLExtensionString, "GL_ATI_fragment_shader"))
	{
		Return=1;

		glGenFragmentShadersATI=glGetProcAddress("glGenFragmentShadersATI", &Return);
		glBindFragmentShaderATI=glGetProcAddress("glBindFragmentShaderATI", &Return);
		glDeleteFragmentShaderATI=glGetProcAddress("glDeleteFragmentShaderATI", &Return);
		glBeginFragmentShaderATI=glGetProcAddress("glBeginFragmentShaderATI", &Return);
		glEndFragmentShaderATI=glGetProcAddress("glEndFragmentShaderATI", &Return);
		glPassTexCoordATI=glGetProcAddress("glPassTexCoordATI", &Return);
		glSampleMapATI=glGetProcAddress("glSampleMapATI", &Return);
		glColorFragmentOp1ATI=glGetProcAddress("glColorFragmentOp1ATI", &Return);
		glColorFragmentOp2ATI=glGetProcAddress("glColorFragmentOp2ATI", &Return);
		glColorFragmentOp3ATI=glGetProcAddress("glColorFragmentOp3ATI", &Return);
		glAlphaFragmentOp1ATI=glGetProcAddress("glAlphaFragmentOp1ATI", &Return);
		glAlphaFragmentOp2ATI=glGetProcAddress("glAlphaFragmentOp2ATI", &Return);
		glAlphaFragmentOp3ATI=glGetProcAddress("glAlphaFragmentOp3ATI", &Return);
		glSetFragmentShaderConstantATI=glGetProcAddress("glSetFragmentShaderConstantATI", &Return);

		if(Return)
			GL_ATI_fragment_shader_Flag=1;
	}

	if(FindSubString(GLExtensionString, "GL_ATI_separate_stencil"))
	{
		Return=1;

		glStencilOpSeparateATI=glGetProcAddress("glStencilOpSeparateATI", &Return);
		glStencilFuncSeparateATI=glGetProcAddress("glStencilFuncSeparateATI", &Return);

		if(Return)
			GL_ATI_separate_stencil_Flag=1;
	}

	if(FindSubString(GLExtensionString, "GL_ATI_draw_buffers"))
	{
		Return=1;

		glDrawBuffersATI=glGetProcAddress("glDrawBuffersATI", &Return);

		if(Return)
			GL_ATI_draw_buffers_Flag=1;
	}

	if(FindSubString(GLExtensionString, "GL_ATI_texture_float"))
		GL_ATI_texture_float_Flag=1;

	if(FindSubString(GLExtensionString, "GL_HP_occlusion_test"))
		GL_HP_occlusion_test_Flag=1;

	if(FindSubString(GLExtensionString, "GL_ARB_occlusion_query"))
	{
		Return=1;

		glGenQueriesARB=glGetProcAddress("glGenQueriesARB", &Return);
		glDeleteQueriesARB=glGetProcAddress("glDeleteQueriesARB", &Return);
		glIsQueryARB=glGetProcAddress("glIsQueryARB", &Return);
		glBeginQueryARB=glGetProcAddress("glBeginQueryARB", &Return);
		glEndQueryARB=glGetProcAddress("glEndQueryARB", &Return);
		glGetQueryivARB=glGetProcAddress("glGetQueryivARB", &Return);
		glGetQueryObjectivARB=glGetProcAddress("glGetQueryObjectivARB", &Return);
		glGetQueryObjectuivARB=glGetProcAddress("glGetQueryObjectuivARB", &Return);

		if(Return)
			GL_ARB_occlusion_query_Flag=1;
	}

	if(FindSubString(GLExtensionString, "GL_SGIS_generate_mipmap"))
		GL_SGIS_generate_mipmap_Flag=1;

	if(FindSubString(GLExtensionString, "GL_ARB_shader_objects"))
	{
		Return=1;

		glDeleteObjectARB=glGetProcAddress("glDeleteObjectARB", &Return);
		glGetHandleARB=glGetProcAddress("glGetHandleARB", &Return);
		glDetachObjectARB=glGetProcAddress("glDetachObjectARB", &Return);
		glCreateShaderObjectARB=glGetProcAddress("glCreateShaderObjectARB", &Return);
		glShaderSourceARB=glGetProcAddress("glShaderSourceARB", &Return);
		glCompileShaderARB=glGetProcAddress("glCompileShaderARB", &Return);
		glCreateProgramObjectARB=glGetProcAddress("glCreateProgramObjectARB", &Return);
		glAttachObjectARB=glGetProcAddress("glAttachObjectARB", &Return);
		glLinkProgramARB=glGetProcAddress("glLinkProgramARB", &Return);
		glUseProgramObjectARB=glGetProcAddress("glUseProgramObjectARB", &Return);
		glValidateProgramARB=glGetProcAddress("glValidateProgramARB", &Return);
		glUniform1fARB=glGetProcAddress("glUniform1fARB", &Return);
		glUniform2fARB=glGetProcAddress("glUniform2fARB", &Return);
		glUniform3fARB=glGetProcAddress("glUniform3fARB", &Return);
		glUniform4fARB=glGetProcAddress("glUniform4fARB", &Return);
		glUniform1iARB=glGetProcAddress("glUniform1iARB", &Return);
		glUniform2iARB=glGetProcAddress("glUniform2iARB", &Return);
		glUniform3iARB=glGetProcAddress("glUniform3iARB", &Return);
		glUniform4iARB=glGetProcAddress("glUniform4iARB", &Return);
		glUniform1fvARB=glGetProcAddress("glUniform1fvARB", &Return);
		glUniform2fvARB=glGetProcAddress("glUniform2fvARB", &Return);
		glUniform3fvARB=glGetProcAddress("glUniform3fvARB", &Return);
		glUniform4fvARB=glGetProcAddress("glUniform4fvARB", &Return);
		glUniform1ivARB=glGetProcAddress("glUniform1ivARB", &Return);
		glUniform2ivARB=glGetProcAddress("glUniform2ivARB", &Return);
		glUniform3ivARB=glGetProcAddress("glUniform3ivARB", &Return);
		glUniform4ivARB=glGetProcAddress("glUniform4ivARB", &Return);
		glUniformMatrix2fvARB=glGetProcAddress("glUniformMatrix2fvARB", &Return);
		glUniformMatrix3fvARB=glGetProcAddress("glUniformMatrix3fvARB", &Return);
		glUniformMatrix4fvARB=glGetProcAddress("glUniformMatrix4fvARB", &Return);
		glGetObjectParameterfvARB=glGetProcAddress("glGetObjectParameterfvARB", &Return);
		glGetObjectParameterivARB=glGetProcAddress("glGetObjectParameterivARB", &Return);
		glGetInfoLogARB=glGetProcAddress("glGetInfoLogARB", &Return);
		glGetAttachedObjectsARB=glGetProcAddress("glGetAttachedObjectsARB", &Return);
		glGetUniformLocationARB=glGetProcAddress("glGetUniformLocationARB", &Return);
		glGetActiveUniformARB=glGetProcAddress("glGetActiveUniformARB", &Return);
		glGetUniformfvARB=glGetProcAddress("glGetUniformfvARB", &Return);
		glGetUniformivARB=glGetProcAddress("glGetUniformivARB", &Return);
		glGetShaderSourceARB=glGetProcAddress("glGetShaderSourceARB", &Return);

		if(Return)
			GL_ARB_shader_objects_Flag=1;
	}

	if(FindSubString(GLExtensionString, "GL_ARB_vertex_shader"))
	{
		Return=1;

		glBindAttribLocationARB=glGetProcAddress("glBindAttribLocationARB", &Return);
		glGetActiveAttribARB=glGetProcAddress("glGetActiveAttribARB", &Return);
		glGetAttribLocationARB=glGetProcAddress("glGetAttribLocationARB", &Return);

		if(Return)
			GL_ARB_vertex_shader_Flag=1;
	}

	if(FindSubString(GLExtensionString, "GL_ARB_fragment_shader"))
		GL_ARB_fragment_shader_Flag=1;

	GLExtensionsInitialized_Flag=1;
}

#ifdef WIN32
void WGLSetupExtensions(void)
{
	int Return=1;
	char *WGLExtensionString=NULL;

	wglGetExtensionsStringARB=glGetProcAddress("wglGetExtensionsStringARB", &Return);

	if(Return)
		WGLExtensionString=(char *)wglGetExtensionsStringARB(wglGetCurrentDC());

	if(FindSubString(WGLExtensionString, "WGL_EXT_swap_control"))
	{
		Return=1;

		wglSwapIntervalEXT=glGetProcAddress("wglSwapIntervalEXT", &Return);
		wglGetSwapIntervalEXT=glGetProcAddress("wglGetSwapIntervalEXT", &Return);

		if(Return)
			WGL_EXT_swap_control_Flag=1;
	}

	if(FindSubString(WGLExtensionString, "WGL_ARB_pixel_format"))
	{
		Return=1;

		wglGetPixelFormatAttribivARB=glGetProcAddress("wglGetPixelFormatAttribivARB", &Return);
		wglGetPixelFormatAttribfvARB=glGetProcAddress("wglGetPixelFormatAttribfvARB", &Return);
		wglChoosePixelFormatARB=glGetProcAddress("wglChoosePixelFormatARB", &Return);

		if(Return)
			WGL_ARB_pixel_format_Flag=1;
	}

	if(FindSubString(WGLExtensionString,"WGL_ARB_make_current_read"))
	{
		Return=1;

		wglMakeContextCurrentARB=glGetProcAddress("wglMakeContextCurrentARB", &Return);
		wglGetCurrentReadDCARB=glGetProcAddress("wglGetCurrentReadDCARB", &Return);

		if(Return)
			WGL_ARB_make_current_read_Flag=1;
	}

	if(FindSubString(WGLExtensionString, "WGL_ARB_pbuffer"))
	{
		Return=1;

		wglCreatePbufferARB=glGetProcAddress("wglCreatePbufferARB", &Return);
		wglGetPbufferDCARB=glGetProcAddress("wglGetPbufferDCARB", &Return);
		wglReleasePbufferDCARB=glGetProcAddress("wglReleasePbufferDCARB", &Return);
		wglDestroyPbufferARB=glGetProcAddress("wglDestroyPbufferARB", &Return);
		wglQueryPbufferARB=glGetProcAddress("wglQueryPbufferARB", &Return);

		if(Return)
			WGL_ARB_pbuffer_Flag=1;
	}

	if(FindSubString(WGLExtensionString, "WGL_ARB_render_texture"))
	{
		Return=1;

		wglBindTexImageARB=glGetProcAddress("wglBindTexImageARB", &Return);
		wglReleaseTexImageARB=glGetProcAddress("wglReleaseTexImageARB", &Return);
		wglSetPbufferAttribARB=glGetProcAddress("wglSetPbufferAttribARB", &Return);

		if(Return)
			WGL_ARB_render_texture_Flag=1;
	}

	if(FindSubString(WGLExtensionString, "WGL_ARB_multisample"))
		WGL_ARB_multisample_Flag=1;

	if(FindSubString(WGLExtensionString, "WGL_ATI_pixel_format_float"))
		WGL_ATI_pixel_format_float_Flag=1;

	if(FindSubString(WGLExtensionString, "WGL_NV_float_buffer"))
		WGL_NV_float_buffer_Flag=1;

	if(FindSubString(WGLExtensionString, "WGL_NV_render_texture_rectangle"))
		WGL_NV_render_texture_rectangle_Flag=1;

	if(FindSubString(WGLExtensionString, "WGL_ATI_render_texture_rectangle"))
		WGL_ATI_render_texture_rectangle_Flag=1;

	WGLExtensionsInitialized_Flag=1;
}
#else
void GLXSetupExtensions(Display *dpy)
{
	int Return=1;
	char *GLXExtensionString=NULL;

	GLXExtensionString=(char *)glXQueryExtensionsString(dpy, DefaultScreen(dpy));

	if(FindSubString(GLXExtensionString, "GLX_ARB_multisample"))
		GLX_ARB_multisample_Flag=1;

	if(FindSubString(GLXExtensionString, "GLX_ATI_render_texture"))
	{
		Return=1;

		glXBindTexImageATI=glGetProcAddress("glXBindTexImageATI", &Return);
		glXReleaseTexImageATI=glGetProcAddress("glXReleaseTexImageATI", &Return);
		glXDrawableAttribATI=glGetProcAddress("glXDrawableAttribATI", &Return);

		if(Return)
			GLX_ATI_render_texture_Flag=1;
	}

	if(FindSubString(GLXExtensionString, "GLX_ATI_pixel_format_float"))
		GLX_ATI_pixel_format_float_Flag=1;

	if(FindSubString(GLXExtensionString, "GLX_NV_float_buffer"))
		GLX_NV_float_buffer_Flag=1;

	GLXExtensionsInitialized_Flag=1;
}
#endif

int CreateContext(GLContext_t *Context, int Color, int Depth, int Stencil, int Accum, unsigned long Flags)
{
#ifdef WIN32
	int PixelFormat;
	unsigned int NumFormats;
	int Attrib[128], i=0;

	if(!(GLExtensionsInitialized_Flag||WGLExtensionsInitialized_Flag))
	{
		PIXELFORMATDESCRIPTOR pfd;
		HWND hWndTemp=NULL;
		HDC hDCTemp=NULL;
		HGLRC hRCTemp=NULL;

		memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
		pfd.nSize=sizeof(PIXELFORMATDESCRIPTOR);
		pfd.nVersion=1;
		pfd.dwFlags=PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL;
		pfd.iPixelType=PFD_TYPE_RGBA;
		pfd.cColorBits=16;
		pfd.cDepthBits=16;
		pfd.iLayerType=PFD_MAIN_PLANE;
		
		hWndTemp=CreateWindow("static", NULL, WS_POPUP, 0, 0, 1, 1, NULL, NULL, (HINSTANCE)GetModuleHandle(NULL), NULL);
		hDCTemp=GetDC(hWndTemp);

		if(!(PixelFormat=ChoosePixelFormat(hDCTemp, &pfd)))
		{
			MessageBox(Context->hWnd, "ChoosePixelFormat Failed!", NULL, MB_OK);
			return 0;
		}

		if(!SetPixelFormat(hDCTemp, PixelFormat, NULL))
		{
			MessageBox(Context->hWnd, "SetPixelFormat Failed!", NULL, MB_OK);
			return 0;
		}

		if(!(hRCTemp=wglCreateContext(hDCTemp)))
		{
			MessageBox(Context->hWnd, "wglCreateContext Failed!", NULL, MB_OK);
			return 0;
		}


		if(!wglMakeCurrent(hDCTemp, hRCTemp))
		{
			MessageBox(Context->hWnd, "wglMakeCurrent Failed!", NULL, MB_OK);
			return 0;
		}

		if(!GLExtensionsInitialized_Flag)
			GLSetupExtensions();

		if(!WGLExtensionsInitialized_Flag)
			WGLSetupExtensions();

		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(hRCTemp);
		ReleaseDC(hWndTemp, hDCTemp);
		DestroyWindow(hWndTemp);
	}

	if(!WGL_ARB_pixel_format_Flag)
		return 0;

	Context->hDC=GetDC(Context->hWnd);

	Attrib[i++]=WGL_DRAW_TO_WINDOW_ARB;
	Attrib[i++]=TRUE;
	Attrib[i++]=WGL_SUPPORT_OPENGL_ARB;
	Attrib[i++]=TRUE;

	if(Flags&OGL_SINGLEBUFFER)
	{
		Attrib[i++]=WGL_DOUBLE_BUFFER_ARB;
		Attrib[i++]=FALSE;
	}

	if(Flags&OGL_DOUBLEBUFFER)
	{
		Attrib[i++]=WGL_DOUBLE_BUFFER_ARB;
		Attrib[i++]=TRUE;
	}

	Attrib[i++]=WGL_PIXEL_TYPE_ARB;
	Attrib[i++]=WGL_TYPE_RGBA_ARB;

	Attrib[i++]=WGL_COLOR_BITS_ARB;
	Attrib[i++]=Color;
	Attrib[i++]=WGL_ACCUM_BITS_ARB;
	Attrib[i++]=Accum;
	Attrib[i++]=WGL_DEPTH_BITS_ARB;
	Attrib[i++]=Depth;
	Attrib[i++]=WGL_STENCIL_BITS_ARB;
	Attrib[i++]=Stencil;

	if(Flags&OGL_AUXBUFFERS1)
	{
		Attrib[i++]=WGL_AUX_BUFFERS_ARB;
		Attrib[i++]=1;
	}

	if(Flags&OGL_AUXBUFFERS2)
	{
		Attrib[i++]=WGL_AUX_BUFFERS_ARB;
		Attrib[i++]=2;
	}

	if(Flags&OGL_AUXBUFFERS3)
	{
		Attrib[i++]=WGL_AUX_BUFFERS_ARB;
		Attrib[i++]=3;
	}

	if(Flags&OGL_AUXBUFFERS4)
	{
		Attrib[i++]=WGL_AUX_BUFFERS_ARB;
		Attrib[i++]=4;
	}

	if(WGL_ARB_multisample_Flag)
	{
		if(Flags&OGL_SAMPLES2X)
		{
			Attrib[i++]=WGL_SAMPLE_BUFFERS_ARB;
			Attrib[i++]=TRUE;
			Attrib[i++]=WGL_SAMPLES_ARB;
			Attrib[i++]=2;
		}

		if(Flags&OGL_SAMPLES4X)
		{
			Attrib[i++]=WGL_SAMPLE_BUFFERS_ARB;
			Attrib[i++]=TRUE;
			Attrib[i++]=WGL_SAMPLES_ARB;
			Attrib[i++]=4;
		}

		if(Flags&OGL_SAMPLES6X)
		{
			Attrib[i++]=WGL_SAMPLE_BUFFERS_ARB;
			Attrib[i++]=TRUE;
			Attrib[i++]=WGL_SAMPLES_ARB;
			Attrib[i++]=6;
		}

		if(Flags&OGL_SAMPLES8X)
		{
			Attrib[i++]=WGL_SAMPLE_BUFFERS_ARB;
			Attrib[i++]=TRUE;
			Attrib[i++]=WGL_SAMPLES_ARB;
			Attrib[i++]=8;
		}
	}

	Attrib[i++]=0;

	if(!wglChoosePixelFormatARB(Context->hDC, Attrib, NULL, 1, &PixelFormat, &NumFormats))
	{
		MessageBox(Context->hWnd, "wglChoosePixelFormatARB failed!", NULL, MB_OK);
		return 0;
	}

	if(!SetPixelFormat(Context->hDC, PixelFormat, NULL))
	{
		MessageBox(Context->hWnd, "SetPixelFormat Failed!", NULL, MB_OK);
		return 0;
	}

	if(!(Context->hRC=wglCreateContext(Context->hDC)))
	{
		MessageBox(Context->hWnd, "wglCreateContext Failed!", NULL, MB_OK);
		return 0;
	}

	if(!wglMakeCurrent(Context->hDC, Context->hRC))
	{
		MessageBox(Context->hWnd, "wglMakeCurrent Failed!", NULL, MB_OK);
		return 0;
	}

	return 1;
#else
	int i, Attrib[128];
	GLXFBConfig *fbconfig=NULL;

	if(!GLXExtensionsInitialized_Flag)
		GLXSetupExtensions(dpy);

	i=0;
	Attrib[i++]=GLX_DRAWABLE_TYPE;
	Attrib[i++]=GLX_WINDOW_BIT;
	Attrib[i++]=GLX_RENDER_TYPE;
	Attrib[i++]=GLX_RGBA_BIT;
	Attrib[i++]=GLX_DOUBLEBUFFER;
	Attrib[i++]=DoubleBuffer;
	Attrib[i++]=GLX_BUFFER_SIZE;
	Attrib[i++]=Color;
	Attrib[i++]=GLX_DEPTH_SIZE;
	Attrib[i++]=Depth;
	Attrib[i++]=GLX_STENCIL_SIZE;
	Attrib[i++]=Stencil;

	if(GLX_ARB_multisample_Flag&&SampleBuffers)
	{
		Attrib[i++]=GLX_SAMPLE_BUFFERS_ARB;
		Attrib[i++]=1;
		Attrib[i++]=GLX_SAMPLES_ARB;
		Attrib[i++]=SampleBuffers;
	}

	Attrib[i++]=None;

	if((fbconfig=glXChooseFBConfig(dpy, DefaultScreen(dpy), Attrib, &i))==NULL)
	{
		fprintf(stderr, "Error: glXChooseFBConfig failed\n");
		return 0;
	}

	if((Context->ctx=glXCreateNewContext(dpy, fbconfig[0], GLX_RGBA, NULL, GL_TRUE))==NULL)
	{
		fprintf(stderr, "Error: glXCreateContext failed\n");
		return 0;
	}

	glXGetFBConfigAttrib(dpy, fbconfig[0], GLX_VISUAL_ID, &Context->visualid);

	if(!GLExtensionsInitialized_Flag)
	{
		XVisualInfo *visinfo=glXGetVisualFromFBConfig(dpy, fbconfig[0]);
		Window win=XCreateWindow(dpy, RootWindow(dpy, DefaultScreen(dpy)), 0, 0, 1, 1, 0, 0, 0, 0, 0, 0);

		glXMakeCurrent(dpy, win, Context->ctx);
		GLSetupExtensions();
		glXMakeCurrent(dpy, 0, NULL);

		XDestroyWindow(dpy, win);
		XFree(visinfo);
	}

	return 1;
#endif
}

int CreatePbuffer(GLContext_t *Pbuffer, int Color, int Depth, int Stencil, int Accum, unsigned long Flags, int Width, int Height)
{
#ifdef WIN32
	HDC hDCTemp=NULL;
	int Attrib[128], i=0, PixelFormat=-1;
	unsigned int Formats;

	if(!(WGL_ARB_pixel_format_Flag&&WGL_ARB_pbuffer_Flag))
		return 0;

	hDCTemp=wglGetCurrentDC();

	Attrib[i++]=WGL_DRAW_TO_PBUFFER_ARB;
	Attrib[i++]=TRUE;
	Attrib[i++]=WGL_SUPPORT_OPENGL_ARB;
	Attrib[i++]=TRUE;

	if(Flags&OGL_SINGLEBUFFER)
	{
		Attrib[i++]=WGL_DOUBLE_BUFFER_ARB;
		Attrib[i++]=FALSE;
	}

	if(Flags&OGL_DOUBLEBUFFER)
	{
		Attrib[i++]=WGL_DOUBLE_BUFFER_ARB;
		Attrib[i++]=TRUE;
	}

	if(Flags&OGL_FLOATBUFFER)
	{
		/*ATI float pixel format*/
		if(WGL_ATI_pixel_format_float_Flag)
		{
			Attrib[i++]=WGL_PIXEL_TYPE_ARB;
			Attrib[i++]=WGL_TYPE_RGBA_FLOAT_ATI;
		}
		else
		{
			/*NVIDIA float pixel format*/
			if(WGL_NV_float_buffer_Flag)
			{
				Attrib[i++]=WGL_PIXEL_TYPE_ARB;
				Attrib[i++]=WGL_TYPE_RGBA_ARB;
				Attrib[i++]=WGL_FLOAT_COMPONENTS_NV;
				Attrib[i++]=1;
			}
			/*Fail*/
			else
				return 0;
		}
	}
	else
	{
		Attrib[i++]=WGL_PIXEL_TYPE_ARB;
		Attrib[i++]=WGL_TYPE_RGBA_ARB;
	}

	Attrib[i++]=WGL_COLOR_BITS_ARB;
	Attrib[i++]=Color;
	Attrib[i++]=WGL_ACCUM_BITS_ARB;
	Attrib[i++]=Accum;
	Attrib[i++]=WGL_DEPTH_BITS_ARB;
	Attrib[i++]=Depth;
	Attrib[i++]=WGL_STENCIL_BITS_ARB;
	Attrib[i++]=Stencil;

	if(Flags&OGL_AUXBUFFERS1)
	{
		Attrib[i++]=WGL_AUX_BUFFERS_ARB;
		Attrib[i++]=1;
	}

	if(Flags&OGL_AUXBUFFERS2)
	{
		Attrib[i++]=WGL_AUX_BUFFERS_ARB;
		Attrib[i++]=2;
	}

	if(Flags&OGL_AUXBUFFERS3)
	{
		Attrib[i++]=WGL_AUX_BUFFERS_ARB;
		Attrib[i++]=3;
	}

	if(Flags&OGL_AUXBUFFERS4)
	{
		Attrib[i++]=WGL_AUX_BUFFERS_ARB;
		Attrib[i++]=4;
	}

	if(WGL_ARB_multisample_Flag)
	{
		if(Flags&OGL_SAMPLES2X)
		{
			Attrib[i++]=WGL_SAMPLE_BUFFERS_ARB;
			Attrib[i++]=TRUE;
			Attrib[i++]=WGL_SAMPLES_ARB;
			Attrib[i++]=2;
		}

		if(Flags&OGL_SAMPLES4X)
		{
			Attrib[i++]=WGL_SAMPLE_BUFFERS_ARB;
			Attrib[i++]=TRUE;
			Attrib[i++]=WGL_SAMPLES_ARB;
			Attrib[i++]=4;
		}

		if(Flags&OGL_SAMPLES6X)
		{
			Attrib[i++]=WGL_SAMPLE_BUFFERS_ARB;
			Attrib[i++]=TRUE;
			Attrib[i++]=WGL_SAMPLES_ARB;
			Attrib[i++]=6;
		}

		if(Flags&OGL_SAMPLES8X)
		{
			Attrib[i++]=WGL_SAMPLE_BUFFERS_ARB;
			Attrib[i++]=TRUE;
			Attrib[i++]=WGL_SAMPLES_ARB;
			Attrib[i++]=8;
		}
	}

	if(WGL_ARB_render_texture_Flag)
	{
		if((Flags&OGL_RENDER_TEXTURE2D)||(Flags&OGL_RENDER_TEXTURECUBE))
		{
			Attrib[i++]=WGL_BIND_TO_TEXTURE_RGBA_ARB;
			Attrib[i++]=TRUE;
		}

		if(Flags&OGL_RENDER_TEXTURERECTANGLE)
		{
			if(WGL_ATI_render_texture_rectangle_Flag)
			{
			}
			else
			{
				if(WGL_NV_render_texture_rectangle_Flag)
				{
					Attrib[i++]=WGL_BIND_TO_TEXTURE_RECTANGLE_RGBA_NV;
					Attrib[i++]=TRUE;
				}
			}
		}
	}

	Attrib[i++]=0;

	if(!wglChoosePixelFormatARB(hDCTemp, Attrib, NULL, 1, &PixelFormat, &Formats))
	{
		MessageBox(NULL, "wglChoosePixelFormatARB Failed!", NULL, MB_OK);
		return 0;
	}

	i=0;

	if(WGL_ARB_render_texture_Flag)
	{
		if(Flags&OGL_RENDER_TEXTURE2D)
		{
			Attrib[i++]=WGL_TEXTURE_FORMAT_ARB;
			Attrib[i++]=WGL_TEXTURE_RGBA_ARB;
			Attrib[i++]=WGL_TEXTURE_TARGET_ARB;
			Attrib[i++]=WGL_TEXTURE_2D_ARB;
		}

		if(Flags&OGL_RENDER_TEXTURECUBE)
		{
			Attrib[i++]=WGL_TEXTURE_FORMAT_ARB;
			Attrib[i++]=WGL_TEXTURE_RGBA_ARB;
			Attrib[i++]=WGL_TEXTURE_TARGET_ARB;
			Attrib[i++]=WGL_TEXTURE_CUBE_MAP_ARB;
		}

		if(Flags&OGL_RENDER_TEXTURERECTANGLE)
		{
			Attrib[i++]=WGL_TEXTURE_FORMAT_ARB;
			Attrib[i++]=WGL_TEXTURE_RGBA_ARB;
			Attrib[i++]=WGL_TEXTURE_TARGET_ARB;

			if(WGL_ATI_render_texture_rectangle_Flag)
			{}
			else
			{
				if(WGL_NV_render_texture_rectangle_Flag)
					Attrib[i++]=WGL_TEXTURE_RECTANGLE_NV;
			}
		}
	}

	Attrib[i++]=0;

	if(!(Pbuffer->hPbuffer=wglCreatePbufferARB(hDCTemp, PixelFormat, Width, Height, Attrib)))
	{
		MessageBox(NULL, "wglCreatePbufferARB Failed!", NULL, MB_OK);
		return 0;
	}

	if(!(Pbuffer->hDC=wglGetPbufferDCARB(Pbuffer->hPbuffer)))
	{
		MessageBox(NULL, "wglGetPbufferDCARB Failed!", NULL, MB_OK);
		return 0;
	}

	if(!(Pbuffer->hRC=wglCreateContext(Pbuffer->hDC)))
	{
		MessageBox(NULL, "wglCreateContext Failed!", NULL, MB_OK);
		return 0;
	}

	wglShareLists(wglGetCurrentContext(), Pbuffer->hRC);

	return 1;
#else
	GLXFBConfig *fbconfig;
	int i, Attrib[128];

	i=0;
	Attrib[i++]=GLX_DRAWABLE_TYPE;
	Attrib[i++]=GLX_PBUFFER_BIT|GLX_WINDOW_BIT;

	if(FloatBuffer)
	{
		/*ATI float pixel format*/
		if(GLX_ATI_pixel_format_float_Flag)
		{
			Attrib[i++]=GLX_RENDER_TYPE;
			Attrib[i++]=GLX_RGBA_FLOAT_ATI_BIT;
		}
		else
		{
			/*NVIDIA float pixel format*/
			if(GLX_NV_float_buffer_Flag)
			{
				Attrib[i++]=GLX_RENDER_TYPE;
				Attrib[i++]=GLX_RGBA_BIT;
				Attrib[i++]=GLX_FLOAT_COMPONENTS_NV;
				Attrib[i++]=1;
			}
			/*Fixed point fallback*/
			else
			{
				Attrib[i++]=GLX_RENDER_TYPE;
				Attrib[i++]=GLX_RGBA_BIT;
			}
		}
	}
	else
	{
		Attrib[i++]=GLX_RENDER_TYPE;
		Attrib[i++]=GLX_RGBA_BIT;
	}

	Attrib[i++]=GLX_DOUBLEBUFFER;
	Attrib[i++]=DoubleBuffer;
	Attrib[i++]=GLX_BUFFER_SIZE;
	Attrib[i++]=Color;
	Attrib[i++]=GLX_DEPTH_SIZE;
	Attrib[i++]=Depth;
	Attrib[i++]=GLX_STENCIL_SIZE;
	Attrib[i++]=Stencil;
	Attrib[i++]=GLX_AUX_BUFFERS;
	Attrib[i++]=AuxBuffers;

	if(GLX_ARB_multisample_Flag&&SampleBuffers)
	{
		Attrib[i++]=GLX_SAMPLE_BUFFERS_ARB;
		Attrib[i++]=1;
		Attrib[i++]=GLX_SAMPLES_ARB;
		Attrib[i++]=SampleBuffers;
	}

	Attrib[i++]=None;

	if(!(fbconfig=glXChooseFBConfig(dpy, DefaultScreen(dpy), Attrib, &i)))
	{
		fprintf(stderr,"Error: glXChooseFBConfig failed\n");
		return 0;
	}

	i=0;
	Attrib[i++]=GLX_PBUFFER_WIDTH;
	Attrib[i++]=Width;
	Attrib[i++]=GLX_PBUFFER_HEIGHT;
	Attrib[i++]=Height;
	Attrib[i++]=None;

	if(!(Pbuffer->pbuf=glXCreatePbuffer(dpy, fbconfig[0], Attrib)))
	{
		fprintf(stderr, "Error: glXCreatePbuffer failed\n");
		return 0;
	}

	if(!(Pbuffer->ctx=glXCreateNewContext(dpy, fbconfig[0], GLX_RGBA, glXGetCurrentContext(), GL_TRUE)))
	{
		fprintf(stderr, "Error: glXCreateContext failed\n");
		return 0;
	}

	glXGetFBConfigAttrib(dpy, fbconfig[0], GLX_VISUAL_ID, &Pbuffer->visualid);

	return 1;
#endif
}

void DestroyContext(GLContext_t *Context)
{
#ifdef WIN32
	if(Context->hPbuffer)
	{
		wglDeleteContext(Context->hRC);
		wglReleasePbufferDCARB(Context->hPbuffer, Context->hDC);
		wglDestroyPbufferARB(Context->hPbuffer);
	}
	else
	{
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(Context->hRC);
		ReleaseDC(Context->hWnd, Context->hDC);
	}
#else
	if(Context->pbuf)
	{
		glXDestroyContext(dpy, Context->ctx);
		glXDestroyPbuffer(dpy, Context->pbuf);
	}
	else
		glXDestroyContext(dpy, Context->ctx);
#endif

	memset(Context, 0, sizeof(GLContext_t));
}
