/**
 * @file	default_render_manager.cpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/15
 */

/* -- Includes -- */

#include "api.hpp"
#include "buffer.hpp"
#include "default_render_manager.hpp"
#include "default_state_manager.hpp"
#include "opengl.hpp"
#include "render_manager.hpp"
#include "shader.hpp"
#include "shader_program.hpp"
#include "shader_source.hpp"
#include "state_manager.hpp"
#include "vertex.hpp"
#include "vertex_array.hpp"

/* -- Namespaces -- */

using namespace lineage;

/* -- Constants -- */

namespace
{
  const GLuint BINDING_INDEX = 0;
  const vertex3x4 VERTEX_DATA[] =
  {
    { { 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
    { { 0.5f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
    { { 0.0f, 0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
    { { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 1.0f, 1.0f } },
    { { -0.5f, 0.0f, 0.0f }, { 1.0f, 0.0f, 1.0f, 1.0f } },
    { { 0.0f, -0.5f, 0.0f }, { 1.0f, 1.0f, 0.0f, 1.0f } },
  };
}

/* -- Private Procedures -- */

namespace
{

  /** Creates the shader program to use. */
  shader_program create_default_shader_program()
  {
    auto vertex_shader
      = create_shader(GL_VERTEX_SHADER, shader_source_string(shader_source::default_vertex_shader));
    auto fragment_shader
      = create_shader(GL_FRAGMENT_SHADER, shader_source_string(shader_source::default_fragment_shader));
    return create_shader_program({ &vertex_shader, &fragment_shader });
  }

  /** TEMPORARY - Create the data buffer for the renderer. */
  immutable_buffer TEMP_create_buffer()
  {
    return immutable_buffer(sizeof(VERTEX_DATA), VERTEX_DATA, 0);
  }

  /** Creates the vertex array object for the renderer to use. */
  vertex_array create_vertex_array(const shader_program& program)
  {
    vertex_array vao;
    configure_attribute(vao, BINDING_INDEX, program, "vertex_position", position_attribute_spec<vertex3x4>());
    configure_attribute(vao, BINDING_INDEX, program, "vertex_color", color_attribute_spec<vertex3x4>());
    return vao;
  }

}

/* -- Procedures -- */

default_render_manager::default_render_manager(opengl& opengl, const default_state_manager& state_manager)
  : m_opengl(opengl),
    m_state_manager(state_manager),
    m_program(create_default_shader_program()),
    m_buffer(TEMP_create_buffer()),
    m_vao(create_vertex_array(m_program))
{
}

void default_render_manager::render(const render_args& args)
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

    // activate program
  m_opengl.push_program(m_program);
  defer pop_program([&] { m_opengl.pop_program(); });

  // actviate vertex array
  m_opengl.push_vertex_array(m_vao);
  defer pop_vertex_array([&] { m_opengl.pop_vertex_array(); });

  // bind vertex buffer
  m_vao.bind_buffer(BINDING_INDEX, m_buffer, 0, sizeof(vertex3x4));
  defer unbind_buffer([&] { m_vao.unbind_buffer(BINDING_INDEX); });

  // draw vertices
  glDrawArrays(GL_TRIANGLES, 0, array_size(VERTEX_DATA));
}

double default_render_manager::target_delta_t() const
{
  return (1.0 / 60.0); // 60 HZ
}
