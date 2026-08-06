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
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    glm::vec4 solidColor = glm::vec4(1.f);
    
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, glm::vec4 solidColor);
    

    unsigned int VAO, VBO, EBO;
    void draw(Shader &shader);

    private:
    
    void setupMesh();
    void generateBuffers();
};

#endif