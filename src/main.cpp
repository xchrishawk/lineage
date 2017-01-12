/**
 * @file	main.cpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/11
 */

/* -- Includes -- */

#include <string>
#include <utility>

#include "application.hpp"
#include "debug.hpp"
#include "input_manager.hpp"
#include "opengl.hpp"
#include "window.hpp"

/* -- Namespaces -- */

using namespace std::string_literals;
using namespace lineage;

/* -- Procedure Prototypes -- */

namespace
{
  window create_window();
  opengl create_opengl();
  input_manager create_input_manager();
}

/* -- Procedures -- */

int main(int argc, char** argv)
{
  try
  {
    lineage::window window = create_window();
    lineage::opengl opengl = create_opengl();
    lineage::input_manager input_manager = create_input_manager();

    application app(std::move(window),
                    std::move(opengl),
                    std::move(input_manager));
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
    args.context_version_minor = 2;
    args.context_profile = GLFW_OPENGL_CORE_PROFILE;
    args.context_forward_compatibility = true;

    args.width = 800;
    args.height = 600;
    args.title = "Lineage";
    args.swap_interval = 1;

    return window(args);
  }

  /**
   * Creates the OpenGL interface.
   */
  opengl create_opengl()
  {
    return opengl();
  }

  /**
   * Creates the input manager.
   */
  input_manager create_input_manager()
  {
    return input_manager();
  }

}
