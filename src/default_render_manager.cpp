/**
 * @file	default_render_manager.cpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/15
 */

/* -- Includes -- */

#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include "api.hpp"
#include "buffer.hpp"
#include "default_render_manager.hpp"
#include "default_state_manager.hpp"
#include "mesh.hpp"
#include "opengl.hpp"
#include "render_manager.hpp"
#include "scene_graph.hpp"
#include "shader.hpp"
#include "shader_program.hpp"
#include "shader_source.hpp"
#include "state_manager.hpp"
#include "util.hpp"
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

/* -- Types -- */

/**
 * Implementation for the `lineage::default_render_manager` class.
 */
struct default_render_manager::implementation
{

  /* -- Constructor -- */

  implementation(lineage::opengl& opengl, const lineage::default_state_manager& state_manager)
    : opengl(opengl),
      state_manager(state_manager),
      program(implementation::create_shader_program()),
      vao(implementation::create_vertex_array<vertex>())
  { }

  /* -- Fields -- */

  lineage::opengl& opengl;
  const lineage::default_state_manager& state_manager;
  const std::unique_ptr<const lineage::shader_program> program;
  const std::unique_ptr<lineage::vertex_array> vao;

  /* -- Procedures -- */

  /** Create the model matrix to use for rendering a specific scene node. */
  glm::mat4 model_matrix(const lineage::scene_node& node, const glm::mat4& model_matrix) const
  {
    return
      glm::translate(node.position()) *
      glm::mat4_cast(node.rotation()) *
      glm::scale(node.scale()) *
      model_matrix;
  }

  /** Create the view matrix to use for rendering. */
  glm::mat4 view_matrix() const
  {
    auto matrix =
      glm::translate(state_manager.camera_position()) *
      glm::mat4_cast(state_manager.camera_rotation());
    return glm::inverse(matrix);
  }

  /** Create the projection matrix to use for rendering. */
  glm::mat4 proj_matrix(const render_args& args) const
  {
    auto aspect_ratio =
      static_cast<float>(args.framebuffer_width) /
      static_cast<float>(args.framebuffer_height);
    return glm::perspective(state_manager.camera_fov(),
                            aspect_ratio,
                            state_manager.camera_clip_near(),
                            state_manager.camera_clip_far());
  }

  /** Initialize the framebuffer for rendering. */
  void render_init(const render_args& args)
  {
    // set viewport
    glViewport(0, 0, args.framebuffer_width, args.framebuffer_height);

    // clear buffer
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
  }

  /** Renders the specified scene node. */
  void render_scene_node(const lineage::scene_graph& graph,
                         const lineage::scene_node& node,
                         const glm::mat4& parent_model_matrix)
  {
    // update the model matrix for this specific node
    auto model_matrix = this->model_matrix(node, parent_model_matrix);
    opengl.set_uniform(MODEL_MATRIX_UNIFORM_LOCATION, model_matrix);

    // render all meshes for this node
    for (const auto& mesh_index : node.meshes())
    {
      const auto& mesh = *graph.meshes()[mesh_index];
      render_mesh(mesh);
    }

    // recursively render child nodes
    for (const auto& child_node : node.children())
      render_scene_node(graph, child_node, model_matrix);
  }

  /** Renders the specified mesh. */
  void render_mesh(const lineage::mesh& mesh)
  {
    // bind vertex buffer
    vao->bind_buffer(BINDING_INDEX, mesh.vertex_buffer(), 0, mesh.vertex_size());
    defer unbind_vertex_buffer([&] { vao->unbind_buffer(BINDING_INDEX); });

    opengl.push_buffer(GL_ELEMENT_ARRAY_BUFFER, mesh.index_buffer());
    defer unbind_element_buffer([&] { opengl.pop_buffer(GL_ELEMENT_ARRAY_BUFFER); });

    // draw vertices
    static const void* NO_OFFSET = reinterpret_cast<void*>(0);
    glDrawElements(mesh.draw_mode(), mesh.index_count(), mesh.index_datatype(), NO_OFFSET);
  }

  /** Creates the shader program for the renderer to be use. */
  static std::unique_ptr<shader_program> create_shader_program()
  {
    shader vertex_shader(GL_VERTEX_SHADER);
    vertex_shader.set_source(shader_source_string(shader_source::default_vertex_shader));
    vertex_shader.compile();

    shader fragment_shader(GL_FRAGMENT_SHADER);
    fragment_shader.set_source(shader_source_string(shader_source::default_fragment_shader));
    fragment_shader.compile();

    auto program = std::make_unique<shader_program>();
    program->attach_shader(vertex_shader);
    program->attach_shader(fragment_shader);
    program->link();
    program->detach_shader(vertex_shader);
    program->detach_shader(fragment_shader);

    return program;
  }

  /** Creates the vertex array for the renderer to use. */
  template <typename TVertex>
  static std::unique_ptr<vertex_array> create_vertex_array()
  {
    auto vao = std::make_unique<vertex_array>();

    configure_attribute(*vao,
                        BINDING_INDEX,
                        VERTEX_POSITION_ATTRIBUTE_LOCATION,
                        position_attribute_spec<TVertex>());
    configure_attribute(*vao,
                        BINDING_INDEX,
                        VERTEX_NORMAL_ATTRIBUTE_LOCATION,
                        normal_attribute_spec<TVertex>());
    configure_attribute(*vao,
                        BINDING_INDEX,
                        VERTEX_COLOR_ATTRIBUTE_LOCATION,
                        color_attribute_spec<TVertex>());

    return vao;
  }

};

/* -- Procedures -- */

default_render_manager::default_render_manager(opengl& opengl, const default_state_manager& state_manager)
  : impl(std::make_unique<implementation>(opengl, state_manager))
{
}

default_render_manager::~default_render_manager() = default;

void default_render_manager::render(const render_args& args)
{
  // activate program
  impl->opengl.push_program(*impl->program);
  defer pop_program([&] { impl->opengl.pop_program(); });

  // actviate vertex array
  impl->opengl.push_vertex_array(*impl->vao);
  defer pop_vertex_array([&] { impl->opengl.pop_vertex_array(); });

  // set common uniforms
  impl->opengl.set_uniform(VIEW_MATRIX_UNIFORM_LOCATION, impl->view_matrix());
  impl->opengl.set_uniform(PROJ_MATRIX_UNIFORM_LOCATION, impl->proj_matrix(args));

  // initialize framebuffer
  impl->render_init(args);

  // recursively render nodes
  glm::mat4 model_matrix = glm::mat4();
  const auto& graph = impl->state_manager.scene_graph();
  for (const auto& node : graph.nodes())
    impl->render_scene_node(graph, node, model_matrix);
}

double default_render_manager::target_delta_t() const
{
  return (1.0 / 60.0); // 60 HZ
}
