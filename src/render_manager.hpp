/**
 * @file	render_manager.hpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/12
 */

#pragma once

/* -- Types -- */

namespace lineage
{

  /**
   * Struct containing arguments required to render a frame.
   */
  struct render_args
  {
    double abs_t;		/**< Total elapsed time, in seconds. */
    double delta_t;		/**< Time since last loop, in seconds. */
  };

  /**
   * Abstract interface for types responsible for rendering.
   */
  class render_manager
  {

    /* -- Lifecycle -- */

  protected:

    render_manager() = default;

  public:

    virtual ~render_manager() = default;

  private:

    render_manager(const lineage::render_manager&) = delete;
    render_manager(lineage::render_manager&&) = delete;
    lineage::render_manager& operator =(const lineage::render_manager&) = delete;
    lineage::render_manager& operator =(lineage::render_manager&&) = delete;

    /* -- Public Methods -- */

  public:

    /**
     * Renders a single frame.
     *
     * @param args
     * The arguments required to render a frame.
     */
    virtual void render(const lineage::render_args& args) = 0;

    /**
     * The target time between rendered frames, in seconds.
     */
    virtual double target_delta_t() const = 0;

  };

}
