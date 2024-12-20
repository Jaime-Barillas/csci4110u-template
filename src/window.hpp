#ifndef CSCI_4110U_WINDOW_H
#define CSCI_4110U_WINDOW_H

#define GLFW_INCLUDE_NONE
#include <glad/gl.h>
#include <GLFW/glfw3.h>

struct WindowOpts {
  /* Window Opts */
  int width;
  int height;
  const char *title;

  /* GL Context */
  int glMajor = 3;
  int glMinor = 3;
  int glProfile = GLFW_OPENGL_CORE_PROFILE;
};

class Window {
  static void logError(int error_code, const char *description);
  static void glError(void *ret, const char *name, GLADapiproc proc, int len_args, ...);

  protected:
    GLFWwindow *ptr = nullptr;

    Window();
    Window(WindowOpts opts);

  public:
    ~Window();
    void run();
    virtual void handleInput(int key) = 0;
    virtual void draw() = 0;
};

#endif
