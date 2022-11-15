#include "model_loader.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/geometric.hpp"

#include <cassert>
#include <cstring>
#include <deque>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <vector>

namespace kgl {

std::deque<std::string> split(const std::string &str,
                              const std::string &delimiters = " ") {
    std::deque<std::string> tokens{};

    char *str_copy{new char[(str.length() + 1) * sizeof(char)]};
    strcpy(str_copy, str.c_str());

    char *token{std::strtok(str_copy, delimiters.c_str())};

    while (token) {
        tokens.push_back(token);
        token = std::strtok(nullptr, delimiters.c_str());
    }

    delete[] str_copy;

    return tokens;
}

Model *Model::load(const std::string &path) {
    std::ifstream input_stream{path};

    if (!input_stream.good()) {
        std::cerr << "Could not open: \"" << path << "\"\n";
        assert(input_stream.good());
    }

    std::vector<glm::vec3> positions{};
    std::vector<glm::vec3> normals{};
    std::vector<glm::vec2> tex_coords{};

    Model *model{new Model{}};
    bool ok{true};

    char buf[256]{};
    input_stream.getline(buf, 256);

    while (input_stream.good()) {
        std::deque<std::string> tokens{split(buf)};
        std::string first_tk{tokens.front()};
        tokens.pop_front();

        if (first_tk == "v") {
            // Remove the `W` coordinate
            if (tokens.size() > 3) {
                tokens.pop_back();
            }

            glm::vec3 vertex{};
            for (size_t i = 0; i < 3; i++) {
                vertex[i] = std::stof(tokens[i]);
            }

            positions.push_back(vertex);

        } else if (first_tk == "vt") {
            glm::vec2 uv{};
            for (size_t i = 0; i < tokens.size(); i++) {
                uv[i] = std::stof(tokens[i]);
            }

            tex_coords.push_back(uv);

        } else if (first_tk == "vn") {
            glm::vec3 normal{};
            for (size_t i = 0; i < tokens.size(); i++) {
                normal[i] = stof(tokens[i]);
            }

            normals.push_back(glm::normalize(normal));

        } else if (first_tk == "f") {
            if (tokens.size() > 3) {
                std::cerr << "UNSUPPORTED: Face is not a triangle!\n";
                ok = false;
                break;
            }

            for (auto &token : tokens) {
                // The -1 here is important becaus .obj files have indices
                // starting at 1 instead of 0

                std::deque<std::string> vertex{split(token, "/")};

                // Position
                Vertex vert{};

                int position_idx{std::stoi(vertex[0]) - 1};
                vert.position = positions.at(position_idx);

                if (vertex.size() >= 2) {
                    int uv_idx{std::stoi(vertex[1]) - 1};
                    vert.tex_coords = tex_coords.at(uv_idx);
                }

                if (vertex.size() >= 3) {
                    int normal_idx{std::stoi(vertex[2]) - 1};
                    vert.normal = normals.at(normal_idx);
                }

                model->vertices.push_back(vert);
                // model->indices.push_back(std::stoi(token) - 1);
            }

        } else if (first_tk == "vp") {
            std::cerr << "UNSUPPORTED: Parameter space vertices!\n";
            ok = false;
            break;
        } else if (first_tk != "#") {
            std::cerr << "ERROR: Invalid token: " << std::quoted(first_tk)
                      << std::endl;
            // ok = false;
            // break;
        }

        input_stream.getline(buf, 256);
    }

    if (!ok) {
        delete model;
        model = nullptr;
        return nullptr;
    }

    return model;
}

} // namespace kgl
