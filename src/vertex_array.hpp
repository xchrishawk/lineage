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

    vertex_array();
    vertex_array(lineage::vertex_array&& other) noexcept;
    ~vertex_array();

  private:

    vertex_array(const lineage::vertex_array&) = delete;
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
     * @param size
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
    void set_attribute_format(GLuint attribute, size_t size, GLenum type, bool normalized, size_t relative_offset);

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
     * The offset of the initial vertex record in the buffer.
     *
     * @param stride
     * The total size of each vertex record in the buffer.
     */
    void bind_buffer(GLuint binding_index, const lineage::buffer& buffer, size_t offset, size_t stride);

    /**
     * Unbinds the currently bound buffer for the specified binding index.
     */
    void unbind_buffer(GLuint binding_index);

    /* -- Implementation -- */

  private:

    friend class opengl;

    GLuint m_handle;

  };

  /**
   * Struct containing data required to initialize a vertex array attribute.
   */
  struct vertex_array_attrib_spec
  {
    GLuint binding_index;
    size_t size;
    GLenum type;
    bool normalized;
    size_t relative_offset;
    bool enabled;
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
  void vertex_array_initialize_attribute(lineage::vertex_array& vao,
                                         const lineage::shader_program& program,
                                         const std::string& attribute_name,
                                         const lineage::vertex_array_attrib_spec& spec);

  /**
   * Same as `vertex_array_initialize_attribute`, but allows directly specifying the attribute index.
   */
  void vertex_array_initialize_attribute(lineage::vertex_array& vao,
                                         GLuint attribute_index,
                                         const lineage::vertex_array_attrib_spec& spec);

}
