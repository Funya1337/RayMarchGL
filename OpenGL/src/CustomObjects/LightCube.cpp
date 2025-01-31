#include "LightCube.h"

LightCube::LightCube(glm::vec4 color, glm::vec3 position)
{
  this->color = color;
  this->position = position;
  
  // create mesh with smart pointer
  this->meshPtr = std::make_unique<Mesh>(this->lightVertices, this->lightIndices, this->lightTextures);
}

void LightCube::PassModelToShader(Shader& shaderProgram)
{
  this->model = glm::translate(this->model, this->position);
  shaderProgram.use();
  glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID(), "model"), 1, GL_FALSE, glm::value_ptr(this->model));
  glUniform4f(glGetUniformLocation(shaderProgram.ID(), "lightColor"), this->color.x, this->color.y, this->color.z, this->color.w);
}

void LightCube::UpdateColor(Shader& shaderProgramLight, Shader& shaderProgramObject, glm::vec4 newColor)
{
  this->color = newColor;
  shaderProgramLight.use();
  glUniform4f(glGetUniformLocation(shaderProgramLight.ID(), "lightColor"), this->color.x, this->color.y, this->color.z, this->color.w);

  shaderProgramObject.use();
  glUniform4f(glGetUniformLocation(shaderProgramObject.ID(), "lightColor"), this->color.x, this->color.y, this->color.z, this->color.w);
}

void LightCube::UpdatePosition(Shader& shaderProgramLight, Shader& shaderProgramObject, glm::vec3 position)
{
  this->position = position;
  this->model = glm::mat4(1.0f);
  this->model = glm::translate(this->model, this->position);
  shaderProgramLight.use();
  glUniformMatrix4fv(glGetUniformLocation(shaderProgramLight.ID(), "model"), 1, GL_FALSE, glm::value_ptr(this->model));

  shaderProgramObject.use();
  glUniform3f(glGetUniformLocation(shaderProgramObject.ID(), "lightPos"), this->position.x, this->position.y, this->position.z);
}

void LightCube::Draw(Shader& shader, Camera& camera)
{
  this->meshPtr->Draw(shader, camera);
}

glm::vec4 LightCube::getColor()
{
  return this->color;
}
glm::vec3 LightCube::getPosition()
{
  return this->position;
}
glm::mat4 LightCube::getModel()
{
  return this->model;
}