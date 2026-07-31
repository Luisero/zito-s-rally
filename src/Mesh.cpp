#include "../include/Mesh.hpp"
#include <glad/glad.h>

Mesh::Mesh()
{

}

void Mesh::generateBuffers()
{
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
}

void Mesh::setupMesh()
{
    Mesh::generateBuffers();
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
}


void Mesh::draw()
{
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}