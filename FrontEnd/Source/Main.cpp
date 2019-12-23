#include "../../Include/Luna/Common.hpp"
#include "../../Include/Luna/FE/Renderer.hpp"

using namespace Luna;
using namespace Luna::FE;

int main(int argc, char * argv[])
{
  LUNA_UNUSED_PARAM(argc);
  LUNA_UNUSED_PARAM(argv);

  Renderer renderer;
  renderer.fill(renderer.rect(), kWhite);

  RectBase<int64_t> rect1(0, 0, 100, 100);
  RectBase<int64_t> rect2(50, 50, 100, 100);
  auto rect3 = rect1.intersect(rect2);

  renderer.fill(rect1, kBlack);
  renderer.fill(rect2, kBlue);

  std::cout << "Rect1: " << rect1 << std::endl;
  std::cout << "Rect2: " << rect2 << std::endl;
  if(rect3)
  {
    std::cout << "Rect3: " << rect3.value() << std::endl;
    renderer.fill(rect3.value(), kRed);
  }

  renderer.savePNG("Test.png");

  return 0;
}
