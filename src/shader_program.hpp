/**
 * @file	shader_program.hpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/13
 */

#pragma once

/* -- Includes -- */

#include <string>
#include <utility>

#include "api.hpp"
#include "opengl_error.hpp"

/* -- Types -- */

namespace lineage
{

  class opengl;
  class shader;

  /**
   * Exception class representing an error occurring when attempting to link a shader program.
   */
  class shader_program_link_error : public lineage::opengl_error
  {
  public:

    /**
     * Constructs a new `lineage::shader_program_link_error` exception.
     *
     * @param message
     * The link error message.
     */
    shader_program_link_error(const std::string& message)
      : lineage::opengl_error(message)
    { }

  };

  /**
   * Class representing an OpenGL shader program.
   */
  class shader_program
  {

    /* -- Constants -- */

  public:

    /**
     * Invalid attribute/uniform location.
     */
    static const GLint invalid_location = -1;

    /* -- Lifecycle -- */

  public:

    /**
     * Constructs a new OpenGL shader program instance.
     *
     * @exception lineage::opengl_error
     * Thrown if a new program cannot be created for any reason.
     */
    shader_program();

    /**
     * Destructor.
     */
    ~shader_program();

  private:

    shader_program(const lineage::shader_program&) = delete;
    lineage::shader_program& operator =(const lineage::shader_program&) = delete;

    /* -- Public Methods -- */

  public:

    /**
     * Attaches the specified shader to this program.
     */
    void attach_shader(const lineage::shader& shader);

    /**
     * Detaches the specified shader from this program.
     */
    void detach_shader(const lineage::shader& shader);

    /**
     * Links the program.
     *
     * @exception lineage::opengl_error
     * Thrown is the shader program cannot be linked due to a generic OpenGL error.
     *
     * @exception lineage::shader_program_link_error
     * Thrown if the shader program cannot be linked due to a program-specific error.
     */
    void link();

    /**
     * Returns `true` if the program has been successfully linked.
     */
    bool is_linked() const;

    /**
     * Returns the number of shaders attached to this program.
     */
    GLint attached_shader_count() const;

    /**
     * Returns the number of active attributes for this program.
     */
    GLint active_attribute_count() const;

    /**
     * Returns the number of active uniforms for this program.
     */
    GLint active_uniform_count() const;

    /**
     * Returns the shader program's info log.
     */
    std::string info_log() const;

    /**
     * Returns the location for the attribute with the specified name. or
     * `shader_program::invalid_location` if no matching attribute is found.
     */
    GLint attribute_location(const std::string& name) const;

    /**
     * Returns the location for the uniform with the specified name, or
     * `shader_program::invalid_location` if no matching uniform is found.
     */
    GLint uniform_location(const std::string& name) const;

    /* -- Implementation -- */

  private:

    friend class opengl;

    GLuint m_handle;

  };

}
