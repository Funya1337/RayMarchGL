#ifndef MODEL_CLASS_H
#define MODEL_CLASS_H

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>

#include "Mesh.h"
#include "Camera.h"
#include "./Helper/Shader.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);

class Model
{
public:
  Model(std::string path)
  {
    loadModel(path);
  }
  void Draw(Shader& shader, Camera& camera);

private:
  std::vector<Mesh> meshes;
  std::vector<std::string> loadedTexName;
  std::vector<Texture> loadedTex;
  std::string directory;
  bool checker = false;

  void loadModel(std::string path);
  void processNode(aiNode* node, const aiScene* scene);
  Mesh processMesh(aiMesh* mesh, const aiScene* scene);
  std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};

#endif