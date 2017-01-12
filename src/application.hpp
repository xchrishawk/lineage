/**
 * @file	application.hpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/11
 */

#pragma once

/* -- Includes -- */

#include "opengl.hpp"
#include "window.hpp"

/* -- Types -- */

namespace lineage
{

  /**
   * Main class for the application.
   */
  class application : public window_observer
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
     */
    application(lineage::window window,
                lineage::opengl opengl);

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

    /* -- `lineage::window_observer` Implementation -- */

    virtual void window_key_event(int key, int action, int mods) override;

    /* -- Implementation -- */

  private:

    lineage::window m_window;
    lineage::opengl m_opengl;

  };

}
