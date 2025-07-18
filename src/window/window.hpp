#include "utils.h"
#include "deps.h"

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
      /* CREATING INSTANCE */
      // --------------------
      VkApplicationInfo appInfo{};
      appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
      appInfo.pApplicationName = "triangle";
      appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
      appInfo.pEngineName = "none";
      appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
      appInfo.apiVersion = VK_API_VERSION_1_0;

      VkInstanceCreateInfo createInfo{};
      createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
      createInfo.pApplicationInfo = &appInfo;

      uint32_t glfwExtensionCount = 0;
      const char** glfwExtensions;

      glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

      createInfo.enabledExtensionCount = glfwExtensionCount;
      createInfo.ppEnabledExtensionNames = glfwExtensions;

      createInfo.enabledLayerCount = 0;

      std::cout << "\n";
      if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) 
      {
          throw std::runtime_error("failed to create instance!");
      }
      // --------------------

      /* EXTENSION COUNT */
      // ------------------ 
      uint32_t extensionCount = 0;
      vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
      std::vector<VkExtensionProperties> extensions(extensionCount);
      vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
      std::cout << "available extensions:\n";

      for (const auto& extension : extensions) 
      {
        std::cout << '\t' << extension.extensionName << '\n';
      }
      // -------------------
    }

    void mainLoop()
    {
      while (!glfwWindowShouldClose(window)) 
      {
        processInput(window);
        glfwPollEvents();
        
      }
    }

    void cleanme()
    {
      vkDestroyInstance(instance, nullptr);
      glfwDestroyWindow(window);

      glfwTerminate();
    }
    
    void processInput(GLFWwindow *window)
    {
      if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      {
        glfwSetWindowShouldClose(window, true);
      }
    }

  };
}


