#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <yaaacd/common.hpp>

YAAACD::Vertex _parse_vertex(std::string line) {
    std::istringstream input(line);
    char v;

    double x = 0;
    double y = 0;
    double z = 0;

    input >> v;

    input >> x >> y >> z;

    return YAAACD::Vertex(x, y, z);
}

std::array<int, 3> _parse_face(std::string line) {
    char* string = &line[0];

    int indices[3] = {0};

    if (sscanf(
            string, "f %d%*s %d%*s %d%*s", indices, indices + 1, indices + 2
        ) < 3)
        sscanf(string, "f %d %d %d", indices, indices + 1, indices + 2);

    return {indices[0], indices[1], indices[2]};
}

std::vector<YAAACD::Triangle> parse_obj_file(std::string filename) {
    std::ifstream obj_file(filename, std::ios::in);

    YAAACD::Vertex emptyVertex(0, 0, 0);

    std::vector<YAAACD::Triangle> triangles;
    std::vector<YAAACD::Vertex> vertices{emptyVertex};
    std::vector<std::array<int, 3>> face_incides;

    while (!obj_file.eof()) {
        std::string line;

        getline(obj_file, line);

        if (line.find("v ") != std::string::npos) {
            vertices.push_back(_parse_vertex(line));
        } else if (line.find("f ") != std::string::npos) {
            face_incides.push_back(_parse_face(line));
        }
    }

    for (size_t i = 0; i < face_incides.size(); i++) {
        triangles.push_back(YAAACD::Triangle{
            YAAACD::Vertex(vertices[face_incides[i][0]]),
            YAAACD::Vertex(vertices[face_incides[i][1]]),
            YAAACD::Vertex(vertices[face_incides[i][2]])});
    }

    obj_file.close();

    return triangles;
}
