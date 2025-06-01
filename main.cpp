#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>


constexpr int WIDTH = 800;
constexpr int HEIGHT = 600;

const char* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char* fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n""
    "void main()\n""
    "{\n""
      "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n""
      "}\n""
      "The fragment\0";

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

char* check_shader_compile(unsigned int shader) {
  int  success;
  char infoLog[512];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

  if(!success)
  {
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    return infoLog;
  }
  return nullptr;
}

char* check_program_link(unsigned int program) {
  int  success;
  char infoLog[512];
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if(!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    return infoLog;
  }
  return nullptr
}

void hello_triangle() {
  float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
  };  
  unsigned int VBO;

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPoint(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
  glEnableVertexAttribArray(0);

  unsigned int vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
  glCompileShader(vertexShader);

  char* infoLog = check_shader_compile(vertexShader);
  if (infoLog != nullptr) {
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    exit(1);
  }

  unsigned int fragmentShader;
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
  glCompileShader(fragmentShader);

  infoLog = check_shader_compile(vertexShader);
  if (infoLog != nullptr) {
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    exit(1);
  }


  unsigned int shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  infoLog = check_program_link(shaderProgram);
  if (infoLog != nullptr) {
    std::cout << "ERROR::SHADER::PROGRAM::LINKER_FAILED\n" << infoLog << std::endl;
    exit(1);
  }

  glUseProgram(shaderProgram);
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);


}

int main() {


  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  
  GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", NULL, NULL);
  if (window == NULL)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  float x = 0.0f;
  float increment = 1;
  while(!glfwWindowShouldClose(window))
  {
    // input
    processInput(window);

    // rendering
    

    glClearColor(x, 0.3f, 0.3f, 1.0f);
    x += increment * .01;
    if (x >= 1.0f || x <= 0.0f) {
      increment *= -1;
    }
    std::cout << x << "\n";
    glClear(GL_COLOR_BUFFER_BIT);

    // events and swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();    
  }

  glfwTerminate();

  return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}
