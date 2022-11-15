#include "renderer.hpp"
#include "camera.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "object.hpp"
#include "shaders.hpp"
#include "texture.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

namespace kgl {
static GLFWwindow *s_window = nullptr;
static update_cb_t s_update_cb = nullptr;
static render_cb_t s_render_cb = nullptr;
static int *s_width_ptr = nullptr;
static int *s_height_ptr = nullptr;
static GLuint s_program = 0;
static GLuint s_tex = 0;
static Camera s_cam{};

static struct {
    GLint color;
    GLint modelview_matrix;
    GLint projection;
    GLint tex;
    GLint normal_matrix;
} uniforms;

void framebuffer_resize_callback(GLFWwindow *, int width, int height) {
    glViewport(0, 0, width, height);
    *s_width_ptr = width;
    *s_height_ptr = height;
}

bool init(int *width, int *height) {
    glfwInitHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwInitHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwInitHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    if (!glfwInit()) {
        std::cerr << "Could not initialize GLFW.\n";
        return false;
    }

    s_width_ptr = width;
    s_height_ptr = height;
    s_window = glfwCreateWindow(*width, *height, "Renderer", nullptr, nullptr);
    if (!s_window) {
        std::cerr << "Could not create window.\n";
        return false;
    }

    glfwMakeContextCurrent(s_window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Could not load OpenGL processes.\n";
        return false;
    }

    glViewport(0, 0, *width, *height);
    glfwSetFramebufferSizeCallback(s_window, framebuffer_resize_callback);

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    // Other stuff
    s_program = create_program("shaders/vertex.vert", "shaders/fragment.frag");

    uniforms.color = glGetUniformLocation(s_program, "u_color");
    uniforms.modelview_matrix =
        glGetUniformLocation(s_program, "modelview_matrix");
    uniforms.tex = glGetUniformLocation(s_program, "u_tex");
    uniforms.projection = glGetUniformLocation(s_program, "projection");
    uniforms.normal_matrix = glGetUniformLocation(s_program, "normal_matrix");

    s_tex = create_texture("assets/container.png");
    if (!s_tex) {
        std::cerr << "Couldn't load texture.\n";
    }

    return s_window;
}

void setUpdateCallback(update_cb_t update_cb) { s_update_cb = update_cb; }
void setRenderCallback(render_cb_t render_cb) { s_render_cb = render_cb; }

void start() {
    double dt{};
    double start{glfwGetTime()};

    while (!glfwWindowShouldClose(s_window)) {
        // Delta timing
        double now{glfwGetTime()};
        dt = now - start;
        start = now;

        // event handling
        glfwPollEvents();

        // Update
        float cam_velocity{static_cast<float>(s_cam.speed * dt)};
        glm::vec3 front{s_cam.calculateFront()};
        if (glfwGetKey(s_window, GLFW_KEY_W) == GLFW_PRESS) {
            s_cam.position += front * cam_velocity;
        } else if (glfwGetKey(s_window, GLFW_KEY_S) == GLFW_PRESS) {
            s_cam.position -= front * cam_velocity;
        }

        glm::vec3 right{s_cam.calculateRight()};
        if (glfwGetKey(s_window, GLFW_KEY_A) == GLFW_PRESS) {
            s_cam.position -= right * cam_velocity;
        } else if (glfwGetKey(s_window, GLFW_KEY_D) == GLFW_PRESS) {
            s_cam.position += right * cam_velocity;
        }

        s_cam.sensitivity = 5;
        if (glfwGetKey(s_window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            s_cam.yaw -= s_cam.sensitivity * dt;
        } else if (glfwGetKey(s_window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            s_cam.yaw += s_cam.sensitivity * dt;
        }

        if (glfwGetKey(s_window, GLFW_KEY_UP) == GLFW_PRESS) {
            s_cam.pitch += s_cam.sensitivity * dt;
        } else if (glfwGetKey(s_window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            s_cam.pitch -= s_cam.sensitivity * dt;
        }

        if (s_cam.pitch <= -M_PI_2) {
            s_cam.pitch = -M_PI_2 + 0.02;
        }
        if (s_cam.pitch >= M_PI_2) {
            s_cam.pitch = M_PI_2 - 0.02;
        }

        if (s_update_cb) {
            s_update_cb(dt);
        }

        // Render
        if (s_render_cb) {
            s_render_cb();
        }

        glfwSwapBuffers(s_window);
    }
}

void stop() { glfwSetWindowShouldClose(s_window, true); }

bool is_key_pressed(int key) { return glfwGetKey(s_window, key) == GLFW_PRESS; }

void terminate() {
    glDeleteProgram(s_program);
    delete_all_shaders();
    glfwTerminate();
}

void render(const Instance &instance) {
    static float i = 0;
    i += 0.01;

    glUseProgram(s_program);
    glBindVertexArray(instance.object->vao);

    glUniform4f(uniforms.color, 0, 1, 0, 1);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, s_tex);
    glUniform1i(uniforms.tex, 0);

    // MOdel matrix
    glm::mat4 model_matrix{1.0f};
    model_matrix = glm::translate(model_matrix, instance.transform.position);
    model_matrix = glm::scale(model_matrix, instance.transform.scale);
    model_matrix = glm::rotate(model_matrix, i, glm::vec3{0.0, 1.0, 0.0});

    // View matrix
    // s_cam.yaw = -M_PI / 2;

    glm::mat4 view_matrix{s_cam.calculateViewMatrix()};

    // Model view
    glm::mat4 modelview_matrix{view_matrix * model_matrix};
    glUniformMatrix4fv(uniforms.modelview_matrix, 1, false,
                       glm::value_ptr(modelview_matrix));

    // Projection matrix
    glm::mat4 projection_matrix{glm::perspective(
        90.0f, static_cast<float>(*s_width_ptr) / *s_height_ptr, 0.1f, 100.0f)};
    glUniformMatrix4fv(uniforms.projection, 1, false,
                       glm::value_ptr(projection_matrix));

    // Normal Matrix
    // This normal matrix applies the transformations that happens in the model
    // matrix into the normal vector and prevents distortions, keeping it
    // perpendicular to the surface
    glm::mat3 normal_matrix{glm::transpose(glm::inverse(modelview_matrix))};
    glUniformMatrix3fv(uniforms.normal_matrix, 1, false,
                       glm::value_ptr(normal_matrix));

    // Lights
    glm::vec4 directional_light_dir{1.0, 0.0, 0.0, 1.0};
    directional_light_dir = glm::normalize(view_matrix * directional_light_dir);
    glUniform3f(glGetUniformLocation(s_program, "directional_light.dir"),
                directional_light_dir.x, directional_light_dir.y,
                directional_light_dir.z);

    glUniform3f(glGetUniformLocation(s_program, "directional_light.color"), 0.0,
                1.0, 0.0);

    glm::vec4 spot_light_pos{-4.0, 0.0, 0.0, 1.0};
    spot_light_pos = view_matrix * spot_light_pos;

    glUniform3f(glGetUniformLocation(s_program, "spotlights[0].position"),
                spot_light_pos.x, spot_light_pos.y, spot_light_pos.z);
    glUniform3f(glGetUniformLocation(s_program, "spotlights[0].color"), 1.0f,
                0.0f, 0.0f);

    glUniform1i(glGetUniformLocation(s_program, "num_spotlights"), 1);

    // Draw call
    glDrawArrays(GL_TRIANGLES, 0, instance.object->vertex_count);
}

void clearScreen() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

} // namespace kgl
