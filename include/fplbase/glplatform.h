// Copyright 2014 Google Inc. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// clang-format off

// OpenGL platform definitions

#ifndef FPLBASE_GLPLATFORM_H
#define FPLBASE_GLPLATFORM_H

#include "fplbase/config.h"  // Must come first.

#ifdef __APPLE__
#include "TargetConditionals.h"
#if defined(__IOS__) || TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
#define PLATFORM_MOBILE
#include <OpenGLES/ES3/gl.h>
#include <OpenGLES/ES3/glext.h>
#else  // !defined(__IOS__) || TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
#include <OpenGL/gl.h>
#define glDrawElementsInstanced glDrawElementsInstancedARB
#endif  // defined(__IOS__) || TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR

#else  // !defined(__APPLE__)
#ifdef __ANDROID__
#define PLATFORM_MOBILE
#include <android/api-level.h>
#if __ANDROID_API__ >= 18
#include <GLES3/gl3.h>
#else  // __ANDROID_API__ < 18
#include <GLES2/gl2.h>
#include "gl3stub.h"
#endif  // __ANDROID_API__ < 18

#else  // !defined(__ANDROID__), so WIN32 & Linux
#ifdef _WIN32
#define VC_EXTRALEAN
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif  // !defined(WIN32_LEAN_AND_MEAN)
#ifndef NOMINMAX
#define NOMINMAX
#endif  // !defined(NOMINMAX)
#include <windows.h>
#endif  // !defined(_WIN32)

#include <GL/gl.h>
#include <GL/glext.h>
#if !defined(GL_GLEXT_PROTOTYPES)
#ifdef _WIN32
#define GLBASEEXTS                                             \
  GLEXT(PFNGLACTIVETEXTUREARBPROC, glActiveTexture)            \
  GLEXT(PFNGLCOMPRESSEDTEXIMAGE2DPROC, glCompressedTexImage2D)
#else   // !defined(_WIN32)
#define GLBASEEXTS
#endif  // !defined(_WIN32)
#define GLEXTS                                                                \
  GLEXT(PFNGLGENFRAMEBUFFERSPROC, glGenFramebuffers)                          \
      GLEXT(PFNGLBINDFRAMEBUFFEREXTPROC, glBindFramebuffer)                   \
      GLEXT(PFNGLGENRENDERBUFFERSEXTPROC, glGenRenderbuffers)                 \
      GLEXT(PFNGLBINDRENDERBUFFEREXTPROC, glBindRenderbuffer)                 \
      GLEXT(PFNGLRENDERBUFFERSTORAGEEXTPROC, glRenderbufferStorage)           \
      GLEXT(PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC, glFramebufferRenderbuffer)   \
      GLEXT(PFNGLFRAMEBUFFERTEXTURE2DPROC, glFramebufferTexture2D)            \
      GLEXT(PFNGLDRAWBUFFERSPROC, glDrawBuffers)                              \
      GLEXT(PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC, glCheckFramebufferStatus)     \
      GLEXT(PFNGLDELETERENDERBUFFERSEXTPROC, glDeleteRenderbuffers)           \
      GLEXT(PFNGLDELETEFRAMEBUFFERSEXTPROC, glDeleteFramebuffers)             \
      GLEXT(PFNGLGENBUFFERSARBPROC, glGenBuffers)                             \
      GLEXT(PFNGLBINDBUFFERARBPROC, glBindBuffer)                             \
      GLEXT(PFNGLMAPBUFFERARBPROC, glMapBuffer)                               \
      GLEXT(PFNGLUNMAPBUFFERARBPROC, glUnmapBuffer)                           \
      GLEXT(PFNGLBUFFERDATAARBPROC, glBufferData)                             \
      GLEXT(PFNGLBUFFERSUBDATAARBPROC, glBufferSubData)                       \
      GLEXT(PFNGLDELETEBUFFERSARBPROC, glDeleteBuffers)                       \
      GLEXT(PFNGLGETBUFFERSUBDATAARBPROC, glGetBufferSubData)                 \
      GLEXT(PFNGLVERTEXATTRIBPOINTERARBPROC, glVertexAttribPointer)           \
      GLEXT(PFNGLENABLEVERTEXATTRIBARRAYARBPROC, glEnableVertexAttribArray)   \
      GLEXT(PFNGLDISABLEVERTEXATTRIBARRAYARBPROC, glDisableVertexAttribArray) \
      GLEXT(PFNGLCREATEPROGRAMPROC, glCreateProgram)                          \
      GLEXT(PFNGLDELETEPROGRAMPROC, glDeleteProgram)                          \
      GLEXT(PFNGLDELETESHADERPROC, glDeleteShader)                            \
      GLEXT(PFNGLUSEPROGRAMPROC, glUseProgram)                                \
      GLEXT(PFNGLCREATESHADERPROC, glCreateShader)                            \
      GLEXT(PFNGLSHADERSOURCEPROC, glShaderSource)                            \
      GLEXT(PFNGLCOMPILESHADERPROC, glCompileShader)                          \
      GLEXT(PFNGLGETPROGRAMIVARBPROC, glGetProgramiv)                         \
      GLEXT(PFNGLGETSHADERIVPROC, glGetShaderiv)                              \
      GLEXT(PFNGLGETPROGRAMINFOLOGPROC, glGetProgramInfoLog)                  \
      GLEXT(PFNGLGETSHADERINFOLOGPROC, glGetShaderInfoLog)                    \
      GLEXT(PFNGLATTACHSHADERPROC, glAttachShader)                            \
      GLEXT(PFNGLLINKPROGRAMARBPROC, glLinkProgram)                           \
      GLEXT(PFNGLGETUNIFORMLOCATIONARBPROC, glGetUniformLocation)             \
      GLEXT(PFNGLUNIFORM1FARBPROC, glUniform1f)                               \
      GLEXT(PFNGLUNIFORM2FARBPROC, glUniform2f)                               \
      GLEXT(PFNGLUNIFORM3FARBPROC, glUniform3f)                               \
      GLEXT(PFNGLUNIFORM4FARBPROC, glUniform4f)                               \
      GLEXT(PFNGLUNIFORM1FVARBPROC, glUniform1fv)                             \
      GLEXT(PFNGLUNIFORM2FVARBPROC, glUniform2fv)                             \
      GLEXT(PFNGLUNIFORM3FVARBPROC, glUniform3fv)                             \
      GLEXT(PFNGLUNIFORM4FVARBPROC, glUniform4fv)                             \
      GLEXT(PFNGLUNIFORM1IARBPROC, glUniform1i)                               \
      GLEXT(PFNGLUNIFORMMATRIX4FVARBPROC, glUniformMatrix4fv)                 \
      GLEXT(PFNGLUNIFORMMATRIX4FVARBPROC /*type*/, glUniformMatrix3x4fv)      \
      GLEXT(PFNGLBINDATTRIBLOCATIONARBPROC, glBindAttribLocation)             \
      GLEXT(PFNGLGETACTIVEUNIFORMARBPROC, glGetActiveUniform)                 \
      GLEXT(PFNGLGENERATEMIPMAPEXTPROC, glGenerateMipmap)                     \
      GLEXT(PFNGLGETATTRIBLOCATIONPROC, glGetAttribLocation)                  \
      GLEXT(PFNGLDRAWELEMENTSINSTANCEDPROC, glDrawElementsInstanced)
#define GLEXT(type, name) extern type name;
GLBASEEXTS
GLEXTS
#undef GLEXT
#endif  // !defined(GL_GLEXT_PROTOTYPES)
#endif  // !defined(__ANDROID__), so WIN32 & Linux
#endif  // !defined(__APPLE__)

// Define a GL_CALL macro to wrap each (void-returning) OpenGL call.
// This logs GL error when LOG_GL_ERRORS below is defined.
#if defined(_DEBUG) || DEBUG == 1 || !defined(NDEBUG)
#define LOG_GL_ERRORS
#endif

#ifdef LOG_GL_ERRORS
#define GL_CALL(call)                      \
  {                                        \
    call;                                  \
    LogGLError(__FILE__, __LINE__, #call); \
  }
#else
#define GL_CALL(call) \
  do {                \
    call;             \
  } while (0)
#endif

// The error checking function used by the GL_CALL macro above,
// uses glGetError() to check for errors.
extern void LogGLError(const char *file, int line, const char *call);

// These are missing in older NDKs.
#ifndef GL_ETC1_RGB8_OES
#define GL_ETC1_RGB8_OES 0x8D64
#endif

#ifndef GL_COMPRESSED_RGB8_ETC2
#define GL_COMPRESSED_RGB8_ETC2 0x9274
#endif

#ifndef GL_COMPRESSED_RGBA_ASTC_4x4_KHR
#define GL_COMPRESSED_RGBA_ASTC_4x4_KHR 0x93B0
#endif
#ifndef GL_COMPRESSED_RGBA_ASTC_5x4_KHR
#define GL_COMPRESSED_RGBA_ASTC_5x4_KHR 0x93B1
#endif
#ifndef GL_COMPRESSED_RGBA_ASTC_5x5_KHR
#define GL_COMPRESSED_RGBA_ASTC_5x5_KHR 0x93B2
#endif
#ifndef GL_COMPRESSED_RGBA_ASTC_6x5_KHR
#define GL_COMPRESSED_RGBA_ASTC_6x5_KHR 0x93B3
#endif
#ifndef GL_COMPRESSED_RGBA_ASTC_6x6_KHR
#define GL_COMPRESSED_RGBA_ASTC_6x6_KHR 0x93B4
#endif
#ifndef GL_COMPRESSED_RGBA_ASTC_8x5_KHR
#define GL_COMPRESSED_RGBA_ASTC_8x5_KHR 0x93B5
#endif
#ifndef GL_COMPRESSED_RGBA_ASTC_8x6_KHR
#define GL_COMPRESSED_RGBA_ASTC_8x6_KHR 0x93B6
#endif
#ifndef GL_COMPRESSED_RGBA_ASTC_8x8_KHR
#define GL_COMPRESSED_RGBA_ASTC_8x8_KHR 0x93B7
#endif
#ifndef GL_COMPRESSED_RGBA_ASTC_10x5_KHR
#define GL_COMPRESSED_RGBA_ASTC_10x5_KHR 0x93B8
#endif
#ifndef GL_COMPRESSED_RGBA_ASTC_10x6_KHR
#define GL_COMPRESSED_RGBA_ASTC_10x6_KHR 0x93B9
#endif
#ifndef GL_COMPRESSED_RGBA_ASTC_10x8_KHR
#define GL_COMPRESSED_RGBA_ASTC_10x8_KHR 0x93BA
#endif
#ifndef GL_COMPRESSED_RGBA_ASTC_10x10_KHR
#define GL_COMPRESSED_RGBA_ASTC_10x10_KHR 0x93BB
#endif
#ifndef GL_COMPRESSED_RGBA_ASTC_12x10_KHR
#define GL_COMPRESSED_RGBA_ASTC_12x10_KHR 0x93BC
#endif
#ifndef GL_COMPRESSED_RGBA_ASTC_12x12_KHR
#define GL_COMPRESSED_RGBA_ASTC_12x12_KHR 0x93BD
#endif

#endif  // FPLBASE_GLPLATFORM_H
