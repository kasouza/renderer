#ifndef KASOUZA_RENDERER_INCLUDE_MODEL_LOADER_HPP
#define KASOUZA_RENDERER_INCLUDE_MODEL_LOADER_HPP

#include "gl_includes.hpp"
#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_float3.hpp"

#include <string>
#include <vector>

namespace kgl {

// BE CAREFULL WHEN REORDERING THE COMPONENTS, this order is used to setup VBOS
// and stuff
struct Vertex {
    glm::vec3 position;
    glm::vec2 tex_coords;
    glm::vec3 normal;
};

struct Model {
    static Model *load(const std::string &path);

    std::vector<Vertex> vertices{};
    std::vector<GLuint> indices{};
};

} // namespace kgl

#endif
