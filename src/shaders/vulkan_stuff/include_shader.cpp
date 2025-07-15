#include "include_shader.hpp"
#include "utils.h"

namespace shader {

Shader::Shader(const std::string& vertPath, const std::string& fragPath) 
{
  createGraphicsPipeline(vertPath, fragPath);
}

std::vector<char> Shader::readFile(const std::string& filepath) 
{
  std::ifstream file{filepath, std::ios::ate | std::ios::binary};

  if (!file.is_open()) {
    throw std::runtime_error("failed to open file: " + filepath);
  }

  size_t fileSize = static_cast<size_t>(file.tellg());
  std::vector<char> buffer(fileSize);

  file.seekg(0);
  file.read(buffer.data(), fileSize);

  file.close();
  return buffer;
}

void Shader::createGraphicsPipeline(
    const std::string& vertPath, const std::string& fragPath) 
{
  auto vertCode = readFile(vertPath);
  auto fragCode = readFile(fragPath);

  std::cout << "Vertex Shader Code Size: " << vertCode.size() << '\n';
  std::cout << "Fragment Shader Code Size: " << fragCode.size() << '\n';
}

}  // namespace lve
