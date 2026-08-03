#ifndef ASSETSMANAGER_HPP
#define ASSETSMANAGER_HPP
#include <unordered_map>
#include <string>
#include "../vendor/stb_image/include/stb_image.h"
#include "../include/Image.hpp"
#include "../include/Texture.hpp"

class Model; // forward declaration 

class AssetsManager{
    public:
    AssetsManager();
    ~AssetsManager();

    std::unordered_map<std::string, Image*> images;
    std::unordered_map<std::string, Texture*> textures;
    std::unordered_map<std::string, Model*> models;

    void loadImage(std::string path, std::string key);
    Image* getImage(std::string key);

    Texture* loadTexture(std::string path, std::string key);
    Texture* getTexture(std::string key);

    Model* loadModel(std::string path, std::string key);
    Model* getModel(std::string key);
};

#endif