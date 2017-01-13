/**
 * @file	application.hpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/11
 */

#pragma once

/* -- Includes -- */

#include <memory>

#include "input_manager.hpp"
#include "opengl.hpp"
#include "render_manager.hpp"
#include "state_manager.hpp"
#include "window.hpp"

/* -- Types -- */

namespace lineage
{

  /**
   * Main class for the application.
   */
  class application : public lineage::input_observer
  {

    /* -- Lifecycle -- */

  public:

    /**
     * Creates an application instance.
     *
     * @param window
     * The main application window.
     *
     * @param opengl
     * The OpenGL interface object to use.
     *
     * @param input_manager
     * The input manager object to use.
     *
     * @param state_manager
     * The state manager object to use.
     *
     * @param render_manager
     * The render manager object to use.
     */
    application(lineage::window& window,
                lineage::opengl& opengl,
                lineage::input_manager& input_manager,
                lineage::state_manager& state_manager,
                lineage::render_manager& render_manager);

    ~application();

  private:

    application(const lineage::application&) = delete;
    application(lineage::application&) = delete;
    lineage::application& operator =(const lineage::application&) = delete;
    lineage::application& operator =(lineage::application&&) = delete;

    /* -- Public Methods -- */

  public:

    /**
     * Runs the main application loop.
     */
    void main();

    /* -- `lineage::input_observer` Implementation -- */

    virtual void input_event(lineage::input_type type, lineage::input_state state) override;

    /* -- Implementation -- */

  private:

    lineage::window& m_window;
    lineage::opengl& m_opengl;
    lineage::input_manager& m_input_manager;
    lineage::state_manager& m_state_manager;
    lineage::render_manager& m_render_manager;

    void do_input();
    void do_state(double abs_t, double delta_t);
    void do_render(double abs_t, double delta_t);

#if defined(LINEAGE_DEBUG)
    void prototype_function();
#endif

  };

}
