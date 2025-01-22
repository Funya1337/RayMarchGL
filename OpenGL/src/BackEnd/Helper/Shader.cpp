#include "Shader.h"

unsigned int Shader::ID()
{
  return programID;
}

Shader::~Shader()
{
  glDeleteProgram(programID);
}

void Shader::use()
{
  glUseProgram(programID);
}

void Shader::errorHandler(unsigned int shader, std::string type)
{
  char logInfo[1024];
  int res;
  if (type == "PROGRAM")
  {
    glGetProgramiv(shader, GL_LINK_STATUS, &res);
    if (res == 0)
    {
      glGetProgramInfoLog(shader, 1000, NULL, logInfo);
      std::cout << "Shader linking error: " << logInfo << std::endl;
    }
  }
  else
  {
    glGetShaderiv(shader, GL_COMPILE_STATUS, &res);
    if (res == 0)
    {
      glGetShaderInfoLog(shader, 1000, NULL, logInfo);
      std::cout << "Shader compilation error : " << logInfo << std::endl;
    }
  }
}

Shader::Shader(const char* vShaderPath, const char* fShaderPath)
{
  std::string vTmpShaderCode = getShaderCode(vShaderPath);
  std::string fTmpShaderCode = getShaderCode(fShaderPath);
  const char* vShaderCode = vTmpShaderCode.c_str();
  const char* fShaderCode = fTmpShaderCode.c_str();

  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vShaderCode, NULL);
  glCompileShader(vertexShader);
  errorHandler(vertexShader, "SHADER");

  unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
  glCompileShader(fragmentShader);
  errorHandler(fragmentShader, "SHADER");

  programID = glCreateProgram();
  glAttachShader(programID, vertexShader);
  glAttachShader(programID, fragmentShader);
  glLinkProgram(programID);
  errorHandler(programID, "PROGRAM");

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}

std::string Shader::getShaderCode(const char* shaderPath)
{
  std::string text, line;
  std::ifstream in(shaderPath);
  while (std::getline(in, line))
  {
    text += line + "\n";
  }
  return text;
}