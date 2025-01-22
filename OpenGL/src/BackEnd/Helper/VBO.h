#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <vector>

class VBO
{
public:
  GLuint ID;
  VBO(GLfloat* vertices, GLsizeiptr size);

  void Bind();
  void Unbind();
  void Delete();
};

#endif