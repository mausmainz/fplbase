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

#include "precompiled.h"
#include <vector>

#include "fplbase/renderer.h"
#include "fplbase/shader.h"

#ifdef _WIN32
#define snprintf(buffer, count, format, ...) \
  _snprintf_s(buffer, count, count, format, __VA_ARGS__)
#endif  // _WIN32

namespace fplbase {

Shader::~Shader() {
  if (vs_) GL_CALL(glDeleteShader(vs_));
  if (ps_) GL_CALL(glDeleteShader(ps_));
  if (program_) GL_CALL(glDeleteProgram(program_));
}

UniformHandle Shader::FindUniform(const char *uniform_name) {
  GL_CALL(glUseProgram(program_));
  return glGetUniformLocation(program_, uniform_name);
}

void Shader::SetUniform(GLint uniform_loc, const float *value,
                        size_t num_components) {
  // clang-format off
  switch (num_components) {
    case 1: GL_CALL(glUniform1f(uniform_loc, *value)); break;
    case 2: GL_CALL(glUniform2fv(uniform_loc, 1, value)); break;
    case 3: GL_CALL(glUniform3fv(uniform_loc, 1, value)); break;
    case 4: GL_CALL(glUniform4fv(uniform_loc, 1, value)); break;
    case 16: GL_CALL(glUniformMatrix4fv(uniform_loc, 1, false, value)); break;
    default: assert(0); break;
  }
  // clang-format on
}

void Shader::InitializeUniforms() {
  // Look up variables that are standard, but still optionally present in a
  // shader.
  uniform_model_view_projection_ =
      glGetUniformLocation(program_, "model_view_projection");
  uniform_model_ = glGetUniformLocation(program_, "model");

  uniform_color_ = glGetUniformLocation(program_, "color");

  uniform_light_pos_ = glGetUniformLocation(program_, "light_pos");
  uniform_camera_pos_ = glGetUniformLocation(program_, "camera_pos");

  uniform_time_ = glGetUniformLocation(program_, "time");

  // An array of vec4's. Three vec4's compose one affine transform.
  // The i'th affine transform is the translation, rotation, and
  // orientation of the i'th bone.
  uniform_bone_transforms_ = glGetUniformLocation(program_, "bone_transforms");

  // Set up the uniforms the shader uses for texture access.
  char texture_unit_name[] = "texture_unit_#####";
  for (int i = 0; i < kMaxTexturesPerShader; i++) {
    snprintf(texture_unit_name, sizeof(texture_unit_name), "texture_unit_%d",
             i);
    auto loc = glGetUniformLocation(program_, texture_unit_name);
    if (loc >= 0) GL_CALL(glUniform1i(loc, i));
  }
}

void Shader::Set(const Renderer &renderer) const {
  const int kNumVec4InBoneTransform = 3;

  GL_CALL(glUseProgram(program_));

  if (uniform_model_view_projection_ >= 0)
    GL_CALL(glUniformMatrix4fv(uniform_model_view_projection_, 1, false,
                               &renderer.model_view_projection()[0]));
  if (uniform_model_ >= 0)
    GL_CALL(glUniformMatrix4fv(uniform_model_, 1, false, &renderer.model()[0]));
  if (uniform_color_ >= 0)
    GL_CALL(glUniform4fv(uniform_color_, 1, &renderer.color()[0]));
  if (uniform_light_pos_ >= 0)
    GL_CALL(glUniform3fv(uniform_light_pos_, 1, &renderer.light_pos()[0]));
  if (uniform_camera_pos_ >= 0)
    GL_CALL(glUniform3fv(uniform_camera_pos_, 1, &renderer.camera_pos()[0]));
  if (uniform_time_ >= 0)
    GL_CALL(glUniform1f(uniform_time_, static_cast<float>(renderer.time())));
  if (uniform_bone_transforms_ >= 0 && renderer.num_bones() > 0) {
    assert(renderer.bone_transforms() != nullptr);

    const mathfu::AffineTransform *bone_transforms = renderer.bone_transforms();

    GL_CALL(glUniform4fv(uniform_bone_transforms_,
                         renderer.num_bones() * kNumVec4InBoneTransform,
                         &bone_transforms[0][0]));
  }
}

}  // namespace fplbase

