#include "utils.h"

namespace shader 
{
  class Shader 
  {
  public:
    Shader(const std::string& vertPath, const std::string& fragPath);

  private:
    static std::vector<char> readFile(const std::string& filepath);

    void createGraphicsPipeline(const std::string& vertPath, const std::string& fragPath);
  };
}  // namespace lve
