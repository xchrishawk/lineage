/**
 * @file	opengl_error.hpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/12
 */

#pragma once

/* -- Includes -- */

#include <stdexcept>

#include "api.hpp"
#include "debug.hpp"

/* -- Types -- */

namespace lineage
{

  /**
   * Class representing an error thrown by the OpenGL library.
   */
  class opengl_error : public std::runtime_error
  {

    /* -- Lifecycle -- */

  public:

    /**
     * Constructs a new `lineage::opengl_error` instance for a specific error code.
     *
     * @param error
     * The OpenGL error code returned.
     */
    opengl_error(GLenum error)
      : std::runtime_error(error_string(error)),
        m_error(error)
    { }

  protected:

    /**
     * Constructs a new `lineage::opengl_error` instance with an arbitrary message.
     *
     * @note
     * `error` is set to `GL_NO_ERROR`.
     *
     * @param message
     * The error message.
     */
    opengl_error(const std::string& message)
      : std::runtime_error(message),
        m_error(GL_NO_ERROR)
    { }

    /* -- Public Methods -- */

  public:

    /**
     * The error code used to generate this exception, or `GL_NO_ERROR` if there was no specific
     * error code which triggered the exception.
     */
    GLenum error() const
    {
      return m_error;
    }

    /* -- Static Methods -- */

  public:

    /**
     * Returns the last OpenGL error code returned by the API.
     */
    static GLenum last_error();

    /**
     * Throws a `lineage::opengl_error` exception for the most recent OpenGL error.
     */
    [[noreturn]] static void throw_last_error();

    /**
     * Returns an error string for the specified OpenGL error enum.
     */
    static const std::string& error_string(GLenum error) noexcept;

  private:

    GLenum m_error;

  };

}
