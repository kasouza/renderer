#include "object.hpp"
#include "model_loader.hpp"

namespace kgl {

Object::Object(const Model &model) {
	vertex_count = model.vertices.size();
    // Create Buffer / VAO
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    // Fill vbo with data from model
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, model.vertices.size() * sizeof(Vertex), model.vertices.data(),
                 GL_STATIC_DRAW);

    // Bind VAO and setvertex attributes
    glBindVertexArray(vao);

    // Position -> glm::vec3
    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(kgl::Vertex),
                          (void *)0);
    glEnableVertexAttribArray(0);

    // Texture Coordinates -> glm::vec2
    glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(kgl::Vertex),
                          (void *)(sizeof(glm::vec3)));
    glEnableVertexAttribArray(1);

    // Normals -> glm::vec3
    glVertexAttribPointer(2, 3, GL_FLOAT, false, sizeof(kgl::Vertex),
                          (void *)(sizeof(glm::vec3) + sizeof(glm::vec2)));
    glEnableVertexAttribArray(2);
}

Object::~Object() {
    if (vbo) {
        glDeleteBuffers(1, &vbo);
    }

    if (vao) {
        glDeleteVertexArrays(1, &vao);
    }
}

Instance Object::instantiate() {
    return Instance{
        this,
        Transform{},
    };
}

} // namespace kgl
