#include "BackEnd.h"
#include "Model.h"

namespace BackEnd {

  Vertex vertices[] =
  { //               COORDINATES           /            COLORS          /           NORMALS         /       TEXTURE COORDINATES    //
    Vertex{glm::vec3(-1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
    Vertex{glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
    Vertex{glm::vec3(1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
    Vertex{glm::vec3(1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)}
  };

  GLuint indices[] =
  {
    0, 1, 2,
    0, 2, 3
  };

  GLFWwindow* window = nullptr;
  int _windowWidth = 2560.0f;
  int _windowHeight = 1440.0f;

  void Framebuffer_size_callback(GLFWwindow* window, int width, int height)
  {
    glViewport(0, 0, width, height);
  }

  void InitializeProgram()
  {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(_windowWidth, _windowHeight, "LearnOpenGL", NULL, NULL);
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
  }
  
  void MainLoop()
  {
    Model backpack("./res/models/backpack/backpack.obj");

    Texture textures[]
    {
      Texture("./res/textures/planks.png", "diffuse", 0),
      Texture("./res/textures/planksSpec.png", "specular", 0)
    };

    Shader shaderProgram("./res/shaders/default.vert", "./res/shaders/default.frag");
    std::vector<Vertex> verts(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
    std::vector<GLuint> ind(indices, indices + sizeof(indices) / sizeof(GLuint));
    std::vector<Texture> tex(textures, textures + sizeof(textures) / sizeof(Texture));
    Mesh floor(verts, ind, tex);

    glm::vec3 planePos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::mat4 planeModel = glm::mat4(1.0f);
    planeModel = glm::translate(planeModel, planePos);

    glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);

    shaderProgram.use();
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID(), "model"), 1, GL_FALSE, glm::value_ptr(planeModel));

    Camera camera(_windowWidth, _windowHeight, glm::vec3(0.0f, 0.0f, 2.0f));

    double prevTime = 0.0;
    double crntTime = 0.0;
    double timeDiff;
    unsigned int counter = 0;

    glfwSwapInterval(1);

    glEnable(GL_DEPTH_TEST);

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
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      
      camera.Inputs(window);
      camera.updateMatrix(45.0f, 0.1f, 100.0f);

      floor.Draw(shaderProgram, camera);
      backpack.Draw(shaderProgram, camera);

      glfwSwapBuffers(window);
      glfwPollEvents();
    }
    glfwDestroyWindow(window);
  }
  
  void CleanUp()
  {
    glfwTerminate();
  }
}