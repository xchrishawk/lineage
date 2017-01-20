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
#include "vertex.hpp"
#include "vertex_array.hpp"

/* -- Namespaces -- */

using namespace lineage;

/* -- Constants -- */

namespace
{
  const GLuint BINDING_INDEX = 0;
  const vertex VERTEX_DATA[] =
  {
    { { 0.0f, 0.0f, 0.0f }, { }, { 1.0f, 0.0f, 0.0f, 1.0f }, { } },
    { { 0.5f, 0.0f, 0.0f }, { }, { 0.0f, 1.0f, 0.0f, 1.0f }, { } },
    { { 0.0f, 0.5f, 0.0f }, { }, { 0.0f, 0.0f, 1.0f, 1.0f }, { } },
    { { 0.0f, 0.0f, 0.0f }, { }, { 0.0f, 1.0f, 1.0f, 1.0f }, { } },
    { { -0.5f, 0.0f, 0.0f }, { }, { 1.0f, 0.0f, 1.0f, 1.0f }, { } },
    { { 0.0f, -0.5f, 0.0f }, { }, { 1.0f, 1.0f, 0.0f, 1.0f }, { } },
  };
}

/* -- Private Procedures -- */

namespace
{

  /** Creates the prototype shader program. */
  std::unique_ptr<shader_program> create_prototype_shader_program()
  {
    shader vertex_shader(GL_VERTEX_SHADER);
    vertex_shader.set_source(shader_source_string(shader_source::prototype_vertex_shader));
    vertex_shader.compile();

    shader fragment_shader(GL_FRAGMENT_SHADER);
    fragment_shader.set_source(shader_source_string(shader_source::prototype_fragment_shader));
    fragment_shader.compile();

    auto program = std::make_unique<shader_program>();
    program->attach_shader(vertex_shader);
    program->attach_shader(fragment_shader);
    program->link();
    program->detach_shader(vertex_shader);
    program->detach_shader(fragment_shader);

    return program;
  }

  /** Creates the data buffer for the renderer. */
  std::unique_ptr<const immutable_buffer> create_buffer()
  {
    return std::make_unique<immutable_buffer>(sizeof(VERTEX_DATA), VERTEX_DATA, 0);
  }

  /** Creates the vertex array object. */
  std::unique_ptr<vertex_array> create_vertex_array(const std::unique_ptr<const shader_program>& program)
  {
    auto vao = std::make_unique<vertex_array>();
    configure_attribute(*vao, BINDING_INDEX, *program, "vertex_position", position_attribute_spec<vertex>());
    configure_attribute(*vao, BINDING_INDEX, *program, "vertex_color", color_attribute_spec<vertex>());
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

void prototype_render_manager::render(const render_args& args)
{
  auto background_color = m_state_manager.background_color();
  glClearColor(background_color.r,
               background_color.g,
               background_color.b,
               background_color.a);

  glClear(GL_COLOR_BUFFER_BIT);

  // activate program
  m_opengl.push_program(*m_program);
  defer pop_program([&] { m_opengl.pop_program(); });

  // actviate vertex array
  m_opengl.push_vertex_array(*m_vao);
  defer pop_vertex_array([&] { m_opengl.pop_vertex_array(); });

  // bind vertex buffer
  m_vao->bind_buffer(BINDING_INDEX, *m_buffer, 0, sizeof(vertex));
  defer unbind_buffer([&] { m_vao->unbind_buffer(BINDING_INDEX); });

  // draw vertices
  glDrawArrays(GL_TRIANGLES, 0, array_size(VERTEX_DATA));
}

double prototype_render_manager::target_delta_t() const
{
  return (1.0 / 60.0); // 60 HZ
}
