#ifndef KASOUZA_RENDERER_INCLUDE_SHADERS_HPP
#define KASOUZA_RENDERER_INCLUDE_SHADERS_HPP

#include "gl_includes.hpp"

#include <string>

namespace kgl {

GLuint create_shader(const std::string &shader_src_path, GLenum shader_type);

GLuint create_program(const std::string &vertex_shader_path,
                      const std::string &fragment_shader_path);

void delete_all_shaders();

} // namespace kgl
#endif
