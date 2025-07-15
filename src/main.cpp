#include "deps.h"
#include "utils.h"

int main() {
  brainfuck::HandleWindow app;
  Shader("src/shaders/vulkan_stuff/vulkan.vert", "src/shaders/vulkan_stuff/vulkan.frag");

  try 
  {
    app.start();
  } 
  catch (const std::exception &e) 
  {
    std::cerr << e.what() << '\n';
    return 1;
  }

  return 0;
}

