/**
 * @file	main.cpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/11
 */

/* -- Includes -- */

#include <string>

#include "application.hpp"
#include "debug.hpp"
#include "window.hpp"

/* -- Namespaces -- */

using namespace std::string_literals;
using namespace lineage;

/* -- Procedure Prototypes -- */

namespace
{
  window create_window();
}

/* -- Procedures -- */

int main(int argc, char** argv)
{
  try
  {
    application app(create_window());
    app.main();
    return 0;
  }
  catch (const std::exception& ex)
  {
    lineage_assert_fail("Uncaught exception! "s + ex.what());
    return 1;
  }
  catch (...)
  {
    lineage_assert_fail("Uncaught exception of unknown type!"s);
    return 2;
  }
}

namespace
{

  /**
   * Creates the main application window.
   */
  window create_window()
  {
    window_args args;
    args.context_version_major = 3;
    args.context_version_minor = 3;
    args.width = 800;
    args.height = 600;
    args.title = "Lineage";
    args.swap_interval = 1;

    return window(args);
  }

}
