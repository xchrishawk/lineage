/**
 * @file	application.hpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/11
 */

#pragma once

/* -- Includes -- */

#include "input_manager.hpp"
#include "opengl.hpp"
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
     */
    application(lineage::window window,
                lineage::opengl opengl,
                lineage::input_manager input_manager);

    ~application();

  private:

    application(const lineage::application&) = delete;
    lineage::application& operator =(const lineage::application&) = delete;

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

    lineage::window m_window;
    lineage::opengl m_opengl;
    lineage::input_manager m_input_manager;

    void do_input();
    void do_state(double abs_t, double delta_t);
    void do_render(double abs_t, double delta_t);

  };

}
