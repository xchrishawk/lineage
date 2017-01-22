/**
 * @file	opengl.hpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/11
 */

#pragma once

/* -- Includes -- */

#include <memory>
#include <glm/glm.hpp>

#include "api.hpp"

/* -- Types -- */

namespace lineage
{

  class buffer;
  class shader_program;
  class vertex_array;

  /**
   * Class representing an interface to the OpenGL library.
   */
  class opengl final
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

    /**
     * Destructor.
     */
    ~opengl();

  private:

    opengl(const lineage::opengl&) = delete;
    opengl(lineage::opengl&&) = delete;
    lineage::opengl& operator =(const lineage::opengl&) = delete;
    lineage::opengl& operator =(lineage::opengl&&) = delete;

    /* -- Public Methods -- */

  public:

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

    /**
     * Sets the value of a float uniform.
     */
    void set_uniform(GLuint location, float value);

    /**
     * Sets the value of a 4-dimensional vector uniform.
     */
    void set_uniform(GLuint location, const glm::vec4& value);

    /**
     * Sets the value of a 4x4 matrix uniform.
     */
    void set_uniform(GLuint location, const glm::mat4& value);

    /**
     * Pushes a buffer onto the stack for the specified binding point, making it active.
     */
    void push_buffer(GLenum target, const lineage::buffer& buffer);

    /**
     * Pops the active buffer off of the stack for the specified binding point, deactivating it and
     * reactivating the previous buffer.
     */
    void pop_buffer(GLenum target);

    /**
     * Pushes a shader program onto the stack, making it active.
     */
    void push_program(const lineage::shader_program& program);

    /**
     * Pops the active shader program off of the stack, deactivating it and reactivating the
     * previous shader program.
     */
    void pop_program();

    /**
     * Pushes a vertex array object onto the stack, making it active.
     */
    void push_vertex_array(const lineage::vertex_array& vao);

    /**
     * Pops the active vertex array object off of the stack, deactivating it and reactivating the
     * previous vertex array.
     */
    void pop_vertex_array();

    /* -- Implementation -- */

  private:

    struct implementation;
    const std::unique_ptr<implementation> impl;

  };

}
