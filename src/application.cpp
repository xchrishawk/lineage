/**
 * @file	application.cpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/11
 */

/* -- Includes -- */

#include <utility>

#include "application.hpp"
#include "debug.hpp"
#include "window.hpp"

/* -- Namespaces -- */

using namespace lineage;

/* -- Procedures -- */

application::application(lineage::window window,
                         lineage::opengl opengl,
                         lineage::input_manager input_manager)
  : m_window(std::move(window)),
    m_opengl(std::move(opengl)),
    m_input_manager(std::move(input_manager))
{
  m_input_manager.add_observer(this);
  m_window.add_observer(&m_input_manager);

  lineage_log_status("Application launched succesfully.");
}

application::~application()
{
  m_window.remove_observer(&m_input_manager);
  m_input_manager.remove_observer(this);

  lineage_log_status("Application terminating...");
}

void application::main()
{
  lineage_log_status("Entering main application loop...");

  static constexpr double STATE_TARGET_DELTA_T = (1.0 / 60.0);
  static constexpr double RENDER_TARGET_DELTA_T = (1.0 / 60.0);

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
    if (state_delta_t > STATE_TARGET_DELTA_T)
    {
      do_state(abs_t, state_delta_t);
      state_last_t = abs_t;
    }

    // render if needed
    double render_delta_t = abs_t - render_last_t;
    if (render_delta_t > RENDER_TARGET_DELTA_T)
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

}

void application::do_render(double abs_t, double delta_t)
{
  glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  m_window.swap_buffers();
}
