#include "../include/AssetsManager.hpp"

AssetsManager::AssetsManager() {

};

void AssetsManager::loadImage(std::string path, std::string key)
{

    int width, height, nrChannels;
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

    Image *image = new Image(data,width,height,nrChannels);
    this->images.insert({key,image});
}

Image* AssetsManager::getImage(std::string key)
{
    return this->images[key];
}