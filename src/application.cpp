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
                         lineage::opengl opengl)
  : m_window(std::move(window)),
    m_opengl(std::move(opengl))
{
  lineage_log_status("Application launched succesfully.");
}

application::~application()
{
  lineage_log_status("Application terminating...");
}

void application::main()
{
  lineage_log_status("Entering main application loop...");

  while (!m_window.should_close())
  {
    m_window.poll_events();
  }

  lineage_log_status("Exited main application loop.");
}
