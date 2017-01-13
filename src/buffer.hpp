/**
 * @file	buffer.hpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/13
 */

#pragma once

/* -- Includes -- */

#include "api.hpp"

/* -- Types -- */

namespace lineage
{

  /**
   * Abstract base class for types representing an OpenGL buffer.
   */
  class buffer
  {

    /* -- Lifecycle -- */

  protected:

    /**
     * Constructs a new `lineage::buffer` object.
     */
    buffer();

  public:

    virtual ~buffer();

  private:

    buffer(const lineage::buffer&) = delete;
    buffer(lineage::buffer&&) = delete;
    lineage::buffer& operator =(const lineage::buffer&) = delete;
    lineage::buffer& operator =(lineage::buffer&&) = delete;

    /* -- Implementation -- */

  protected:

    const GLuint m_handle;

  };

}
