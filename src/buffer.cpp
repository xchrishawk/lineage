/**
 * @file	buffer.cpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/13
 */

/* -- Includes -- */

#include "buffer.hpp"

/* -- Namespaces -- */

using namespace lineage;

/* -- Private Procedures -- */

namespace
{

  /** Generate a new OpenGL buffer handle. */
  GLuint new_buffer_handle()
  {
    GLuint handle = 0;
    glCreateBuffers(1, &handle);
    return handle;
  }

  /** Get the specified buffer parameter. */
  GLint get_buffer_parameter(GLuint handle, GLenum param)
  {
    GLint value = 0;
    glGetNamedBufferParameteriv(handle, param, &value);
    return value;
  }

  /** Get the specified 64-bit buffer parameter. */
  GLint64 get_buffer_parameter_64(GLuint handle, GLenum param)
  {
    GLint64 value = 0;
    glGetNamedBufferParameteri64v(handle, param, &value);
    return value;
  }

}

/* -- Procedures -- */

buffer::buffer()
  : m_handle(new_buffer_handle())
{
}

buffer::~buffer()
{
  glDeleteBuffers(1, &m_handle);
}

void buffer::get_data(size_t offset, size_t size, void* data) const
{
  glGetNamedBufferSubData(m_handle, offset, size, data);
}

void buffer::set_data(size_t offset, size_t size, const void* data)
{
  glNamedBufferSubData(m_handle, offset, size, data);
}

void* buffer::map(GLenum access)
{
  return glMapNamedBuffer(m_handle, access);
}

void buffer::unmap()
{
  glUnmapNamedBuffer(m_handle);
}

bool buffer::is_immutable() const
{
  return static_cast<bool>(get_buffer_parameter(m_handle, GL_BUFFER_IMMUTABLE_STORAGE));
}

size_t buffer::size() const
{
  return static_cast<size_t>(get_buffer_parameter_64(m_handle, GL_BUFFER_SIZE));
}

GLbitfield buffer::storage_flags() const
{
  return static_cast<GLbitfield>(get_buffer_parameter(m_handle, GL_BUFFER_STORAGE_FLAGS));
}

GLenum buffer::usage() const
{
  return static_cast<GLenum>(get_buffer_parameter(m_handle, GL_BUFFER_USAGE));
}

bool buffer::is_mapped() const
{
  return static_cast<bool>(get_buffer_parameter(m_handle, GL_BUFFER_MAPPED));
}

size_t buffer::map_offset() const
{
  return static_cast<size_t>(get_buffer_parameter(m_handle, GL_BUFFER_MAP_OFFSET));
}

size_t buffer::map_size() const
{
  return static_cast<size_t>(get_buffer_parameter(m_handle, GL_BUFFER_MAP_LENGTH));
}

GLenum buffer::map_access() const
{
  return static_cast<GLenum>(get_buffer_parameter(m_handle, GL_BUFFER_ACCESS));
}

immutable_buffer::immutable_buffer(size_t size, const void* data, GLenum usage)
  : buffer()
{
  glNamedBufferStorage(m_handle, size, data, usage);
}
