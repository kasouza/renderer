#include "camera.hpp"
#include <iostream>

namespace kgl {

glm::vec3 Camera::calculateFront() {
    glm::vec3 direction{0.0f};
    direction.x = std::cos(yaw) * std::cos(pitch);
    direction.y = std::sin(pitch);
    direction.z = std::sin(yaw) * std::cos(pitch);

    return glm::normalize(direction);
}

glm::vec3 Camera::calculateRight() {
	glm::vec3 front{ calculateFront() };
	return glm::normalize(glm::cross(front, up));
}

glm::mat4 Camera::calculateViewMatrix() {
    glm::vec3 front{calculateFront()};
    return glm::lookAt(position, position + front, up);
}

} // namespace kgl
