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

/* -- Procedures -- */

application::application(lineage::window& window,
                         lineage::opengl& opengl,
                         lineage::input_manager& input_manager,
                         lineage::state_manager& state_manager,
                         lineage::render_manager& render_manager)
  : m_window(window),
    m_opengl(opengl),
    m_input_manager(input_manager),
    m_state_manager(state_manager),
    m_render_manager(render_manager)
{
  m_input_manager.add_observer(*this);
  lineage_log_status("Application launched succesfully.");

#if defined(LINEAGE_DEBUG)
  prototype_function();
#endif
}

application::~application()
{
  m_input_manager.remove_observer(*this);
  lineage_log_status("Application terminating...");
}

void application::main()
{
  lineage_log_status("Entering main application loop...");

  double state_last_t = m_window.time();
  double render_last_t = m_window.time();

  while (!m_window.should_close())
  {
    // process input every loop
    do_input();

    // get current tick count
    double abs_t = m_window.time();

    // process state if needed
    double state_delta_t = abs_t - state_last_t;
    if (state_delta_t > m_state_manager.target_delta_t())
    {
      do_state(abs_t, state_delta_t);
      state_last_t = abs_t;
    }

    // render if needed
    double render_delta_t = abs_t - render_last_t;
    if (render_delta_t > m_render_manager.target_delta_t())
    {
      do_render(abs_t, render_delta_t);
      render_last_t = abs_t;
    }
  }

  lineage_log_status("Exited main application loop.");
}

void application::input_event(input_type type, input_state state)
{
  if (type == input_type::application_exit && state == input_state::active)
    m_window.set_should_close(true);
}

void application::do_input()
{
  m_window.poll_events();
}

void application::do_state(double abs_t, double delta_t)
{
  state_args args;
  args.abs_t = abs_t;
  args.delta_t = delta_t;

  m_state_manager.run(args);
}

void application::do_render(double abs_t, double delta_t)
{
  render_args args;
  args.abs_t = abs_t;
  args.delta_t = delta_t;
  m_window.framebuffer_size(&args.framebuffer_width, &args.framebuffer_height);

  m_render_manager.render(args);
  m_window.swap_buffers();

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

#if defined(LINEAGE_DEBUG)

void application::prototype_function()
{
}

#endif
