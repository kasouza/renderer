#ifndef KASOUZA_RENDERER_INCLUDE_TRANSFORM_HPP
#define KASOUZA_RENDERER_INCLUDE_TRANSFORM_HPP

#include <glm/vec3.hpp>

namespace kgl {

class Transform {
  public:
    glm::vec3 position{0.0f};
    glm::vec3 scale{1.0f};
};

} // namespace kgl

#endif
