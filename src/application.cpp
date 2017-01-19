/**
 * @file	application.cpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/11
 */

/* -- Includes -- */

#include <sstream>
#include <utility>

#include "application.hpp"
#include "debug.hpp"
#include "input_manager.hpp"
#include "opengl.hpp"
#include "opengl_error.hpp"
#include "render_manager.hpp"
#include "state_manager.hpp"
#include "util.hpp"
#include "window.hpp"

/* -- Namespaces -- */

using namespace lineage;

/* -- Types -- */

/**
 * Implementation for the `lineage::application` class.
 */
struct application::implementation
{

  /* -- Constructor -- */

  implementation(lineage::window& window,
                 lineage::opengl& opengl,
                 lineage::input_manager& input_manager,
                 lineage::state_manager& state_manager,
                 lineage::render_manager& render_manager)
    : window(window),
      opengl(opengl),
      input_manager(input_manager),
      state_manager(state_manager),
      render_manager(render_manager)
  { }

  /* -- Fields -- */

  lineage::window& window;
  lineage::opengl& opengl;
  lineage::input_manager& input_manager;
  lineage::state_manager& state_manager;
  lineage::render_manager& render_manager;

  /* -- Methods -- */

  /** Handles application input. */
  void do_input()
  {
    window.poll_events();
  }

  /** Updates the state object. */
  void do_state(double abs_t, double delta_t)
  {
    state_args args;
    args.abs_t = abs_t;
    args.delta_t = delta_t;

    state_manager.run(args);
  }

  /** Renders a frame. */
  void do_render(double abs_t, double delta_t)
  {
    render_args args;
    args.abs_t = abs_t;
    args.delta_t = delta_t;
    window.framebuffer_size(&args.framebuffer_width, &args.framebuffer_height);

    render_manager.render(args);
    window.swap_buffers();

#if defined(LINEAGE_DEBUG)
    GLenum error = opengl_error::last_error();
    if (error != GL_NO_ERROR)
    {
      std::ostringstream message;
      message << "Unexpected OpenGL error! " << opengl_error::error_string(error);
      lineage_log_warning(message.str());
    }
#endif
  }

};

/* -- Procedures -- */

application::application(window& window,
                         opengl& opengl,
                         input_manager& input_manager,
                         state_manager& state_manager,
                         render_manager& render_manager)
  : impl(std::make_unique<implementation>(window, opengl, input_manager, state_manager, render_manager))
{
  impl->input_manager.add_observer(*this);
  lineage_log_status("Application launched succesfully.");
}

application::~application()
{
  impl->input_manager.remove_observer(*this);
  lineage_log_status("Application terminating...");
}

void application::main()
{
  lineage_log_status("Entering main application loop...");

  double state_last_t = impl->window.time();
  double render_last_t = impl->window.time();

  while (!impl->window.should_close())
  {
    // process input every loop
    impl->do_input();

    // get current tick count
    double abs_t = impl->window.time();

    // process state if needed
    double state_delta_t = abs_t - state_last_t;
    if (state_delta_t > impl->state_manager.target_delta_t())
    {
      impl->do_state(abs_t, state_delta_t);
      state_last_t = abs_t;
    }

    // render if needed
    double render_delta_t = abs_t - render_last_t;
    if (render_delta_t > impl->render_manager.target_delta_t())
    {
      impl->do_render(abs_t, render_delta_t);
      render_last_t = abs_t;
    }
  }

  lineage_log_status("Exited main application loop.");
}

void application::input_event(input_type type, input_state state)
{
  if (type == input_type::application_exit && state == input_state::active)
    impl->window.set_should_close(true);
}
