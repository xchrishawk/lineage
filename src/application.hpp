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
  class application
  {

    /* -- Lifecycle -- */

  public:

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

    /* -- Implementation -- */

  private:

    lineage::window m_window;
    lineage::opengl m_opengl;

  };

}
