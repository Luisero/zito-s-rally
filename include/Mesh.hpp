#include <vector>
#ifndef MESH_HPP
#define MESH_HPP

class Mesh{
    public:
    Mesh();
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    std::vector<float> colors;
    unsigned int VAO, VBO, EBO;
    
    void setupMesh();
    void generateBuffers();
    void draw();
};

#endif