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

/* -- Private Procedures -- */

namespace
{

  /** TEMPORARY - create object to render. */
  object<vertex334> TEMP_create_object()
  {
    std::vector<mesh<vertex334>> meshes;

    static const std::vector<vertex334> MESH_1_VERTICES =
    {
      { { 0.0f, 0.0f, 0.0f }, { }, { 1.0f, 0.0f, 0.0f, 1.0f } },
      { { 0.5f, 0.0f, 0.0f }, { }, { 0.0f, 1.0f, 0.0f, 1.0f } },
      { { 0.5f, 0.5f, 0.0f }, { }, { 0.0f, 0.0f, 1.0f, 1.0f } },
    };
    meshes.emplace_back(MESH_1_VERTICES);

    static const std::vector<vertex334> MESH_2_VERTICES =
    {
      { { 0.0f, 0.0f, 0.0f }, { }, { 0.0f, 1.0f, 1.0f, 1.0f } },
      { { -0.5f, 0.0f, 0.0f }, { }, { 1.0f, 0.0f, 1.0f, 1.0f } },
      { { 0.0f, -0.5f, 0.0f }, { }, { 1.0f, 1.0f, 0.0f, 1.0f } },
    };
    meshes.emplace_back(MESH_2_VERTICES);

    return object<vertex334>(glm::vec3(-2.0f, 0.0f, 0.0f),
                             glm::quat(),
                             glm::vec3(1.0f),
                             std::move(meshes));
  }

}

/* -- Procedures -- */

default_render_manager::default_render_manager(opengl& opengl, const default_state_manager& state_manager)
  : m_opengl(opengl),
    m_state_manager(state_manager),
    m_program(create_shader_program()),
    m_object(TEMP_create_object()),
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

  // draw each object
  render_object(m_object);
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

void default_render_manager::render_object(const object<vertex_type>& object)
{
  // set model matrix uniform
  m_opengl.set_uniform(MODEL_MATRIX_UNIFORM_LOCATION, model_matrix(object));

  // render each mesh
  for (const auto& mesh : object.meshes())
    render_mesh(mesh);
}

void default_render_manager::render_mesh(const mesh<vertex_type>& mesh)
{
  // set mesh matrix uniform
  m_opengl.set_uniform(MESH_MATRIX_UNIFORM_LOCATION, model_matrix(mesh));

  // bind the buffer for this mesh
  m_vao.bind_buffer(BINDING_INDEX, mesh.buffer(), 0, sizeof(vertex_type));
  defer unbind_buffer([&] { m_vao.unbind_buffer(BINDING_INDEX); });

  // draw the mesh
  glDrawArrays(GL_TRIANGLES, 0, mesh.vertex_count());
}

template <typename TObject>
glm::mat4 default_render_manager::model_matrix(const TObject& obj) const
{
  auto matrix =
    glm::translate(obj.position()) *
    glm::scale(obj.scale()) *
    glm::mat4_cast(obj.rotation());
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
