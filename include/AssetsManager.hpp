#ifndef ASSETSMANAGER_HPP
#define ASSETSMANAGER_HPP
#include <unordered_map>
#include <string>
#include "../vendor/stb_image/include/stb_image.h"
#include "../include/Image.hpp"

class AssetsManager{
    public:
    AssetsManager();
    std::unordered_map<std::string, Image*> images;

    void loadImage(std::string path, std::string key);
    Image* getImage(std::string key);

};


#endif 