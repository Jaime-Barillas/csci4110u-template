#include <memory>
#include <stdexcept>

// Use glad headers.
#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#undef GLAD_GL_IMPLEMENTATION
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>
#include <spdlog/logger.h>
#include <spdlog/common.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

#include "window.hpp"
#include "shader_manager.hpp"

class Program : public Window {
  ShaderManager shader_manager;
  GLuint vbo = 0;
  GLuint vao = 0;

  float screen_quad[6 * 3] {
    -1.0f, -1.0f, 0.0f,
     1.0f,  1.0f, 0.0f,
    -1.0f,  1.0f, 0.0f,

    -1.0f, -1.0f, 0.0f,
     1.0f, -1.0f, 0.0f,
     1.0f,  1.0f, 0.0f,
};

  static void framebufferResized(GLFWwindow *window, int width, int height) {
    auto prog = (Program*)glfwGetWindowUserPointer(window);
    glViewport(0, 0, width, height);
  }

public:
  Program() : Window() {};
  Program(WindowOpts opts) : Window(opts) {
    glfwSetWindowUserPointer(ptr, this);
    glfwSetFramebufferSizeCallback(ptr, framebufferResized);

    glViewport(0, 0, opts.width, opts.height);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(screen_quad), screen_quad, GL_STATIC_DRAW);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    shader_manager.compileAndWatch({
      .name = "scene",
      .shaders = {
        Shader{.path = "shaders/vert.glsl", .type = GL_VERTEX_SHADER},
        Shader{.path = "shaders/frag.glsl", .type = GL_FRAGMENT_SHADER}
      }
    });
  };

  void handleInput(int key) override {

  }

  void draw() override {
    shader_manager.recompilePending();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shader_manager.get("scene"));
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
  }
};

int main() {
  auto console_target = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
  auto file_target = std::make_shared<spdlog::sinks::basic_file_sink_mt>("log.txt", true);
  spdlog::sinks_init_list targets = {file_target, console_target};
  auto logger = std::make_shared<spdlog::logger>("logger", targets);
  logger->set_level(spdlog::level::trace);
  spdlog::register_logger(logger);
  spdlog::set_default_logger(logger);

  spdlog::info("//-------------------------//");
  spdlog::info("//        CSCI4110U        //");
  spdlog::info("//-------------------------//");

  Program *window;
  try {
    window = new Program({.width = 1152, .height = 720, .title = "RayMarcher - SDF"});
    window->run();
  } catch(std::runtime_error &err) {
    return -1;
  }
}
