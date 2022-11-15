#include "gl_includes.hpp"

#include <stb_image.h>

#include <iostream>

namespace kgl {

unsigned int create_texture(const std::string &path_to_image) {
    stbi_set_flip_vertically_on_load(true);

    unsigned int texture;
    glGenTextures(1, &texture);

    int width, height, nr_channels;
    unsigned char *data{
        stbi_load(path_to_image.c_str(), &width, &height, &nr_channels, 0)};

    if (!data) {
        std::cerr << "Couldn't load texture: \"" << path_to_image << "\"\n";
        glDeleteTextures(1, &texture);
        return 0;
    }

    GLenum format;
    if (nr_channels == 1) {
        format = GL_RED;
    } else if (nr_channels == 3) {
        format = GL_RGB;
    } else if (nr_channels == 4) {
        format = GL_RGBA;
    } else {
        std::cerr << "Couldn't specify image's channels: \"" << path_to_image
                  << "\"\n";
        stbi_image_free(data);
        glDeleteTextures(1, &texture);
        return 0;
    }

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);

    return texture;
}

} // namespace kgl
