#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <glad/glad.h>

#include "shader.hpp"

#include <string>
#include <vector>

class Texture {

private:
  unsigned int *textures;
  size_t texture_count = 0;
  unsigned int _type;

public:
  Texture(std::vector<std::string> filepaths, unsigned int type = GL_TEXTURE_2D,
          unsigned int texture_wrap_s = GL_REPEAT,
          unsigned int texture_wrap_t = GL_REPEAT,
          unsigned int texture_min_filter = GL_NEAREST_MIPMAP_NEAREST,
          unsigned int texture_mag_filter = GL_NEAREST);

  void
  setTextureParams(size_t textureIdx, unsigned int type = GL_TEXTURE_2D,
                   unsigned int texture_wrap_s = GL_REPEAT,
                   unsigned int texture_wrap_t = GL_REPEAT,
                   unsigned int texture_min_filter = GL_NEAREST_MIPMAP_NEAREST,
                   unsigned int texture_mag_filter = GL_NEAREST);
  ~Texture();

  void activate() const;
  void configureShader(Shader &shader);
};

#endif
