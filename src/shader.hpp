/**
 * @file	shader.hpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/12
 */

#pragma once

/* -- Includes -- */

#include <stdexcept>
#include <string>

#include "api.hpp"
#include "opengl_error.hpp"

/* -- Types -- */

namespace lineage
{

  class shader_program;

  /**
   * Exception class representing an error occurring when attempting to compile a shader.
   */
  class shader_compile_error : public lineage::opengl_error
  {
  public:

    /**
     * Constructs a new `lineage::shader_compile_error` exception.
     *
     * @param message
     * The compilation error message.
     */
    shader_compile_error(const std::string& message)
      : lineage::opengl_error(message)
    { }

  };

  /**
   * Class representing an OpenGL shader.
   */
  class shader
  {

    /* -- Lifecycle -- */

  public:

    /**
     * Constructs a new OpenGL shader instance.
     *
     * @param type
     * The type of shader to create.
     *
     * @exception lineage::opengl_error
     * Thrown if a new shader cannot be created for any reason.
     */
    shader(GLenum type);

    /**
     * Destructor.
     */
    ~shader();

  private:

    shader(const lineage::shader&) = delete;
    shader(lineage::shader&&) = delete;
    lineage::shader& operator =(const lineage::shader&) = delete;
    lineage::shader& operator =(lineage::shader&&) = delete;

    /* -- Public Methods -- */

  public:

    /**
     * The type of this shader.
     */
    GLenum type() const;

    /**
     * A string for the type of this shader.
     */
    const std::string& type_string() const;

    /**
     * Sets the source code for this shader.
     */
    void set_source(const std::string& source);

    /**
     * Compiles the shader.
     *
     * @exception lineage::opengl_error
     * Thrown if the shader cannot be compiled due to a generic OpenGL error.
     *
     * @exception lineage::shader_compile_error
     * Thrown if the shader cannot be compiled due to a GLSL error.
     */
    void compile();

    /**
     * Returns `true` if the shader has been successfully compiled.
     */
    bool is_compiled() const;

    /**
     * Returns the OpenGL info log for this shader.
     */
    std::string info_log() const;

    /**
     * Returns a string for the specified shader type enum.
     */
    static const std::string& type_string(GLenum shader_type);

    /* -- Implementation -- */

  private:

    friend class lineage::shader_program;

    const GLenum m_type;
    const GLuint m_handle;

  };

}
