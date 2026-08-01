#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <glad/glad.h>
#include "../include/Image.hpp"

class Texture
{
public:
    unsigned int ID;
    Image *image;
    unsigned int textureRepeatPattern = GL_REPEAT;

    Texture(Image *image)
    {
        this->image = image;
        glGenTextures(1, &ID);

        this->bind();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->textureRepeatPattern);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->textureRepeatPattern);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    void bind()
    {
        glBindTexture(GL_TEXTURE_2D, ID);
    }
};

#endif