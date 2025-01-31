#include "BackEnd.h"
#include "Model.h"
#include "../CustomObjects/LightCube.h"

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

  Vertex lightVertices[] =
  { //     COORDINATES     //
    Vertex{glm::vec3(-0.1f, -0.1f,  0.1f)},
    Vertex{glm::vec3(-0.1f, -0.1f, -0.1f)},
    Vertex{glm::vec3(0.1f, -0.1f, -0.1f)},
    Vertex{glm::vec3(0.1f, -0.1f,  0.1f)},
    Vertex{glm::vec3(-0.1f,  0.1f,  0.1f)},
    Vertex{glm::vec3(-0.1f,  0.1f, -0.1f)},
    Vertex{glm::vec3(0.1f,  0.1f, -0.1f)},
    Vertex{glm::vec3(0.1f,  0.1f,  0.1f)}
  };

  GLuint lightIndices[] =
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
    //Model backpack("./res/models/backpack/backpack.obj");
    //Model AKM("./res/models/AK47/AK47.obj");
    //Model fpsarms("./res/models/fpsarms/fpsarms.obj");

    // color, position arguments
    LightCube lightcube(glm::vec4(0.0f, 1.0f, 1.0f, 1.0f), glm::vec3(0.5f, 0.5f, 0.5f));

    Texture textures[]
    {
      Texture("./res/textures/planks.png", "diffuse", 0),
      Texture("./res/textures/planksSpec.png", "specular", 1)
    };

    Shader shaderProgram("./res/shaders/default.vert", "./res/shaders/default.frag");
    Shader shaderProgramLight("./res/shaders/light.vert", "./res/shaders/light.frag");
    
    std::vector<Vertex> verts(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
    std::vector<GLuint> ind(indices, indices + sizeof(indices) / sizeof(GLuint));
    std::vector<Texture> tex(textures, textures + sizeof(textures) / sizeof(Texture));
    Mesh floor(verts, ind, tex);

    lightcube.PassModelToShader(shaderProgramLight);

    glm::mat4 modelUniform = glm::mat4(1.0f);
    modelUniform = glm::translate(modelUniform, {0.0f, 0.0f, 0.0f});

    shaderProgramLight.use();
    glUniformMatrix4fv(glGetUniformLocation(shaderProgramLight.ID(), "model"), 1, GL_FALSE, glm::value_ptr(lightcube.getModel()));
    glUniform4f(glGetUniformLocation(shaderProgramLight.ID(), "lightColor"), 
                                                              lightcube.getColor().x,
                                                              lightcube.getColor().y,
                                                              lightcube.getColor().z,
                                                              lightcube.getColor().w);

    shaderProgram.use();
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID(), "model"), 1, GL_FALSE, glm::value_ptr(modelUniform));
    glUniform4f(glGetUniformLocation(shaderProgram.ID(), "lightColor"),
      lightcube.getColor().x,
      lightcube.getColor().y,
      lightcube.getColor().z,
      lightcube.getColor().w);
    glUniform3f(glGetUniformLocation(shaderProgram.ID(), "lightPos"), lightcube.getPosition().x, lightcube.getPosition().y, lightcube.getPosition().z);

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

      lightcube.UpdateColor(shaderProgramLight, shaderProgram, glm::vec4(0.0f, abs(sin(crntTime)), abs(cos(crntTime)), 1.0f));
      

      float radius = 1.0f;   // Radius of the circular motion
      float speed = 1.0f;    // Speed of rotation
      float time = glfwGetTime();  // Get elapsed time

      // Compute new position
      glm::vec3 newPosition;
      newPosition.x = radius * cos(speed * time);  // Circular motion in X
      newPosition.y = lightcube.getPosition().y;  // Keep the same Y height
      newPosition.z = radius * sin(speed * time);  // Circular motion in Z

      // Update position
      lightcube.UpdatePosition(shaderProgramLight, shaderProgram, newPosition);
      
      
      //lightColor = glm::vec4(0.0f, abs(sin(crntTime)), abs(cos(crntTime)), 1.0f);

      //shaderProgramLight.use();
      //glUniform4f(glGetUniformLocation(shaderProgramLight.ID(), "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);

      //shaderProgram.use();
      //glUniform4f(glGetUniformLocation(shaderProgram.ID(), "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);





      //planeModel = glm::rotate(planeModel, rotationAngle, rotationAxis);

      //shaderProgram.use();
      //glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID(), "model"), 1, GL_FALSE, glm::value_ptr(planeModel));

      //if (camera.isFirstPerson)
      //{
      //  camera.adjustFirstPersonCameraPosition(window);
      //  fpsArms.modelUniform = glm::mat4(1.0f);
      //  fpsArms.modelUniform = glm::translate(fpsArms.modelUniform, camera.Position - glm::vec3(camera.fpsCameraOffsetX, camera.fpsCameraOffsetY, camera.fpsCameraOffsetZ));

      //  glm::vec3 forward = glm::normalize(camera.Orientation);
      //  glm::vec3 right = glm::normalize(glm::cross(forward, camera.Up));
      //  glm::vec3 up = glm::normalize(glm::cross(right, forward));

      //  glm::mat4 rotationMatrix = glm::mat4(1.0f);
      //  rotationMatrix[0] = glm::vec4(right, 0.0f);  // X-axis
      //  rotationMatrix[1] = glm::vec4(up, 0.0f);     // Y-axis
      //  rotationMatrix[2] = glm::vec4(forward, 0.0f); // Z-axis (inverted)

      //  fpsArms.modelUniform *= rotationMatrix;

      //  shaderProgram.use();
      //  glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID(), "model"), 1, GL_FALSE, glm::value_ptr(fpsArms.modelUniform));
      //}

      floor.Draw(shaderProgram, camera);
      //fpsarms.Draw(shaderProgram, camera);
      lightcube.Draw(shaderProgramLight, camera);

      //ground.Draw(shaderProgram, camera);

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