/**
 * @file	opengl.hpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/11
 */

#pragma once

/* -- Includes -- */

#include <string>

#include "api.hpp"

/* -- Types -- */

namespace lineage
{

  /**
   * Class representing an interface to the OpenGL library.
   */
  class opengl
  {

    /* -- Lifecycle -- */

  public:

    /**
     * Creates a new OpenGL interface object.
     *
     * @exception std::runtime_error
     * Thrown if the OpenGL interface fails to initialize.
     */
    opengl();

    ~opengl();

  private:

    opengl(const lineage::opengl&) = delete;
    opengl(lineage::opengl&&) = delete;
    lineage::opengl& operator =(const lineage::opengl&) = delete;
    lineage::opengl& operator =(lineage::opengl&&) = delete;

    /* -- Public Methods -- */

    /**
     * Returns the OpenGL `GL_VERSION` string.
     */
    std::string api_version() const;

    /**
     * Returns the OpenGL `GL_SHADING_LANGUAGE_VERSION` string.
     */
    std::string shading_language_version() const;

    /**
     * Returns the OpenGL `GL_RENDERER` string.
     */
    std::string renderer() const;

    /**
     * Returns the OpenGL `GL_VERSION` string.
     */
    std::string vendor() const;

    /**
     * Returns `true` if the ARB extension with the specified name is supported.
     */
    bool is_supported(const std::string& extension) const;

    /* -- Implementation -- */

  private:

    static lineage::opengl* s_instance;

  };

}
