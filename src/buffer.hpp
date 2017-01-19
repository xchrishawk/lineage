/**
 * @file	buffer.hpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/13
 */

#pragma once

/* -- Includes -- */

#include <limits>
#include <utility>

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

    /* -- Constants -- */

  public:

    /** Invalid buffer handle. */
    static const GLuint invalid_handle = std::numeric_limits<GLuint>::max();

    /* -- Lifecycle -- */

  protected:

    /**
     * Constructs a new `lineage::buffer` object.
     */
    buffer()
      : m_handle(new_buffer_handle())
    {
      if (m_handle == invalid_handle)
        opengl_error::throw_last_error();
    }

  public:

    /**
     * Move constructor.
     */
    buffer(lineage::buffer&& other) noexcept
      : m_handle(invalid_handle)
    {
      std::swap(m_handle, other.m_handle);
    }

    /**
     * Move assignment operator.
     */
    lineage::buffer& operator =(lineage::buffer&& other) noexcept
    {
      m_handle = other.m_handle;
      other.m_handle = invalid_handle;
      return *this;
    }

    /**
     * Destructor.
     */
    virtual ~buffer()
    {
      if (m_handle == invalid_handle)
        return;
      glDeleteBuffers(1, &m_handle);
    }

  private:

    buffer(const lineage::buffer&) = delete;
    lineage::buffer& operator =(const lineage::buffer&) = delete;

    /* -- Public Methods -- */

  public:

    /**
     * Returns a subset of this buffer object's data.
     */
    void get_data(size_t offset, size_t size, void* data) const
    {
      glGetNamedBufferSubData(m_handle, offset, size, data);
    }

    /**
     * Updates the data contained in this buffer object.
     */
    void set_data(size_t offset, size_t size, const void* data)
    {
      glNamedBufferSubData(m_handle, offset, size, data);
    }

    /**
     * Maps this buffer to memory for direct access.
     */
    void* map(GLenum access)
    {
      return glMapNamedBuffer(m_handle, access);
    }

    /**
     * Unmaps this buffer.
     */
    void unmap()
    {
      glUnmapNamedBuffer(m_handle);
    }

    /**
     * Returns `true` if the `GL_BUFFER_IMMUTABLE_STORAGE` parameter is `GL_TRUE` for this buffer.
     */
    bool is_immutable() const
    {
      return static_cast<bool>(get_buffer_parameter(GL_BUFFER_IMMUTABLE_STORAGE));
    }

    /**
     * Returns the value of the `GL_BUFFER_SIZE` parameter for this buffer.
     */
    size_t size() const
    {
      return static_cast<size_t>(get_buffer_parameter_64(GL_BUFFER_SIZE));
    }

    /**
     * Returns the value of the `GL_BUFFER_STORAGE_FLAGS` parameter for this buffer.
     */
    GLbitfield storage_flags() const
    {
      return static_cast<GLbitfield>(get_buffer_parameter(GL_BUFFER_STORAGE_FLAGS));
    }

    /**
     * Returns the value of the `GL_BUFFER_USAGE` parameter for this buffer.
     */
    GLenum usage() const
    {
      return static_cast<GLenum>(get_buffer_parameter(GL_BUFFER_USAGE));
    }

    /**
     * Returns `true` if the `GL_BUFFER_MAPPED` parameter is `GL_TRUE` for this buffer.
     */
    bool is_mapped() const
    {
      return static_cast<bool>(get_buffer_parameter(GL_BUFFER_MAPPED));
    }

    /**
     * Returns the value of the `GL_BUFFER_MAP_OFFSET` parameter for this buffer.
     */
    size_t map_offset() const
    {
      return static_cast<size_t>(get_buffer_parameter(GL_BUFFER_MAP_OFFSET));
    }

    /**
     * Returns the value of the `GL_BUFFER_MAP_LENGTH` parameter for this buffer.
     */
    size_t map_size() const
    {
      return static_cast<size_t>(get_buffer_parameter(GL_BUFFER_MAP_LENGTH));
    }

    /**
     * Returns the value of the `GL_BUFFER_ACCESS` buffer parameter.
     */
    GLenum map_access() const
    {
      return static_cast<GLenum>(get_buffer_parameter(GL_BUFFER_ACCESS));
    }

    /* -- Implementation -- */

  protected:

    friend class vertex_array;

    GLuint m_handle;

    static GLuint new_buffer_handle();
    GLint get_buffer_parameter(GLenum param) const;
    GLint64 get_buffer_parameter_64(GLenum param) const;

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
    immutable_buffer(size_t size, const void* data, GLbitfield flags)
      : lineage::buffer()
    {
      glNamedBufferStorage(m_handle, size, data, flags);
    }

    /**
     * Move constructor.
     */
    immutable_buffer(lineage::immutable_buffer&& other)
      : lineage::buffer(std::move(other))
    { }

    /**
     * Destructor.
     */
    virtual ~immutable_buffer() = default;

  };

}
