#ifndef MODEL_HPP
#define MODEL_HPP
#include <vector>
#include <string>
#include <future>
#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/Shape/MeshShape.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/DefaultLogger.hpp>
#include <assimp/LogStream.hpp>
#include "./Shader.hpp"
#include "./Mesh.hpp"

class AssetsManager; // forward declaration 

class Model
{
public:
    Model(char *path, AssetsManager *assetsManager);
    void Draw(Shader &shader, glm::mat4 &model);
    float getBoundingRadius(); 
    void getCollisionData(JPH::VertexList &outVertices,
                          JPH::IndexedTriangleList &outTriangles);

private:
    AssetsManager *assetsManager;
    std::vector<Mesh> meshes;
    std::string directory;
    std::vector<glm::mat4> meshTransforms;
    void loadModel(std::string path);
    void preloadTextures(const aiScene *scene); 

    void processNode(aiNode *node, const aiScene *scene, glm::mat4 parentTransform);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
                                              std::string typeName);
};

#endif