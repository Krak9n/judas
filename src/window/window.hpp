#include "utils.h"

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

namespace brainfuck
{
  class HandleWindow
  {
  public:
    void start()
    {
      initWindow();
      initVulkan();
      createInstance();
      mainLoop();
      cleanme();
    }

  private:
    GLFWwindow *window;
    VkInstance instance;
    
    void initWindow()
    {
      glfwInit();
      
      glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
      glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
      
      window = glfwCreateWindow(
          WIDTH, 
          HEIGHT, 
          "fuck me", 
          nullptr, 
          nullptr);

    }

    void initVulkan()
    {
      createInstance();
    }

    void createInstance()
    {
      VkApplicationInfo appInfo{};
      appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
      appInfo.pApplicationName = "triangle";
      appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
      appInfo.pEngineName = "none";
      appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
      appInfo.apiVersion = VK_API_VERSION_1_0;
    }

    void mainLoop()
    {
      while (!glfwWindowShouldClose(window)) 
      {
            glfwPollEvents();
      }
    }

    void cleanme()
    {
      glfwDestroyWindow(window);

      glfwTerminate();
    }
  };
}


