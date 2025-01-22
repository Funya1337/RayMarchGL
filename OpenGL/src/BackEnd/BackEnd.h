#pragma once

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace BackEnd {
  void Init();
  void Framebuffer_size_callback(GLFWwindow* window, int width, int height);
}