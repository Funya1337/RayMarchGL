#pragma once

#include <iostream>
#include <GLFW/glfw3.h>

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLDebug::GLClearError();\
  x;\
  ASSERT(GLDebug::GLLogCall(#x, __FILE__, __LINE__))

namespace GLDebug {
  void GLClearError();
  bool GLLogCall(const char* function, const char* file, int line);
};