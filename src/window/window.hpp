#include "utils.h"
#include "deps.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_vulkan.h"

// Volk headers
#ifdef IMGUI_IMPL_VULKAN_USE_VOLK
#define VOLK_IMPLEMENTATION
#include <volk/volk.h>
#endif

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

/* VALIDATION LAYERS */
// -----------------
const std::vector<const char*> validationLayers = 
{
  "VK_LAYER_KHRONOS_validation"
};

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) 
{
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) 
    {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else 
    {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) 
{
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) 
    {
        func(instance, debugMessenger, pAllocator);
    }
}
// -----------------------------

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

    void imgui_usage()
    {
      ImGui::CreateContext();
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
      imgui_usage();

      while (!glfwWindowShouldClose(window)) 
      {
        processInput(window);
        glfwPollEvents();
        
        ImGui::Begin("Window A");
        ImGui::Text("This is window A");
        ImGui::End();
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


