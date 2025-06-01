#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include "camera.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "snake_game_3d.hpp"

constexpr int WIDTH = 800;
constexpr int HEIGHT = 600;
float opacity = 1.0f;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
bool firstMouse = true;
float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

int main() {

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  GLFWwindow *window =
      glfwCreateWindow(WIDTH, HEIGHT, "Hello Triangle", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window\n";
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "failed to initialize GLAD\n";
    return -1;
  }

  char *infoLog = nullptr;
  Shader shader("../shader.vs", "../shader.fs");
  Texture textures({"../container.jpg", "../awesomeface.png"}, GL_TEXTURE_2D,
                   GL_REPEAT, GL_REPEAT);
  textures.setTextureParams(1);
  textures.configureShader(shader);

  float vertices[] = {
      // positions         // texcoords
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // 0
      0.5f,  -0.5f, -0.5f, 1.0f, 0.0f, // 1
      0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, // 2
      -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, // 3
      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, // 4
      0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, // 5
      0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // 6
      -0.5f, 0.5f,  0.5f,  0.0f, 1.0f  // 7
  };

  // with a cube, I can make a head (no front), tail (no back), and body (no
  // front or back)
  unsigned int cube_indicies[] = {
      4, 5, 6, 6, 7, 4, // front
      4, 7, 3, 3, 0, 4, // left
      1, 5, 6, 6, 2, 1, // right
      3, 2, 6, 6, 7, 3, // top
      4, 0, 1, 1, 5, 4, // bottom
      0, 1, 2, 2, 3, 0, // back
  };

  unsigned int VAO, VBO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  // for first triangle
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indicies), cube_indicies,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(2 * sizeof(float)));
  glEnableVertexAttribArray(2);

  glEnable(GL_DEPTH_TEST);
  glm::vec3 cubePositions[] = {
      glm::vec3(1.0f, 0.1f, -5.0f),   glm::vec3(2.0f, 5.0f, -15.0f),
      glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
      glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
      glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
      glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};

  while (!glfwWindowShouldClose(window)) {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    processInput(window);
    glClearColor(0.5f, 0.75f, 0.8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    textures.activate();
    shader.use();

    glm::mat4 projection = glm::perspective(
        glm::radians(camera.Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 100.f);
    shader.setMat4("projection", projection);
    glm::mat4 view = camera.GetViewMatrix();
    shader.setMat4("view", view);
    shader.setFloat("opacity", 0.2f);

    glBindVertexArray(VAO);

    for (size_t i = 0; i < 10; ++i) {
      glm::mat4 model{1.0f};
      model = glm::translate(model, cubePositions[i]);
      float angle = 0;
      if (i % 3 == 0)
        angle = 20.0f * (i + 10);
      model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle),
                          glm::vec3(0.5f, 1.0f, 0.0f));
      shader.setMat4("model", model);
      // glDrawArrays(GL_TRIANGLES, 0, 36);
      glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // cleanup
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);

  glfwTerminate();
  return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  const float cameraSpeed = 2.5f * deltaTime; // adjust accordingly
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera.ProcessKeyboard(FORWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera.ProcessKeyboard(BACKWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera.ProcessKeyboard(LEFT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera.ProcessKeyboard(RIGHT, deltaTime);
}
void mouse_callback(GLFWwindow *window, double xposIn, double yposIn) {
  float xpos = static_cast<float>(xposIn);
  float ypos = static_cast<float>(yposIn);

  if (firstMouse) {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  float xoffset = xpos - lastX;
  float yoffset =
      lastY - ypos; // reversed since y-coordinates go from bottom to top

  lastX = xpos;
  lastY = ypos;

  camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
  camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
