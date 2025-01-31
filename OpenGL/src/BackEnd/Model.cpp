#include "Model.h"

void Model::Draw(Shader& shader, Camera& camera)
{
  for (unsigned int i = 0; i < meshes.size(); ++i)
  {
    meshes[i].Draw(shader, camera);
    if (!checker)
    {
      std::cout << meshes[i].textures.size() << std::endl;
      checker = true;
    }
  }
}

void Model::loadModel(std::string path)
{
  Assimp::Importer import;
  const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
  {
    std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
    return;
  }
  directory = path.substr(0, path.find_last_of('/'));

  processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
  for (unsigned int i = 0; i < node->mNumMeshes; i++)
  {
    aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
    std::cout << "Has bones: " << mesh->HasBones() << std::endl;
    meshes.push_back(processMesh(mesh, scene));
  }
  // then do the same for each of its children
  for (unsigned int i = 0; i < node->mNumChildren; i++)
  {
    processNode(node->mChildren[i], scene);
  }
}

void Model::sendModelToShader(Shader& shader, glm::vec3 position)
{
  modelUniform = glm::translate(modelUniform, position);

  shader.use();
  glUniformMatrix4fv(glGetUniformLocation(shader.ID(), "model"), 1, GL_FALSE, glm::value_ptr(modelUniform));
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<Texture> textures;

  // process vertices (parse vertices from .obj file to my own mesh class)
  for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
  {
    Vertex vertex;
    glm::vec3 _vec3;
    _vec3.x = mesh->mVertices[i].x;
    _vec3.y = mesh->mVertices[i].y;
    _vec3.z = mesh->mVertices[i].z;
    vertex.position = _vec3;

    if (mesh->HasNormals())
    {
      _vec3.x = mesh->mVertices[i].x;
      _vec3.y = mesh->mVertices[i].y;
      _vec3.z = mesh->mVertices[i].z;
      vertex.normal = _vec3;
    }

    if (mesh->mTextureCoords[0])
    {
      glm::vec2 _vec2;
      _vec2.x = mesh->mTextureCoords[0][i].x;
      _vec2.y = (1 - mesh->mTextureCoords[0][i].y);
      vertex.texture = _vec2;
    }
    else
      vertex.texture = glm::vec2(0.0f, 0.0f);

    vertices.push_back(vertex);
  }

  // process indices
  for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
  {
    aiFace face = mesh->mFaces[i];
    for (unsigned int j = 0; j < face.mNumIndices; ++j)
      indices.push_back(face.mIndices[j]);
  }

  aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

  std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "diffuse");
  textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

  std::vector<Texture> shininessMaps = loadMaterialTextures(material, aiTextureType_SHININESS, "shininess");
  textures.insert(textures.end(), shininessMaps.begin(), shininessMaps.end());

  //std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "specular");
  //textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

  std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "normal");
  textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

  //std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "height");
  //textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

  Mesh parsedMesh(vertices, indices, textures);

  return parsedMesh;
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
  std::vector<Texture> textures;
  for (unsigned int i = 0; i < mat->GetTextureCount(type); ++i)
  {
    aiString str;
    mat->GetTexture(type, i, &str);
    const char* filename = str.C_Str();
    std::string path = std::string(filename);
    path = directory + '/' + path;

    bool skip = false;
    std::cout << i << " " << loadedTexName.size() << std::endl;
    for (unsigned int j = 0; j < loadedTexName.size(); j++)
    {
      if (loadedTexName[j] == path)
      {
        textures.push_back(loadedTex[j]);
        skip = true;
        break;
      }
    }

    if (!skip)
    {
      std::cout << path << std::endl;
      Texture tex = Texture(path.c_str(), typeName.c_str(), loadedTex.size());
      textures.push_back(tex);
      loadedTex.push_back(tex);
      loadedTexName.push_back(path);
    }
  }
  return textures;
}