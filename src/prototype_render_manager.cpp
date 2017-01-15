/**
 * @file	prototype_render_manager.cpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/12
 */

/* -- Includes -- */

#include "api.hpp"
#include "buffer.hpp"
#include "opengl.hpp"
#include "prototype_render_manager.hpp"
#include "prototype_state_manager.hpp"
#include "render_manager.hpp"
#include "shader.hpp"
#include "shader_program.hpp"
#include "shader_source.hpp"
#include "util.hpp"
#include "vertex_array.hpp"

/* -- Namespaces -- */

using namespace lineage;

/* -- Constants -- */

namespace
{
  constexpr vertex_array_attrib_spec VERTEX_POSITION_ATTRIBUTE_SPEC =
  {
    0,			// binding_index
    4,			// size
    GL_FLOAT,		// type
    false,		// normalized
    0,			// relative_offset
    true,		// enabled
  };

  constexpr float VERTEX_DATA[] =
  {
    0.0f, 0.0f, 0.0f, 1.0f,
    0.5f, 0.0f, 0.0f, 1.0f,
    0.0f, 0.5f, 0.0f, 1.0f,
    0.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, 0.0f, 0.0f, 1.0f,
    0.0f, -0.5f, 0.0f, 1.0f,
  };
}

/* -- Private Procedures -- */

namespace
{

  /** Creates the prototype shader program. */
  shader_program create_prototype_shader_program()
  {
    auto vertex_shader
      = create_shader(GL_VERTEX_SHADER, shader_source_string(shader_source::prototype_vertex_shader));
    auto fragment_shader
      = create_shader(GL_FRAGMENT_SHADER, shader_source_string(shader_source::prototype_fragment_shader));
    return create_shader_program({ &vertex_shader, &fragment_shader });
  }

  /** Creates the data buffer for the renderer. */
  immutable_buffer create_buffer()
  {
    return immutable_buffer(sizeof(VERTEX_DATA), VERTEX_DATA, 0);
  }

  /** Creates the vertex array object. */
  vertex_array create_vertex_array(const shader_program& program)
  {
    vertex_array vao;
    vertex_array_initialize_attribute(vao, program, "vertex_position", VERTEX_POSITION_ATTRIBUTE_SPEC);
    return vao;
  }

}

/* -- Procedures -- */

prototype_render_manager::prototype_render_manager(lineage::opengl& opengl,
                                                   const lineage::prototype_state_manager& state_manager)
  : m_opengl(opengl),
    m_state_manager(state_manager),
    m_program(create_prototype_shader_program()),
    m_buffer(create_buffer()),
    m_vao(create_vertex_array(m_program))
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

  // activate program
  m_opengl.push_program(m_program);
  defer pop_program([&] { m_opengl.pop_program(); });

  // actviate vertex array
  m_opengl.push_vertex_array(m_vao);
  defer pop_vertex_array([&] { m_opengl.pop_vertex_array(); });

  // bind vertex buffer
  m_vao.bind_buffer(VERTEX_POSITION_ATTRIBUTE_SPEC.binding_index,
                    m_buffer,
                    0,
                    VERTEX_POSITION_ATTRIBUTE_SPEC.size * sizeof(float));
  defer unbind_buffer([&] { m_vao.unbind_buffer(VERTEX_POSITION_ATTRIBUTE_SPEC.binding_index); });

  // draw vertices
  glDrawArrays(GL_TRIANGLES, 0, array_size(VERTEX_DATA) / VERTEX_POSITION_ATTRIBUTE_SPEC.size);
}

double prototype_render_manager::target_delta_t() const
{
  return (1.0 / 60.0); // 60 HZ
}
