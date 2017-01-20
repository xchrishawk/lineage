/**
 * @file	buffer.hpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/13
 */

#pragma once

/* -- Includes -- */

#include "api.hpp"
#include "opengl_error.hpp"

/* -- Types -- */

namespace lineage
{

  /**
   * Abstract base class for types representing an OpenGL buffer.
   */
  class buffer
  {

    /* -- Lifecycle -- */

  protected:

    /**
     * Constructs a new `lineage::buffer` object.
     */
    buffer();

  public:

    /**
     * Destructor.
     */
    virtual ~buffer();

  private:

    buffer(const lineage::buffer&) = delete;
    buffer(lineage::buffer&&) = delete;
    lineage::buffer& operator =(const lineage::buffer&) = delete;
    lineage::buffer& operator =(lineage::buffer&&) = delete;

    /* -- Public Methods -- */

  public:

    /**
     * Returns a subset of this buffer object's data.
     */
    void get_data(size_t offset, size_t size, void* data) const;

    /**
     * Updates the data contained in this buffer object.
     */
    void set_data(size_t offset, size_t size, const void* data);

    /**
     * Maps this buffer to memory for direct access.
     */
    void* map(GLenum access);

    /**
     * Unmaps this buffer.
     */
    void unmap();

    /**
     * Returns `true` if the `GL_BUFFER_IMMUTABLE_STORAGE` parameter is `GL_TRUE` for this buffer.
     */
    bool is_immutable() const;

    /**
     * Returns the value of the `GL_BUFFER_SIZE` parameter for this buffer.
     */
    size_t size() const;

    /**
     * Returns the value of the `GL_BUFFER_STORAGE_FLAGS` parameter for this buffer.
     */
    GLbitfield storage_flags() const;

    /**
     * Returns the value of the `GL_BUFFER_USAGE` parameter for this buffer.
     */
    GLenum usage() const;

    /**
     * Returns `true` if the `GL_BUFFER_MAPPED` parameter is `GL_TRUE` for this buffer.
     */
    bool is_mapped() const;

    /**
     * Returns the value of the `GL_BUFFER_MAP_OFFSET` parameter for this buffer.
     */
    size_t map_offset() const;

    /**
     * Returns the value of the `GL_BUFFER_MAP_LENGTH` parameter for this buffer.
     */
    size_t map_size() const;

    /**
     * Returns the value of the `GL_BUFFER_ACCESS` buffer parameter.
     */
    GLenum map_access() const;

    /* -- Implementation -- */

  protected:

    friend class vertex_array;

    GLuint m_handle;

  };

  /**
   * Class representing an OpenGL "immutable" buffer.
   *
   * @note
   * The buffer is immutable in that it cannot be moved or resized once it has been created. The
   * data contained in the buffer may be modified, however.
   */
  class immutable_buffer final : public buffer
  {

    /* -- Lifecycle -- */

  public:

    /**
     * Constructs a new `lineage::immutable_buffer` instance.
     *
     * @param size
     * The size of the data buffer.
     *
     * @param data
     * The data to initialize the buffer with.
     *
     * @param flags
     * The storage flags for this buffer.
     *
     * @note
     * This initializes the buffer using a call to `glNamedBufferStorage()`.
     */
    immutable_buffer(size_t size, const void* data, GLbitfield flags);

    /**
     * Destructor.
     */
    virtual ~immutable_buffer() = default;

  private:

    immutable_buffer(const lineage::immutable_buffer&) = delete;
    immutable_buffer(lineage::immutable_buffer&&) = delete;
    lineage::immutable_buffer& operator =(const lineage::immutable_buffer&) = delete;
    lineage::immutable_buffer& operator =(lineage::immutable_buffer&&) = delete;

  };

}
