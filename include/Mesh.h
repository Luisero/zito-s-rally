#include <vector>
#ifndef MESH_HPP
#define MESH_HPP

class Mesh{
    public:
    Mesh();
    std::vector<float> vertices;
    unsigned int VAO, VBO, UVS;
    
    void setupMesh();
    void generateBuffers();
    void draw();
};

#endif