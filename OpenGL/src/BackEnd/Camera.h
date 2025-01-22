#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "./Helper/Shader.h"

class Camera
{
public:
  float width;
  float height;
  glm::vec3 position;

  glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
  glm::mat4 cameraMatrix = glm::mat4(1.0f);
  glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);

  float speed = 10.0f;
  float sensitivity = 100.0f;
  bool firstClick = true;
  const float FOV = 90.0f;

  Camera(float width, float height, glm::vec3 position);
  void Inputs(GLFWwindow* window);
  void updateMatrix(float FOVdeg, float nearPlane, float farPlane);
  void printVec3(glm::vec3 vec);
  void sendToShader(Shader& shader);

  glm::vec3 getRightVector() const;
  glm::vec3 getUpVector() const;
};

