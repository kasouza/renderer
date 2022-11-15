#ifndef KASOUZA_RENDERER_INCLUDE_CAMERA_HPP
#define KASOUZA_RENDERER_INCLUDE_CAMERA_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cmath>

namespace kgl {

class Camera {
  public:
    glm::vec3 position{0.0f};
    double pitch{0.0};
    double yaw{-(M_PI / 2)};

    float speed{2.5f};
    float sensitivity{0.1f};

	glm::vec3 up{0.0f, 1.0f, 0.0f};

	glm::vec3 calculateFront();
	glm::vec3 calculateRight();
    glm::mat4 calculateViewMatrix();
};

} // namespace kgl

#endif
