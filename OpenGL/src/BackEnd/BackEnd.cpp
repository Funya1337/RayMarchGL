#include "BackEnd.h"
#include "./Helper/Shader.h"
#include "./Helper/VBO.h"
#include "./Helper/VAO.h"
#include "./Helper/EBO.h"

#include "./Helper/GLDebug.h"
#include "Camera.h"

#include <glm/vec2.hpp>

namespace BackEnd {

  GLfloat vertices[] = {
    -1.0f, -1.0f, 0.0f,    1.0f, 0.0f, 0.0f,
     1.0f, -1.0f, 0.0f,    0.0f, 1.0f, 0.0f,
     1.0f,  1.0f, 0.0f,    0.0f, 0.0f, 1.0f,
    -1.0f,  1.0f, 0.0f,    1.0f, 1.0f, 0.0f,
  };

  GLuint indices[] = {
      0, 1, 2,
      0, 2, 3
  };

  float _windowWidth = 2560.0f;
  float _windowHeight = 1440.0f;

  glm::vec2 windowSize = { _windowWidth, _windowHeight };

  void Framebuffer_size_callback(GLFWwindow* window, int width, int height)
  {
    glViewport(0, 0, width, height);
  }

  void mouseEvent(GLFWwindow *window, GLuint uniMouseID)
  {
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
      double mouseX;
      double mouseY;
      glfwGetCursorPos(window, &mouseX, &mouseY);
      glUniform2f(uniMouseID, mouseX, mouseY);
     //std::cout << mouseX << " " << mouseY << std::endl;
    }
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
    {
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
  }

  void Init()
  {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(_windowWidth, _windowHeight, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
      std::cout << "Failed to create GLFW window" << std::endl;
      glfwTerminate();
      return;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, Framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
      std::cout << "Failed to initialize GLAD" << std::endl;
      return;
    }

    Shader shaderProgram("./res/shaders/default.vert", "./res/shaders/default.frag");

    VAO vao;
    vao.Bind();

    VBO vbo(vertices, sizeof(vertices));
    EBO ebo(indices, sizeof(indices));

    vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
    vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    
    vao.Unbind();
    vbo.Unbind();
    ebo.Unbind();

    GLuint uniID = glGetUniformLocation(shaderProgram.ID(), "u_resolution");
    GLuint uniMouseID = glGetUniformLocation(shaderProgram.ID(), "u_mouse");
    GLuint uniTimeID = glGetUniformLocation(shaderProgram.ID(), "u_time");

    Camera camera(_windowWidth, _windowHeight, glm::vec3(0.0f, 0.0f, 2.0f));

    double prevTime = 0.0;
    double crntTime = 0.0;
    double timeDiff;
    unsigned int counter = 0;

    glfwSwapInterval(1);

    while (!glfwWindowShouldClose(window))
    {
      crntTime = glfwGetTime();
      timeDiff = crntTime - prevTime;
      ++counter;
      if (timeDiff >= 1.0 / 30.0)
      {
        std::string FPS = std::to_string((1.0 / timeDiff) * counter);
        std::string ms = std::to_string((timeDiff / counter) * 1000);
        std::string newTitle = "OpenGL - " + FPS + "FPS / " + ms + "ms";
        glfwSetWindowTitle(window, newTitle.c_str());
        prevTime = crntTime;
        counter = 0;
      }

      glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      camera.Inputs(window);
      camera.updateMatrix(90.0f, 0.1f, 100.0f);

      shaderProgram.use();
      glUniform2f(uniID, windowSize.x, windowSize.y);
      glUniform1f(uniTimeID, crntTime);
      camera.sendToShader(shaderProgram);
      //mouseEvent(window, uniMouseID);
      vao.Bind();

      GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));

      glfwSwapBuffers(window);
      glfwPollEvents();
    }

    vao.Delete();
    vbo.Delete();
    ebo.Delete();

    glfwTerminate();
    return;
  }
}