#ifndef KASOUZA_RENDERER_INCLUDE_RENDERER_HPP
#define KASOUZA_RENDERER_INCLUDE_RENDERER_HPP

#include <functional>

struct GLFWwindow;

namespace kgl {

struct Instance;

using update_cb_t = std::function<void(double deltatime)>;
using render_cb_t = std::function<void()>;

// Initialize library
bool init(int *width, int *height);

// Set update callback
void setUpdateCallback(update_cb_t update_cb);

// Set render callback
void setRenderCallback(render_cb_t render_cb);

// Start game loop
void start();

// Stop game loop
void stop();

bool is_key_pressed(int key);

void terminate();

void render(const Instance &instance);

void clearScreen();
} // namespace kgl

#endif
