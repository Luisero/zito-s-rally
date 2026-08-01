#include "../include/Mesh.hpp"
#include <glad/glad.h>

Mesh::Mesh(Texture* texture)
{
    this->texture = texture;
}

void Mesh::generateBuffers()
{
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);
}
void Mesh::setupMesh()
{
    Mesh::generateBuffers();
    glBindVertexArray(VAO);

    // temporary vector to vertex and color interleaving
    std::vector<float> vertexData;
    for (size_t i = 0; i < vertices.size() / 3; ++i)
    {

        vertexData.push_back(vertices[i * 3]);
        vertexData.push_back(vertices[i * 3 + 1]);
        vertexData.push_back(vertices[i * 3 + 2]);

        vertexData.push_back(colors[i * 3]);
        vertexData.push_back(colors[i * 3 + 1]);
        vertexData.push_back(colors[i * 3 + 2]);

        vertexData.push_back(UVs[i * 2]);
        vertexData.push_back(UVs[i * 2 + 1]);
    }

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexData.size(), vertexData.data(), GL_DYNAMIC_DRAW);

    // Vertex
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // Colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //UVs
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);
}

void Mesh::draw(Shader *shader)
{
    shader->use();
   
    glBindTexture(GL_TEXTURE_2D, this->texture->ID);
    glBindVertexArray(VAO);

    //  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}