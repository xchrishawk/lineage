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

    opengl(lineage::opengl&& other);
    ~opengl();

  private:

    opengl(const lineage::opengl&) = delete;
    lineage::opengl& operator =(const lineage::opengl&) = delete;
    lineage::opengl& operator =(lineage::opengl&&) = delete;

    /* -- Public Methods -- */

    /**
     * Returns the OpenGL `GL_VERSION` string.
     */
    std::string api_version() const
    {
      return get_string(GL_VERSION);
    }

    /**
     * Returns the OpenGL `GL_SHADING_LANGUAGE_VERSION` string.
     */
    std::string shading_language_version() const
    {
      return get_string(GL_SHADING_LANGUAGE_VERSION);
    }

    /**
     * Returns the OpenGL `GL_RENDERER` string.
     */
    std::string renderer() const
    {
      return get_string(GL_RENDERER);
    }

    /**
     * Returns the OpenGL `GL_VERSION` string.
     */
    std::string vendor() const
    {
      return get_string(GL_VENDOR);
    }

    /* -- Implementation -- */

  private:

    static lineage::opengl* s_instance;

    static std::string get_string(GLenum name)
    {
      const GLubyte* ustr = glGetString(name);
      const char* cstr = reinterpret_cast<const char*>(ustr);
      return std::string(cstr);
    }

  };

}
