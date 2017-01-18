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
  const GLuint MODEL_MATRIX_UNIFORM_LOCATION = 0;
  const GLuint VIEW_MATRIX_UNIFORM_LOCATION = 1;
  const GLuint PROJ_MATRIX_UNIFORM_LOCATION = 2;

  // Attribute locations
  const GLuint VERTEX_POSITION_ATTRIBUTE_LOCATION = 0;
  const GLuint VERTEX_NORMAL_ATTRIBUTE_LOCATION = 1;
  const GLuint VERTEX_COLOR_ATTRIBUTE_LOCATION = 2;

  // Misc
  const GLuint BINDING_INDEX = 0;
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
    auto program
      = create_shader_program({ &vertex_shader, &fragment_shader });
    return program;
  }

  /** TEMPORARY - create mesh to render. */
  mesh<vertex334> TEMP_create_mesh()
  {
    static const std::vector<vertex334> VERTICES =
    {
      { { 0.0f, 0.0f, 0.0f }, { }, { 1.0f, 0.0f, 0.0f, 1.0f } },
      { { 0.5f, 0.0f, 0.0f }, { }, { 0.0f, 1.0f, 0.0f, 1.0f } },
      { { 0.5f, 0.5f, 0.0f }, { }, { 0.0f, 0.0f, 1.0f, 1.0f } },
      { { 0.0f, 0.0f, 0.0f }, { }, { 0.0f, 1.0f, 1.0f, 1.0f } },
      { { 0.0f, 0.5f, 0.0f }, { }, { 1.0f, 0.0f, 1.0f, 1.0f } },
      { { 0.5f, 0.5f, 0.0f }, { }, { 1.0f, 1.0f, 0.0f, 1.0f } },
    };
    return mesh<vertex334>(glm::vec3(),
                           glm::quat(),
                           glm::vec3(100.0f, 0.5f, 1.0f),
                           VERTICES);
  }

  /** Creates the vertex array object for the renderer to use. */
  template <typename TVertex>
  vertex_array create_vertex_array()
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

}

/* -- Procedures -- */

default_render_manager::default_render_manager(opengl& opengl, const default_state_manager& state_manager)
  : m_opengl(opengl),
    m_state_manager(state_manager),
    m_program(create_default_shader_program()),
    m_mesh(TEMP_create_mesh()),
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

  // bind vertex buffer
  m_vao.bind_buffer(BINDING_INDEX, m_mesh.buffer(), 0, sizeof(vertex_type));
  defer unbind_buffer([&] { m_vao.unbind_buffer(BINDING_INDEX); });

  // set model matrix uniform
  glUniformMatrix4fv(MODEL_MATRIX_UNIFORM_LOCATION,		// location,
                     1,						// count
                     GL_FALSE,					// transpose
                     glm::value_ptr(model_matrix(m_mesh)));	// value

  // set view matrix uniform
  glUniformMatrix4fv(VIEW_MATRIX_UNIFORM_LOCATION,		// location
                     1,						// count
                     GL_FALSE,					// transpose
                     glm::value_ptr(view_matrix()));		// value

  // set projection matrix uniform
  glUniformMatrix4fv(PROJ_MATRIX_UNIFORM_LOCATION,		// location
                     1,						// count
                     GL_FALSE,					// transpose
                     glm::value_ptr(proj_matrix(args)));	// value

  // initialize framebuffer
  render_init(args);

  // draw vertices
  glDrawArrays(GL_TRIANGLES, 0, m_mesh.vertex_count());
}

double default_render_manager::target_delta_t() const
{
  return (1.0 / 60.0); // 60 HZ
}

void default_render_manager::render_init(const render_args& args) const
{
  // set viewport
  glViewport(0, 0, args.framebuffer_width, args.framebuffer_height);

  // clear buffer
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
}

glm::mat4 default_render_manager::model_matrix(const mesh<vertex_type>& mesh) const
{
  auto matrix =
    glm::translate(mesh.position()) *
    glm::scale(mesh.scale()) *
    glm::mat4_cast(mesh.rotation());
  return matrix;
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
