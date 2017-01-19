/**
 * @file	default_render_manager.hpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/15
 */

#pragma once

/* -- Includes -- */

#include <memory>
#include "render_manager.hpp"

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

    /**
     * Destructor.
     */
    virtual ~default_render_manager();

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

  private:

    struct implementation;
    std::unique_ptr<implementation> impl;

  };

}
