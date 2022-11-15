#include <cassert>
#include <fstream>
#include <sstream>
#include <iostream>

namespace kgl {

std::string read_string(const std::string &path) {
    std::ifstream input_stream{path};

    if (!input_stream.good()) {
        std::cerr << "Could not open: \"" << path << "\"\n";
        assert(input_stream.good());
        // return "";
    }

    std::stringstream stream;
    stream << input_stream.rdbuf();

    return stream.str();
}

} // namespace kgl
