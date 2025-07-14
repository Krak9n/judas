#include "window.hpp"

namespace brainfuck
{
  HandleWindow::HandleWindow(unsigned int width, unsigned int height, std::string title) : width{width}, height{height}, name{title}
  {
    init();`
  }

  void HandleWindow::init()
  {
    glfwInit();
    // hint for not creating an ogl window
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(
        width,
        height,
        name.c_str(),
        nullptr,
        nullptr);
  }

  void HandleWindow::~HandleWindow()
  {
    glfwDestroyWindow(window);
    glfwTerminate();
  }
};
