#include "object.hpp"
#include "renderer.hpp"
#include "shaders.hpp"
#include "model_loader.hpp"
#include "texture.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext.hpp>

#include <iostream>
#include <memory>

int main() {
    int width{800};
    int height{600};
    if (!kgl::init(&width, &height)) {
        return -1;
    }

    std::unique_ptr<kgl::Model> model{kgl::Model::load("assets/sUs.obj")};
    kgl::Object mogus_obj{*model};

    kgl::Instance mogus{mogus_obj.instantiate()};
    mogus.transform.position.z = -4;

    kgl::Instance mogus2{mogus_obj.instantiate()};
    mogus2.transform.position.x = -2;
    mogus2.transform.position.z = -6;
    mogus2.transform.scale *= 0.5;

    kgl::setUpdateCallback([&](double) {
        if (kgl::is_key_pressed(GLFW_KEY_ESCAPE)) {
            kgl::stop();
        }
    });

    kgl::setRenderCallback([&]() {
        kgl::clearScreen();
        kgl::render(mogus);
        kgl::render(mogus2);
    });

    kgl::start();

    kgl::terminate();

    return 0;
}
