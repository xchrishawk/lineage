/**
 * @file	default_render_manager.hpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/15
 */

#pragma once

/* -- Includes -- */

#include <glm/glm.hpp>

#include "buffer.hpp"
#include "mesh.hpp"
#include "render_manager.hpp"
#include "shader_program.hpp"
#include "vertex.hpp"
#include "vertex_array.hpp"

/* -- Types -- */

namespace lineage
{

  class default_state_manager;
  class opengl;

  /**
   * Render manager implementation.
   */
  class default_render_manager final : public lineage::render_manager
  {

    /* -- Types -- */

  private:

    /** The type of vertex the renderer is using. */
    using vertex_type = lineage::vertex334;

    /* -- Lifecycle -- */

  public:

    /**
     * Constructs a new `lineage::default_render_manager` instance.
     *
     * @param opengl
     * The OpenGL interface in use by the application.
     *
     * @param state_manager
     * The state manager in use by the application.
     */
    default_render_manager(lineage::opengl& opengl, const lineage::default_state_manager& state_manager);

    virtual ~default_render_manager() = default;

  private:

    default_render_manager(const lineage::default_render_manager&) = delete;
    default_render_manager(lineage::default_render_manager&&) = delete;
    lineage::default_render_manager& operator =(const lineage::default_render_manager&) = delete;
    lineage::default_render_manager& operator =(lineage::default_render_manager&&) = delete;

    /* -- `lineage::render_manager` Implementation -- */

  public:

    virtual void render(const lineage::render_args& args);
    virtual double target_delta_t() const;

    /* -- Implementation -- */

  public:

    lineage::opengl& m_opengl;
    const lineage::default_state_manager& m_state_manager;
    const lineage::shader_program m_program;
    const lineage::mesh<vertex_type> m_mesh;
    lineage::vertex_array m_vao;

    void render_init(const lineage::render_args& args) const;

    glm::mat4 model_matrix(const lineage::mesh<vertex_type>& mesh) const;
    glm::mat4 view_matrix() const;
    glm::mat4 proj_matrix(const lineage::render_args& args) const;

  };

}
