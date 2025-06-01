#include "texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>

Texture::Texture(std::vector<std::string> filepaths, unsigned int type, unsigned int texture_wrap_s, unsigned int texture_wrap_t, unsigned int texture_min_filter, unsigned int texture_mag_filter) : texture_count(filepaths.size()), _type(type)
{
    stbi_set_flip_vertically_on_load(true);  
    texture_count = filepaths.size();
    _type = type;
    textures = (unsigned int*) malloc(sizeof(*textures) * texture_count);
    glGenTextures(filepaths.size(), textures);
    int width, height, nrChannels;
    unsigned char* data;
    for (size_t i = 0; i < texture_count; ++i) {
        glBindTexture(type, textures[i]);
        glTexParameteri(type, GL_TEXTURE_WRAP_S, texture_wrap_s);
        glTexParameteri(type, GL_TEXTURE_WRAP_T, texture_wrap_t);
        glTexParameteri(type, GL_TEXTURE_MIN_FILTER, texture_min_filter);
        glTexParameteri(type, GL_TEXTURE_MAG_FILTER, texture_mag_filter);

        data = stbi_load(filepaths[i].c_str(), &width, &height, &nrChannels, 0);

        if (data) {
            if (nrChannels == 4)
                glTexImage2D(type, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            else if (nrChannels == 3)
                glTexImage2D(type, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            else  // idk
                glTexImage2D(type, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(type);
        } else {
            std::cout << "Failed to load texture for file: " << filepaths[i] << "\n";
        }

        stbi_image_free(data);
    }
}

Texture::~Texture()
{
    glDeleteTextures(texture_count, textures);
    if (textures != nullptr)
        free(textures);
}

void Texture::setTextureParams(size_t textureIdx, unsigned int type,
        unsigned int texture_wrap_s,
        unsigned int texture_wrap_t,
        unsigned int texture_min_filter,
        unsigned int texture_mag_filter)
{
        glBindTexture(type, textures[textureIdx]);
        glTexParameteri(type, GL_TEXTURE_WRAP_S, texture_wrap_s);
        glTexParameteri(type, GL_TEXTURE_WRAP_T, texture_wrap_t);
        glTexParameteri(type, GL_TEXTURE_MIN_FILTER, texture_min_filter);
        glTexParameteri(type, GL_TEXTURE_MAG_FILTER, texture_mag_filter);
}

void Texture::activate() const
{
    for (size_t i = 0; i < texture_count; ++i) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(_type, textures[i]);
    }
}

void Texture::configureShader(Shader& shader)
{
    shader.use();
    for (size_t i = 0; i < texture_count; ++i) {
        shader.setInt("texture" + std::to_string(i), i);
    }
}
