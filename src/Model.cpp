#include "../include/Model.hpp"
#include "../include/AssetsManager.hpp" 
#include <iostream>

#include <iostream>

glm::mat4 AiToGlm(const aiMatrix4x4 &from)
{
    glm::mat4 to;
    to[0][0] = from.a1;
    to[1][0] = from.a2;
    to[2][0] = from.a3;
    to[3][0] = from.a4;
    to[0][1] = from.b1;
    to[1][1] = from.b2;
    to[2][1] = from.b3;
    to[3][1] = from.b4;
    to[0][2] = from.c1;
    to[1][2] = from.c2;
    to[2][2] = from.c3;
    to[3][2] = from.c4;
    to[0][3] = from.d1;
    to[1][3] = from.d2;
    to[2][3] = from.d3;
    to[3][3] = from.d4;
    return to;
}

Model::Model(char *path, AssetsManager *am)
{
    this->assetsManager = am;
    this->loadModel(std::string(path));
}

void Model::Draw(Shader &shader, glm::mat4 &model)
{

    for (unsigned int i = 0; i < meshes.size(); i++)
    {
        glm::mat4 currentModel = model * meshTransforms[i];
        shader.setMat4("model", currentModel);
        glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(currentModel)));
        shader.setMat3("normalMatrix", normalMatrix);

        meshes[i].draw(shader);
    }
}

void Model::loadModel(std::string path)
{
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << "\n";
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene, glm::mat4(1.f));
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector;
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;

        glm::vec3 normal;
        normal.x = mesh->mNormals[i].x;
        normal.y = mesh->mNormals[i].y;
        normal.z = mesh->mNormals[i].z;
        vertex.Normal = normal;

        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
        }
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }
    // process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    // process material
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture> diffuseMaps = loadMaterialTextures(material,
                                                           aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        std::vector<Texture> specularMaps = loadMaterialTextures(material,
                                                            aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    return Mesh(vertices, indices, textures);
}
void Model::processNode(aiNode *node, const aiScene *scene, glm::mat4 parentTransform)
{
    glm::mat4 nodeTransform = parentTransform * AiToGlm(node->mTransformation);
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
        meshTransforms.push_back(nodeTransform);
    }
    // then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene, nodeTransform);
    }
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
{
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        std::string path = directory + "/" + str.C_Str();

        Texture *texture = assetsManager->loadTexture(path, path);
        if (texture == nullptr)
            continue;

        Texture textureCopy = *texture;
        textureCopy.type = typeName; // mesma GLuint ID, tipo diferente pro shader
        
        textures.push_back(textureCopy);
    }
    return textures;
}

float Model::getBoundingRadius()
{
    glm::vec3 minBounds(std::numeric_limits<float>::max());
    glm::vec3 maxBounds(std::numeric_limits<float>::lowest());
    bool anyVertex = false;

    for (unsigned int m = 0; m < meshes.size(); m++)
    {
        for (const auto &vertex : meshes[m].vertices)
        {
            glm::vec4 worldPos = meshTransforms[m] * glm::vec4(vertex.Position, 1.0f);
            glm::vec3 pos = glm::vec3(worldPos);

            minBounds = glm::min(minBounds, pos);
            maxBounds = glm::max(maxBounds, pos);
            anyVertex = true;
        }
    }

    if (!anyVertex)
        return 0.0f;

    glm::vec3 center = (minBounds + maxBounds) * 0.5f;
    float radius = 0.0f;

    for (unsigned int m = 0; m < meshes.size(); m++)
    {
        for (const auto &vertex : meshes[m].vertices)
        {
            glm::vec4 worldPos = meshTransforms[m] * glm::vec4(vertex.Position, 1.0f);
            float dist = glm::length(glm::vec3(worldPos) - center);
            radius = std::max(radius, dist);
        }
    }

    return radius;
}
