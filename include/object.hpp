#ifndef KASOUZA_RENDERER_INCLUDE_OBJECT_HPP
#define KASOUZA_RENDERER_INCLUDE_OBJECT_HPP

#include "gl_includes.hpp"
#include "transform.hpp"

namespace kgl {

class Model;
class Object;
struct Instance;

struct Instance {
    Object *object{nullptr};
    Transform transform{};
};

class Object {
  public:
    GLuint vao{};
    GLuint vbo{};
    int vertex_count{-1};

    Object(const Model &model);
    ~Object();

	Instance instantiate();
};

} // namespace kgl

#endif
