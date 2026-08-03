#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <glad/glad.h>
#include <string>
#include <iostream>
#include "../include/Image.hpp"

class Texture
{
public:
    unsigned int ID;
    Image *image;
    unsigned int textureRepeatPattern = GL_REPEAT;
    std::string type;
    Texture(Image *image)
    {
        this->image = image;
        glGenTextures(1, &ID);

        this->bind();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->textureRepeatPattern);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->textureRepeatPattern);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        GLenum format;
        switch (image->nrChannels)
        {
        case 1:
            format = GL_RED;
            break;
        case 2:
            format = GL_RG;
            break;
        case 3:
            format = GL_RGB;
            break;
        case 4:
            format = GL_RGBA;
            break;
        default:
            std::cerr << "ERROR: unsupported channel count: " << image->nrChannels << std::endl;
            format = GL_RGB;
        }

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        glTexImage2D(GL_TEXTURE_2D, 0, format, image->width, image->height, 0, format, GL_UNSIGNED_BYTE, image->data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    void bind()
    {
        glBindTexture(GL_TEXTURE_2D, ID);
    }
};

#endif