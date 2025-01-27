#pragma once
#include "./Helper/VAO.h"
#include "./Helper/EBO.h"
#include "Camera.h"
#include "./Helper/Texture.h"

class Mesh
{
public:
  std::vector <Vertex> vertices;
  std::vector <GLuint> indices;
  std::vector <Texture> textures;

  VAO VAO;
  Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices, std::vector <Texture>& textures);

  void Draw(Shader& shader, Camera& camera);
  void setupMesh();
};

