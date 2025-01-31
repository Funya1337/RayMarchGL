#pragma once

#include "../BackEnd/Mesh.h"

class LightCube
{
public:
  LightCube(glm::vec4 color, glm::vec3 position);
  void PassModelToShader(Shader &shaderProgram);
  void UpdateColor(Shader& shaderProgramLight, Shader& shaderProgramObject, glm::vec4 newColor);
  void UpdatePosition(Shader& shaderProgram, Shader& shaderProgramObject, glm::vec3 position);
  void Draw(Shader &shader, Camera &camera);

  glm::vec4 getColor();
  glm::vec3 getPosition();
  glm::mat4 getModel();

private:
  
  glm::vec4 color;
  glm::vec3 position;
  glm::mat4 model = glm::mat4(1.0f);

  std::vector<Vertex> lightVertices =
  {
    Vertex{glm::vec3(-0.1f, -0.1f,  0.1f)},
    Vertex{glm::vec3(-0.1f, -0.1f, -0.1f)},
    Vertex{glm::vec3(0.1f, -0.1f, -0.1f)},
    Vertex{glm::vec3(0.1f, -0.1f,  0.1f)},
    Vertex{glm::vec3(-0.1f,  0.1f,  0.1f)},
    Vertex{glm::vec3(-0.1f,  0.1f, -0.1f)},
    Vertex{glm::vec3(0.1f,  0.1f, -0.1f)},
    Vertex{glm::vec3(0.1f,  0.1f,  0.1f)}
  };

  std::vector<GLuint> lightIndices =
  {
    0, 1, 2,
    0, 2, 3,
    0, 4, 7,
    0, 7, 3,
    3, 7, 6,
    3, 6, 2,
    2, 6, 5,
    2, 5, 1,
    1, 5, 4,
    1, 4, 0,
    4, 5, 6,
    4, 6, 7
  };

  std::vector<Texture> lightTextures;
  std::unique_ptr<Mesh> meshPtr;
};

