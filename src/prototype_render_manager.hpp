/**
 * @file	prototype_render_manager.hpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/12
 */

#pragma once

/* -- Includes -- */

#include <memory>

#include "buffer.hpp"
#include "opengl.hpp"
#include "prototype_state_manager.hpp"
#include "render_manager.hpp"
#include "shader_program.hpp"
#include "state_manager.hpp"
#include "vertex_array.hpp"

/* -- Types -- */

namespace lineage
{

  /**
   * Render manager class used for prototyping and testing.
   */
  class prototype_render_manager final : public lineage::render_manager
  {

    /* -- Lifecycle -- */

  public:

    /**
     * Constructs a new `lineage::prototype_render_manager` instance.
     *
     * @param opengl
     * The OpenGL interface object in use by the application.
     *
     * @param state_manager
     * The state manager object in use by the application.
     */
    prototype_render_manager(lineage::opengl& opengl,
                             const lineage::prototype_state_manager& state_manager);

    virtual ~prototype_render_manager() = default;

  private:

    prototype_render_manager(const lineage::prototype_render_manager&) = delete;
    prototype_render_manager(lineage::prototype_render_manager&&) = delete;
    lineage::prototype_render_manager& operator =(const lineage::prototype_render_manager&) = delete;
    lineage::prototype_render_manager& operator =(lineage::prototype_render_manager&&) = delete;

  public:

    /* -- `lineage::render_manager` Implementation -- */

  public:

    virtual void render(const render_args& args) override;
    virtual double target_delta_t() const override;

    /* -- Implementation -- */

  private:

    lineage::opengl& m_opengl;
    const lineage::prototype_state_manager& m_state_manager;

    const std::unique_ptr<const lineage::shader_program> m_program;
    const std::unique_ptr<const lineage::immutable_buffer> m_buffer;
    const std::unique_ptr<lineage::vertex_array> m_vao;

  };

}
