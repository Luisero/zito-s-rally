#ifndef MODEL_HPP
#define MODEL_HPP
#include <vector>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/DefaultLogger.hpp>
#include <assimp/LogStream.hpp>
#include "./Shader.hpp"
#include "./Mesh.hpp"
#include "./AssetsManager.hpp"

class Model
{
public:
    Model(char *path, AssetsManager *assetsManager);
    void Draw(Shader &shader, glm::mat4 &model);

private:
    AssetsManager *assetsManager;
    std::vector<Mesh> meshes;
    std::string directory;
    std::vector<glm::mat4> meshTransforms;
    void loadModel(std::string path);
    void processNode(aiNode *node, const aiScene *scene,glm::mat4 parentTransform);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
                                              std::string typeName);
};

#endif