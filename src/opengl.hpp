/**
 * @file	opengl.hpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/11
 */

#pragma once

/* -- Includes -- */

#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "api.hpp"

/* -- Types -- */

namespace lineage
{

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

    /**
     * Returns `true` if the ARB extension with the specified name is supported.
     */
    bool is_supported(const std::string& extension) const
    {
      return (glewIsSupported(extension.c_str()) == GL_TRUE);
    }

    /**
     * Sets the value of a 4x4 matrix uniform.
     */
    void set_uniform(GLuint location, const glm::mat4& matrix)
    {
      glUniformMatrix4fv(location,			// location
                         1,				// count
                         GL_FALSE,			// transpose
                         glm::value_ptr(matrix));	// value
    }

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

    static std::string get_string(GLenum name);

  };

}
