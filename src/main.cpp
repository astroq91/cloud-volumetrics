#include <array>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <sstream>
#define GLFW_INCLUDE_NONE
#include "glad/glad.h"
#include "linmath.h"
#include <GLFW/glfw3.h>
#include <print>
#include <string>

void GLAPIENTRY openglMessageCallback(GLenum source, GLenum type, GLuint id,
                                      GLenum severity, GLsizei length,
                                      const GLchar *message,
                                      const void *userParam) {
  std::println("OpenGL (id: {0}): {1}", id, message);
}
void glfwErrorCallback(int error, const char *desc) {
  std::println("GLFW error: {0}", desc);
}

struct Vertex {
  vec2 pos;
};

static const std::array<Vertex, 4> vertices{{
    {{-1.0f, -1.0}},
    {{1.0f, -1.0f}},
    {{1.0f, 1.0f}},
    {{-1.0f, 1.0f}},
}};

static const std::array<uint32_t, 6> indices{
    0, 1, 2, 0, 2, 3,
};

std::string load_file(const std::string &path) {
  std::ifstream file(path);
  if (!file) {
    std::println("ifstream");
    exit(EXIT_FAILURE);
  }
  std::stringstream buf;
  buf << file.rdbuf();
  file.close();
  return buf.str();
}

int main() {
  if (!glfwInit()) {
    std::println("glfwInit()");
    exit(EXIT_FAILURE);
  }

  /* Window setup */
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
  GLFWwindow *window =
      glfwCreateWindow(640, 480, "Volumes for days!", NULL, NULL);
  if (!window) {
    std::println("glfwCreateWindow()");
    exit(EXIT_FAILURE);
  }

  /* Context setup */
  glfwMakeContextCurrent(window);
  gladLoadGL();
  glfwSwapInterval(1);

  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(openglMessageCallback, 0);

  /* Initialize the vertex and index buffers */
  GLuint vertex_buffer;
  glCreateBuffers(1, &vertex_buffer);
  glNamedBufferStorage(vertex_buffer, vertices.size() * sizeof(Vertex),
                       vertices.data(), 0);
  GLuint index_buffer;
  glCreateBuffers(1, &index_buffer);
  glNamedBufferStorage(index_buffer, indices.size() * sizeof(uint32_t),
                       indices.data(), 0);

  GLuint vertex_array;
  glGenVertexArrays(1, &vertex_array);
  glBindVertexArray(vertex_array);

  /* Provide layout information */
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
  glEnableVertexAttribArray(0);

  /* Shader initialization */
  std::string vertex_shader_src = load_file("shader.vert");
  auto vertex_shader_src_ptr = vertex_shader_src.c_str();
  std::string fragment_shader_src = load_file("shader.frag");
  auto fragment_shader_src_ptr = fragment_shader_src.c_str();

  std::println("{0}", vertex_shader_src);

  GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, &vertex_shader_src_ptr, NULL);
  glCompileShader(vertex_shader);

  GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, &fragment_shader_src_ptr, NULL);
  glCompileShader(fragment_shader);

  /* Program creation */
  GLuint program = glCreateProgram();
  glAttachShader(program, vertex_shader);
  glAttachShader(program, fragment_shader);
  glLinkProgram(program);

  /* Main loop */
  while (!glfwWindowShouldClose(window)) {
    /* Set the viewport */
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    const float ratio = width / (float)height;
    glViewport(0, 0, width, height);

    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(program);
    glBindVertexArray(vertex_array);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);
  glfwTerminate();
}
