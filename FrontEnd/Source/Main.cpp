#include "../../Include/Luna/Common.hpp"

using namespace Luna;

int main(int argc, char * argv[])
{
  LUNA_UNUSED_PARAM(argc);
  LUNA_UNUSED_PARAM(argv);


  Rect<int64_t> rect1(0, 0, 100, 100);
  Rect<int64_t> rect2(50, 50, 100, 100);
  auto rect3 = rect1.intersect(rect2);

  std::cout << "Rect1: " << rect1 << std::endl;
  std::cout << "Rect2: " << rect2 << std::endl;
  if(rect3)
  {
    std::cout << "Rect3: " << rect3.value() << std::endl;
  }

  return 0;
}
