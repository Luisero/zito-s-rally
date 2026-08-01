#ifndef IMAGE_HPP
#define IMAGE_HPP


class Image{
    public:
    unsigned char *data;
    int width, height, nrChannels;

    Image(unsigned char *data, int width, int height, int nrChannels)
    {
        this->data = data;
        this->width = width;
        this->height = height;
        this->nrChannels = nrChannels;

    }
};

#endif