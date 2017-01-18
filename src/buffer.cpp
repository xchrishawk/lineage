/**
 * @file	buffer.cpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/13
 */

/* -- Includes -- */

#include <limits>

#include "buffer.hpp"
#include "opengl_error.hpp"

/* -- Namespaces -- */

using namespace lineage;

/* -- Procedures -- */

GLuint buffer::new_buffer_handle()
{
  GLuint handle = invalid_handle;
  glCreateBuffers(1, &handle);
  return handle;
}

GLint buffer::get_buffer_parameter(GLenum param) const
{
  GLint value = 0;
  glGetNamedBufferParameteriv(m_handle, param, &value);
  return value;
}

GLint64 buffer::get_buffer_parameter_64(GLenum param) const
{
  GLint64 value = 0;
  glGetNamedBufferParameteri64v(m_handle, param, &value);
  return value;
}
