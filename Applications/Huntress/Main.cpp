#include "Huntress.hpp"

/******************************************************************************/
int main(int argc, char * argv[])
{
  try
  {
    /* Create a new huntress object. */
    Luna::FE::Huntress huntress(argc, argv);

    /* Run the huntress application. */
    huntress.run();

  }
  catch (std::exception & ex)
  {
    /* Log the exception. */
   LUNA_LOG_EXCEPTION(ex);

   /* Exit in an error state. */
   return EXIT_FAILURE;
  }

  /* Return an good state. */
  return EXIT_SUCCESS;
}
