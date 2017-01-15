/**
 * @file	default_render_manager.cpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/15
 */

/* -- Includes -- */

#include "api.hpp"
#include "default_render_manager.hpp"
#include "default_state_manager.hpp"
#include "opengl.hpp"
#include "render_manager.hpp"
#include "state_manager.hpp"

/* -- Namespaces -- */

using namespace lineage;

/* -- Procedures -- */

default_render_manager::default_render_manager(opengl& opengl, const default_state_manager& state_manager)
  : m_opengl(opengl),
    m_state_manager(state_manager)
{
}

void default_render_manager::render(const render_args& args)
{
  glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
}

double default_render_manager::target_delta_t() const
{
  return (1.0 / 60.0); // 60 HZ
}
