#include "file_io.hpp"
#include "shaders.hpp"

#include <iostream>
#include <unordered_map>

namespace kgl {

static std::unordered_map<std::string, GLuint> s_loaded_shaders;

GLuint create_shader(const std::string &shader_src_path, GLenum shader_type) {
    if (s_loaded_shaders.find(shader_src_path) != s_loaded_shaders.end()) {
        return s_loaded_shaders.at(shader_src_path);
    }

    std::string shader_src{read_string(shader_src_path)};
    GLuint shader{glCreateShader(shader_type)};
    if (!shader) {
        std::cerr << "Could not create shader.\n";
        return 0;
    }

    glShaderSource(shader, 1, (char **)&shader_src, NULL);
    glCompileShader(shader);

    GLint result{};
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if (!result) {
        GLint message_len{};
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &message_len);

        char *message = new char[message_len];
        glGetShaderInfoLog(shader, message_len, NULL, message);

        std::cerr << "Could not compile shader \"" << shader_src_path
                  << "\":\n\"" << message << "\"\n";
        glDeleteShader(shader);

        delete[] message;

        return 0;
    }

    s_loaded_shaders.emplace(shader_src_path, shader);

    return shader;
}

GLuint create_program(const std::string &vertex_shader_path,
                      const std::string &fragment_shader_path) {
    GLuint program{glCreateProgram()};
    if (program == 0) {
        std::cerr << "Could not create program.";
        return 0;
    }

    GLuint vert_shader{create_shader(vertex_shader_path, GL_VERTEX_SHADER)};
    GLuint frag_shader{create_shader(fragment_shader_path, GL_FRAGMENT_SHADER)};
    if (!vert_shader || !frag_shader) {
        return 0;
    }

    glAttachShader(program, vert_shader);
    glAttachShader(program, frag_shader);

    glLinkProgram(program);

    int success{};
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        GLint message_len{};
        glGetShaderiv(program, GL_INFO_LOG_LENGTH, &message_len);

        char *message = new char[message_len];
        glGetProgramInfoLog(program, message_len, NULL, message);

        std::cerr << "Could not link program \n\"" << message << "\"\n";
        glDeleteProgram(program);

        delete[] message;

        return 0;
    }

    return program;
}

void delete_all_shaders() {
    for (auto &[path, shader] : s_loaded_shaders) {
        glDeleteShader(shader);
    }

    s_loaded_shaders.clear();
}

} // namespace kgl
