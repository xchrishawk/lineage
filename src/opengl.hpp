/**
 * @file	opengl.hpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/11
 */

#pragma once

/* -- Includes -- */

#include <string>
#include <vector>

#include "api.hpp"

/* -- Types -- */

namespace lineage
{

  class shader_program;
  class vertex_array;

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

    static lineage::opengl* s_instance;

    std::vector<GLuint> m_programs;
    std::vector<GLuint> m_vertex_arrays;

  };

}
