#include "first.hpp"

namespace brainfuck
{
  void first::run() 
  {

    while (!wiwi.shouldClose()) 
    {
      glfwPollEvents();
    }
  }
}
