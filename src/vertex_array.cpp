/**
 * @file	vertex_array.cpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/14
 */

/* -- Includes -- */

#include <limits>

#include "api.hpp"
#include "buffer.hpp"
#include "opengl_error.hpp"
#include "shader_program.hpp"
#include "vertex_array.hpp"

/* -- Namespaces -- */

using namespace lineage;

/* -- Constants -- */

namespace
{
  const GLuint INVALID_HANDLE = std::numeric_limits<GLuint>::max();
}

/* -- Private Procedures -- */

namespace
{

  /** Create a new vertex array handle. */
  GLuint create_vertex_array()
  {
    GLuint handle = INVALID_HANDLE;
    glCreateVertexArrays(1, &handle);
    return handle;
  }

}

/* -- Procedures -- */

vertex_array::vertex_array()
  : m_handle(create_vertex_array())
{
  if (m_handle == INVALID_HANDLE)
    opengl_error::throw_last_error();
}

vertex_array::vertex_array(lineage::vertex_array&& other) noexcept
  : m_handle(INVALID_HANDLE)
{
  std::swap(m_handle, other.m_handle);
}

vertex_array::~vertex_array()
{
  if (m_handle == INVALID_HANDLE)
    return;
  glDeleteVertexArrays(1, &m_handle);
}

void vertex_array::set_attribute_format(GLuint attribute, size_t size, GLenum type, bool normalized, size_t relative_offset)
{
  glVertexArrayAttribFormat(m_handle,					// vaobj
                            attribute,					// attribindex
                            static_cast<GLint>(size),			// size
                            type,					// type
                            normalized ? GL_TRUE : GL_FALSE,		// normalized
                            static_cast<GLuint>(relative_offset));	// relativeoffset
}

void vertex_array::set_attribute_binding(GLuint attribute, GLuint binding_index)
{
  glVertexArrayAttribBinding(m_handle,					// vaobj
                             attribute,					// attribindex
                             binding_index);				// bindingindex
}

void vertex_array::set_attribute_array_enabled(GLuint attribute, bool enabled)
{
  if (enabled)
  {
    glEnableVertexArrayAttrib(m_handle,					// vaobj
                              attribute);				// index
  }
  else
  {
    glDisableVertexArrayAttrib(m_handle,				// vaobj
                               attribute);				// index
  }
}

void vertex_array::bind_buffer(GLuint binding_index, const buffer& buffer, size_t offset, size_t stride)
{
  glVertexArrayVertexBuffer(m_handle,					// vaobj
                            binding_index,				// bindingindex
                            buffer.m_handle,				// buffer
                            static_cast<GLintptr>(offset),		// offset
                            static_cast<GLintptr>(stride));		// stride
}

void vertex_array::unbind_buffer(GLuint binding_index)
{
  glVertexArrayVertexBuffer(m_handle,					// vaobj
                            binding_index,				// bindingindex
                            0,						// buffer
                            0,						// offset
                            0);						// stride
}

void lineage::vertex_array_initialize_attribute(vertex_array& vao,
                                                const shader_program& program,
                                                const std::string& attribute_name,
                                                const vertex_array_attrib_spec& spec)
{
  auto attribute_index = program.attribute_location(attribute_name);
  if (attribute_index != shader_program::invalid_location)
    vertex_array_initialize_attribute(vao, attribute_index, spec);
  else
    lineage_log_warning("Vertex attribute " + attribute_name + " not found in shader program!");
}


void lineage::vertex_array_initialize_attribute(vertex_array& vao,
                                                GLuint attribute_index,
                                                const vertex_array_attrib_spec& spec)
{
  vao.set_attribute_format(attribute_index, spec.size, spec.type, spec.normalized, spec.relative_offset);
  vao.set_attribute_binding(attribute_index, spec.binding_index);
  vao.set_attribute_array_enabled(attribute_index, spec.enabled);
}
