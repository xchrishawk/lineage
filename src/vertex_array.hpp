/**
 * @file	vertex_array.hpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/14
 */

#pragma once

/* -- Includes -- */

#include <string>

#include "api.hpp"

/* -- Types -- */

namespace lineage
{

  class buffer;
  class opengl;
  class shader_program;

  /**
   * Class representing an OpenGL vertex array object (VAO).
   */
  class vertex_array
  {

    /* -- Lifecycle -- */

  public:

    /**
     * Constructs a new `lineage::vertex_array` instance.
     *
     * @exception lineage::opengl_error
     * Thrown if a new vertex array cannot be created for any reason.
     */
    vertex_array();

    /**
     * Destructor.
     */
    ~vertex_array();

  private:

    vertex_array(const lineage::vertex_array&) = delete;
    vertex_array(lineage::vertex_array&&) = delete;
    lineage::vertex_array& operator =(const lineage::vertex_array&) = delete;
    lineage::vertex_array& operator =(lineage::vertex_array&&) = delete;

    /* -- Public Methods -- */

  public:

    /**
     * Sets the data format for a particular attribute location.
     *
     * @param attribute
     * The attribute whose format is being set.
     *
     * @param count
     * The number of values per vertex stored in the array.
     *
     * @param type
     * The type of data stored in the array (`GL_FLOAT`, `GL_DOUBLE`, etc.)
     *
     * @param normalized
     * If `true`, the values in the array will be normalized.
     *
     * @param relative_offset
     * The relative offset from the beginning of the vertex record in the array to the beginning of
     * the data for this particular attribute.
     */
    void set_attribute_format(GLuint attribute,
                              size_t count,
                              GLenum type,
                              bool normalized,
                              size_t relative_offset);

    /**
     * Configures the attribute with the specified location to pull data from the buffer bound to
     * the specified binding index.
     *
     * @param attribute
     * The attribute location to be bound.
     *
     * @param binding_index
     * The binding index whose bound buffer data for this attribute should be pulled from.
     */
    void set_attribute_binding(GLuint attribute, GLuint binding_index);

    /**
     * Enables or disables array access for the specified attribute.
     */
    void set_attribute_array_enabled(GLuint attribute, bool enabled);

    /**
     * Binds a buffer to the specified binding index.
     *
     * @param binding_index
     * The binding index to bind this buffer to. Should be equal to a binding index created via a
     * call to `set_attribute_format()`.
     *
     * @param buffer
     * The buffer to bind to this binding index.
     *
     * @param offset
     * The offset of the initial vertex record in the buffer, in bytes.
     *
     * @param stride
     * The total size of each vertex record in the buffer, in bytes.
     */
    void bind_buffer(GLuint binding_index,
                     const lineage::buffer& buffer,
                     size_t offset,
                     size_t stride);

    /**
     * Unbinds the currently bound buffer for the specified binding index.
     */
    void unbind_buffer(GLuint binding_index);

    /* -- Implementation -- */

  private:

    friend class opengl;

    const GLuint m_handle;

  };

  /**
   * Struct containing data required to initialize a vertex array attribute.
   */
  struct attribute_spec
  {
    size_t count;		/**< Number of values per attribute. */
    GLenum type;		/**< Type of value used. */
    bool normalized;		/**< If `true`, values will be normalized. */
    size_t relative_offset;	/**< Relative offset of values in vertex data. */
  };

}

/* -- Procedure Prototypes -- */

namespace lineage
{

  /**
   * Calls `set_attribute_format`, `set_attribute_binding`, and `set_attribute_array_enabled` on the
   * specified `lineage::vertex_array` to completely configure an attribute.
   *
   * @note
   * If the requested attribute name is not found, a warning will be printed to the debug log.
   */
  void configure_attribute(lineage::vertex_array& vao,
                           GLuint binding_index,
                           const lineage::shader_program& program,
                           const std::string& attribute_name,
                           const lineage::attribute_spec& spec,
                           bool enabled = true);

  /**
   * Same as `configure_attribute`, but allows directly specifying the attribute index.
   */
  void configure_attribute(lineage::vertex_array& vao,
                           GLuint binding_index,
                           GLuint attribute_index,
                           const lineage::attribute_spec& spec,
                           bool enabled = true);

}
