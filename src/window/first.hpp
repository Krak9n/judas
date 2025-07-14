#include "window.hpp"

namespace brainfuck
{
  class first
  {
  public:
    static constexpr int WIDTH = 700;
    static constexpr int HEIGHT = 500;

    void run();
  private:
    HandleWindow wiwi{WIDTH, HEIGHT, "FUCK"};
  };
}
