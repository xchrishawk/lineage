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
    glGenBuffers(1, &handle);
    return handle;
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
