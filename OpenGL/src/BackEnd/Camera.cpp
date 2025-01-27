#include"Camera.h"

Camera::Camera(int width, int height, glm::vec3 position)
{
  Camera::width = width;
  Camera::height = height;
  Position = position;
}

void Camera::updateMatrix(float FOVdeg, float nearPlane, float farPlane)
{
  glm::mat4 view = glm::mat4(1.0f);
  glm::mat4 projection = glm::mat4(1.0f);

  view = glm::lookAt(Position, Position + Orientation, Up);
  projection = glm::perspective(glm::radians(FOVdeg), width / float(height), nearPlane, farPlane);

  cameraMatrix = projection * view;
}

void Camera::Matrix(Shader& shader, const char* uniform)
{
  glUniformMatrix4fv(glGetUniformLocation(shader.ID(), uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

void Camera::updateFirstPersonPosition()
{
  Position = glm::vec3(0.0f, 1.0f, 0.0f);
}

void Camera::Inputs(GLFWwindow* window)
{
  if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS)
  {
    checker = true;
  }
  if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_RELEASE && checker)
  {
    isFirstPerson = !isFirstPerson;
    if (isFirstPerson) updateFirstPersonPosition();
    checker = false;
  }
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
  {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    isMouseHidden = false;
  }
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
  {
    if (isFirstPerson)
      Position += glm::vec3(Orientation.x, 0, Orientation.z) * speed;
    else
      Position += speed * Orientation;
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
  {
    Position += speed * -glm::normalize(glm::cross(Orientation, Up));
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
  {
    if (isFirstPerson)
      Position += glm::vec3(-Orientation.x, 0, -Orientation.z) * speed;
    else
      Position += speed * -Orientation;
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
  {
    Position += speed * glm::normalize(glm::cross(Orientation, Up));
  }

  // turn off for first person
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !isFirstPerson)
  {
    Position += speed * Up;
  }
  if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS && !isFirstPerson)
  {
    Position += speed * -Up;
  }
  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
  {
    speed = 0.1f;
  }
  else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
  {
    speed = 0.05f;
  }

  if (isMouseHidden)
  {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
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
}