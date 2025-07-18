#include "utils.h"
#include "deps.h"

#include "window/window.hpp"
#include "shaders/vulkan_stuff/include_shader.hpp"

int main() {
  brainfuck::HandleWindow app;
  shader::Shader myShader("src/shaders/vulkan_stuff/vulkan.vert.spv", "src/shaders/vulkan_stuff/vulkan.frag.spv");
  
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
