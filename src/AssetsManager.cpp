#include "../include/AssetsManager.hpp"
#include "../include/Model.hpp"
#include <iostream>
AssetsManager::AssetsManager() {}

AssetsManager::~AssetsManager()
{
    for (auto &[key, img] : images)
        delete img;
    for (auto &[key, tex] : textures)
        delete tex;
    for (auto &[key, model] : models)
        delete model;
}

void AssetsManager::loadImage(std::string path, std::string key)
{
    {
        std::lock_guard<std::mutex> lock(mutex);
        if (this->images.find(key) != this->images.end())
            return; 
    }

    int width, height, nrChannels;
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

    if (data == nullptr)
    {
        std::cerr << "ERROR: Could not load image in path: " << path << std::endl;
        return;
    }

    Image *image = new Image(data, width, height, nrChannels);

    std::lock_guard<std::mutex> lock(mutex);
    if (this->images.find(key) == this->images.end())
    {
        this->images.insert({key, image});
    }
    else
    {
        stbi_image_free(data);
        delete image;
    }
}

Image* AssetsManager::getImage(std::string key)
{
    std::lock_guard<std::mutex> lock(mutex);
    auto it = this->images.find(key);
    if (it == this->images.end())
    {
        std::cerr << "ERROR: image key not found: " << key << std::endl;
        return nullptr;
    }
    return it->second;
}

Texture* AssetsManager::loadTexture(std::string path, std::string key)
{
    {
        std::lock_guard<std::mutex> lock(mutex);
        auto it = this->textures.find(key);
        if (it != this->textures.end())
            return it->second; // já está na GPU
    } // <- lock liberado aqui, ANTES de chamar loadImage

    this->loadImage(path, key); // agora pode travar o mutex sem problema

    Image *image = this->getImage(key); // getImage também precisa do mesmo cuidado, veja abaixo
    if (image == nullptr)
        return nullptr;

    Texture *texture = new Texture(image); // chamadas GL — só pode rodar na thread principal

    std::lock_guard<std::mutex> lock(mutex);
    this->textures.insert({key, texture});
    return texture;
}
Texture *AssetsManager::getTexture(std::string key)
{
    auto it = this->textures.find(key);
    if (it == this->textures.end())
    {
        std::cerr << "ERROR: texture key not found: " << key << std::endl;
        return nullptr;
    }
    return it->second;
}

Model *AssetsManager::loadModel(std::string path, std::string key)
{
    auto it = models.find(key);
    if (it != models.end())
        return it->second; 

    Model *model = new Model(const_cast<char *>(path.c_str()), this);
    models.insert({key, model});
    return model;
}

Model *AssetsManager::getModel(std::string key)
{
    auto it = models.find(key);
    if (it == models.end())
    {
        std::cerr << "ERROR: model key not found: " << key << std::endl;
        return nullptr;
    }
    return it->second;
}