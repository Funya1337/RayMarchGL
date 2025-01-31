#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "./Helper/Shader.h"

class Camera
{
public:
  glm::vec3 Position;
  glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
  glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
  glm::mat4 cameraMatrix = glm::mat4(1.0f);

  bool isMouseHidden = true;
  bool isFirstPerson = false;
  bool checker = false;

  float fpsCameraOffsetX = 0.15f;
  float fpsCameraOffsetY = 0.08f;
  float fpsCameraOffsetZ = 0.17f;

  int width;
  int height;

  float speed = 1.0f;
  float sensitivity = 70.0f;

  Camera(int width, int height, glm::vec3 position);

  void updateMatrix(float FOVdeg, float nearPlane, float farPlane);
  void Matrix(Shader& shader, const char* uniform);
  void adjustFirstPersonCameraPosition(GLFWwindow *window);
  void Inputs(GLFWwindow* window);
  void updateFirstPersonPosition();
};

