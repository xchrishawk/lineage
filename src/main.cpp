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
#include "default_render_manager.hpp"
#include "default_state_manager.hpp"
#include "input_manager.hpp"
#include "opengl.hpp"
#include "prototype_render_manager.hpp"
#include "prototype_state_manager.hpp"
#include "render_manager.hpp"
#include "state_manager.hpp"
#include "window.hpp"

/* -- Namespaces -- */

using namespace std::string_literals;
using namespace lineage;

/* -- Procedure Prototypes -- */

namespace
{
  void run_application();
}

/* -- Procedures -- */

int main(int argc, char** argv)
{
  try
  {
    run_application();
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
   * Runs an instance of the application.
   */
  void run_application()
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

    lineage::window window { args };
    lineage::opengl opengl { };
    lineage::input_manager input_manager { window };

#if defined(LINEAGE_PROTOTYPE)
    lineage::prototype_state_manager state_manager { input_manager };
    lineage::prototype_render_manager render_manager { opengl, state_manager };
#else
    lineage::default_state_manager state_manager { input_manager };
    lineage::default_render_manager render_manager { opengl, state_manager };
#endif

    application app { window, opengl, input_manager, state_manager, render_manager };
    app.main();
  }

}
