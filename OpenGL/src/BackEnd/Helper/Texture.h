#pragma once

#include <glad/glad.h>
#define STBI_FAILURE_USERMSG
#include <stb_image.h>

#include "Shader.h"

class Texture
{
public:
  GLuint ID;
  const char* type;
  std::string path;
  GLuint unit;
  Texture(const char* image, const char* texType, GLenum slot);

  void texUnit(Shader& shader, const char* uniform, GLuint unit);
  
  void Bind();
  
  void UnBind();

  void Delete();
};