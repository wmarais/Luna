#include <iostream>
#include <signal.h>
#include "BackEnd.hpp"

#include "../../Include/Luna/Common/RPC.hpp"

using namespace Luna;
using namespace Luna::BE;

volatile sig_atomic_t fExecuting = 1;

/******************************************************************************/
void gLUNA_BackEndTerminate(int sig)
{
  LUNA_UNUSED_PARAM(sig);
  fExecuting = 0;
}

/******************************************************************************/
int add(int x, int y)
{
  return x + y;
}

/******************************************************************************/
int main(int argc, char * argv[])
{
//  RPC rpc;

//  /* Add a handler function. The parrameters are automatically extracted by the
//   * template. */
//  rpc.registerHandler("add", add);

//  /* Execute the function on the remote end. This function is asyncronys / non
//   * blocking. */
//  auto id = rpc.invokeStart<int, int, int>("add", 2, 7);

//  /* Processess the commands. */
//  rpc.update();

//  /* Get the result of the RPC. */
//  auto result = rpc.invokeFinished<int>(id);

//  if(result)
//  {
//    /* Print the result. */
//    std::cout << "2+7 = " << result.value() << std::endl;

//  }
  return EXIT_SUCCESS;
}

/******************************************************************************/
//int main(int argc, char * argv[])
//{
//  /* The exit status. */
//  int status = EXIT_SUCCESS;

//  /* Listen to for Ctrl + C signals. */
//  signal(SIGINT, gLUNA_BackEndTerminate);

//  try
//  {
//    LUNA_LOG_INFO("Starting Luna Back End ...");

//    Session session;

//    /* Wait for execution to finish. */
//    while(fExecuting)
//    {
//      session.update();
//      std::this_thread::yield();
//    }

//    /* Take and save the screen buffer. */

//    LUNA_LOG_INFO("Stopping LunaEarth ...");
//  }
//  catch(std::exception & ex)
//  {
//    LUNA_LOG_EXCEPTION(ex);

//    /* Indicate a failure exit state. */
//    status = EXIT_FAILURE;
//  }

//  /* Return the exit code. */
//  return status;
//}


//int main(int argc, char * argv[])
//{
//  /* The exit status. */
//  int status = EXIT_SUCCESS;

//  /* Listen to for Ctrl + C signals. */
//  signal(SIGINT, gLUNA_BackEndTerminate);

//  try
//  {
//    LUNA_LOG_INFO("Starting LunaEarth ...");

//    /* Create the earth object. */
//    Luna::BE::BackEnd be;;

//    /* Run the server. */
//    be.run(argc, argv);

//    /* Wait for execution to finish. */
//    while(fExecuting)
//    {
//      std::this_thread::yield();
//    }

//    LUNA_LOG_INFO("Stopping LunaEarth ...");
//  }
//  catch(std::exception & ex)
//  {
//    LUNA_LOG_EXCEPTION(ex);

//    /* Indicate a failure exit state. */
//    status = EXIT_FAILURE;
//  }

//  /* Return the exit code. */
//  return status;
//}
