#include "Camera.h"

Camera::Camera(float width, float height, glm::vec3 position)
{
  Camera::width = width;
  Camera::height = height;
  Camera::position = position;
}

void Camera::updateMatrix(float FOVdeg, float nearPlane, float farPlane)
{
  glm::mat4 view = glm::mat4(1.0f);
  glm::mat4 projection = glm::mat4(1.0f);

  view = glm::lookAt(position, position + Orientation, Up);
  projection = glm::perspective(glm::radians(FOVdeg), width / float(height), nearPlane, farPlane);

  cameraMatrix = projection * view;
}

glm::vec3 Camera::getRightVector() const {
  return glm::normalize(glm::cross(Orientation, Up));
}

glm::vec3 Camera::getUpVector() const {
  return Up;
}

void Camera::Inputs(GLFWwindow* window)
{
  // Handles key inputs
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
  {
    position += speed * Orientation;
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
  {
    position += speed * -glm::normalize(glm::cross(Orientation, Up));
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
  {
    position += speed * -Orientation;
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
  {
    position += speed * glm::normalize(glm::cross(Orientation, Up));
  }
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
  {
    position += speed * Up;
  }
  if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
  {
    position += speed * -Up;
  }
  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
  {
    speed = 0.4f;
  }
  else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
  {
    speed = 0.1f;
  }


  if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
  {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    if (firstClick)
    {
      glfwSetCursorPos(window, (width / 2), (height / 2));
      firstClick = false;
    }

    double mouseX;
    double mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);

    float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
    float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;

    glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));

    if (abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
    {
      Orientation = newOrientation;
    }

    Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);

    glfwSetCursorPos(window, (width / 2), (height / 2));
  }
  else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
  {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    firstClick = true;
  }
}

void Camera::sendToShader(Shader& shader) {
  glUniform3f(glGetUniformLocation(shader.ID(), "uCameraPos"), position.x, position.y, position.z);

  glUniform3f(glGetUniformLocation(shader.ID(), "uCameraDir"), Orientation.x, Orientation.y, Orientation.z);

  glm::vec3 right = getRightVector();
  glm::vec3 up = getUpVector();
  glUniform3f(glGetUniformLocation(shader.ID(), "uCameraRight"), right.x, right.y, right.z);
  glUniform3f(glGetUniformLocation(shader.ID(), "uCameraUp"), up.x, up.y, up.z);
  glUniformMatrix4fv(glGetUniformLocation(shader.ID(), "uCameraMatrix"), 1, GL_FALSE, glm::value_ptr(cameraMatrix));

  glUniform1f(glGetUniformLocation(shader.ID(), "uFOV"), FOV);
}