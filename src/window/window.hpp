#include "utils.h"

namespace brainfuck
{
  class HandleWindow
  {
  public:
    HandleWindow(int w, int h, std::string title);
    ~HandleWindow();
  
    HandleWindow(const HandleWindow &) = delete;
    HandleWindow &operator=(const HandleWindow &) = delete;

    bool shouldClose() { return glfwWindowShouldClose(window); }
  private:
    void init();

    const int width;
    const int height;

    std::string name;
    GLFWwindow *window;
  };
}
