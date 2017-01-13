/**
 * @file	prototype_render_manager.cpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/12
 */

/* -- Includes -- */

#include "api.hpp"
#include "prototype_render_manager.hpp"
#include "prototype_state_manager.hpp"
#include "render_manager.hpp"

/* -- Namespaces -- */

using namespace lineage;

/* -- Procedures -- */

prototype_render_manager::prototype_render_manager(lineage::opengl& opengl,
                                                   const lineage::prototype_state_manager& state_manager)
  : m_opengl(opengl),
    m_state_manager(state_manager)
{
}

prototype_render_manager::~prototype_render_manager()
{
}

void prototype_render_manager::render(const render_args& args)
{
  auto background_color = m_state_manager.background_color();
  glClearColor(background_color.r,
               background_color.g,
               background_color.b,
               background_color.a);

  glClear(GL_COLOR_BUFFER_BIT);
}

double prototype_render_manager::target_delta_t() const
{
  return (1.0 / 60.0); // 60 HZ
}
