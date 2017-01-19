/**
 * @file	default_render_manager.cpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/15
 */

/* -- Includes -- */

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

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
  // Uniform locations
  const GLuint MESH_MATRIX_UNIFORM_LOCATION = 0;
  const GLuint MODEL_MATRIX_UNIFORM_LOCATION = 1;
  const GLuint VIEW_MATRIX_UNIFORM_LOCATION = 2;
  const GLuint PROJ_MATRIX_UNIFORM_LOCATION = 3;

  // Attribute locations
  const GLuint VERTEX_POSITION_ATTRIBUTE_LOCATION = 0;
  const GLuint VERTEX_NORMAL_ATTRIBUTE_LOCATION = 1;
  const GLuint VERTEX_COLOR_ATTRIBUTE_LOCATION = 2;

  // Misc
  const GLuint BINDING_INDEX = 0;
}

/* -- Procedures -- */

default_render_manager::default_render_manager(opengl& opengl, const default_state_manager& state_manager)
  : m_opengl(opengl),
    m_state_manager(state_manager),
    m_program(create_shader_program()),
    m_vao(create_vertex_array<vertex_type>())
{
}

void default_render_manager::render(const render_args& args)
{
  // activate program
  m_opengl.push_program(m_program);
  defer pop_program([&] { m_opengl.pop_program(); });

  // actviate vertex array
  m_opengl.push_vertex_array(m_vao);
  defer pop_vertex_array([&] { m_opengl.pop_vertex_array(); });

  // set common uniforms
  m_opengl.set_uniform(VIEW_MATRIX_UNIFORM_LOCATION, view_matrix());
  m_opengl.set_uniform(PROJ_MATRIX_UNIFORM_LOCATION, proj_matrix(args));

  // initialize framebuffer
  render_init(args);
}

double default_render_manager::target_delta_t() const
{
  return (1.0 / 60.0); // 60 HZ
}

void default_render_manager::render_init(const render_args& args)
{
  // set viewport
  glViewport(0, 0, args.framebuffer_width, args.framebuffer_height);

  // clear buffer
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
}

glm::mat4 default_render_manager::view_matrix() const
{
    auto matrix =
      glm::translate(m_state_manager.camera_position()) *
      glm::mat4_cast(m_state_manager.camera_rotation());
    return glm::inverse(matrix);
}

glm::mat4 default_render_manager::proj_matrix(const render_args& args) const
{
    float aspect_ratio =
      static_cast<float>(args.framebuffer_width) /
      static_cast<float>(args.framebuffer_height);
    return glm::perspective(m_state_manager.camera_fov(),
                            aspect_ratio,
                            m_state_manager.camera_clip_near(),
                            m_state_manager.camera_clip_far());
}

shader_program default_render_manager::create_shader_program()
{
  auto vertex_shader
    = create_shader(GL_VERTEX_SHADER, shader_source_string(shader_source::default_vertex_shader));
  auto fragment_shader
    = create_shader(GL_FRAGMENT_SHADER, shader_source_string(shader_source::default_fragment_shader));

  return lineage::create_shader_program({ &vertex_shader, &fragment_shader });
}

template <typename TVertex>
vertex_array default_render_manager::create_vertex_array()
{
    vertex_array vao;

    configure_attribute(vao,
                        BINDING_INDEX,
                        VERTEX_POSITION_ATTRIBUTE_LOCATION,
                        position_attribute_spec<TVertex>());
    configure_attribute(vao,
                        BINDING_INDEX,
                        VERTEX_NORMAL_ATTRIBUTE_LOCATION,
                        normal_attribute_spec<TVertex>());
    configure_attribute(vao,
                        BINDING_INDEX,
                        VERTEX_COLOR_ATTRIBUTE_LOCATION,
                        color_attribute_spec<TVertex>());

    return vao;
}
