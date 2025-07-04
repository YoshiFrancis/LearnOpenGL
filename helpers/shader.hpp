#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h> // include glad to get all the required OpenGL headers

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

class Shader {
public:
  // the program ID
  unsigned int ID;

  // constructor reads and builds the shader
  Shader(const char *vertexPath, const char *fragmentPath);

  ~Shader();
  // use/activate the shader
  void use();
  // utility uniform functions
  void setBool(const std::string &name, bool value) const;
  void setInt(const std::string &name, int value) const;
  void setFloat(const std::string &name, float value) const;
  void setMat4(const std::string &name, glm::mat4 &transform) const;

private:
  void checkCompileErrors(unsigned int shader, std::string type);
};

#endif
