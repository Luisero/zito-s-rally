#include <vector>
#ifndef MESH_HPP
#define MESH_HPP
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "./Texture.hpp"
#include "./Shader.hpp"

struct Vertex{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};


class Mesh{
    public:
    Mesh();
    Mesh(Texture* texture);
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    std::vector<float> colors;
    std::vector<float> UVs;
    Texture *texture;

    unsigned int VAO, VBO, EBO;

    
    void setupMesh();
    void generateBuffers();
    void draw(Shader *shader);
};

#endif