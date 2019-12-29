#include "Huntress.hpp"

using namespace Luna;
using namespace Luna::FE;

/******************************************************************************/
Huntress::Huntress(int argc, char * argv[])
{
  /* These parameters will be used later. */
  LUNA_UNUSED_PARAM(argc);
  LUNA_UNUSED_PARAM(argv);

  /* Create the new window. */
  fWindow = std::make_unique<Window>();
}

/******************************************************************************/
void Huntress::run()
{

}
