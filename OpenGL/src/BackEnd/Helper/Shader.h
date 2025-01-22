#pragma once
#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
  Shader(const char* vShaderPath, const char* fShaderPath);
  ~Shader();
  void use();
  unsigned int ID();

private:
  unsigned int programID;
  void errorHandler(unsigned int shader, std::string type);
  std::string getShaderCode(const char* shaderPath);
};