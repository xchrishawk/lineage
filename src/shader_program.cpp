/**
 * @file	shader_program.cpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/13
 */

/* -- Includes -- */

#include <memory>
#include <sstream>
#include <utility>

#include "api.hpp"
#include "opengl_error.hpp"
#include "shader.hpp"
#include "shader_program.hpp"

/* -- Namespaces -- */

using namespace lineage;

/* -- Constants -- */

namespace
{
  const GLuint INVALID_HANDLE = 0;
}

/* -- Private Procedures -- */

namespace
{

  /** Gets the specified program info variable. */
  GLint get_program_info(GLuint handle, GLenum type)
  {
    GLint value = 0;
    glGetProgramiv(handle, type, &value);
    return value;
  }

}

/* -- Procedures -- */

shader_program::shader_program()
  : m_handle(glCreateProgram())
{
  if (m_handle == INVALID_HANDLE)
    opengl_error::throw_last_error();
}

shader_program::shader_program(shader_program&& other) noexcept
  : m_handle(INVALID_HANDLE)
{
  std::swap(m_handle, other.m_handle);
}

shader_program::~shader_program()
{
  if (m_handle == INVALID_HANDLE)
    return;
  glDeleteProgram(m_handle);
}

void shader_program::attach_shader(const shader& shader)
{
  glAttachShader(m_handle, shader.m_handle);
}

void shader_program::detach_shader(const shader& shader)
{
  glDetachShader(m_handle, shader.m_handle);
}

void shader_program::link()
{
  glLinkProgram(m_handle);
  if (!is_linked())
  {
    auto error = opengl_error::last_error();
    if (error != GL_NO_ERROR)
      throw opengl_error(error);
    else
    {
      std::ostringstream message;
      message << "Failed to compile shader program! " << std::endl;
      message << info_log();
      throw shader_program_link_error(message.str());
    }
  }
}

bool shader_program::is_linked() const
{
  return (get_program_info(m_handle, GL_LINK_STATUS) != GL_FALSE);
}

GLint shader_program::attached_shader_count() const
{
  return get_program_info(m_handle, GL_ATTACHED_SHADERS);
}

GLint shader_program::active_attribute_count() const
{
  return get_program_info(m_handle, GL_ACTIVE_ATTRIBUTES);
}

GLint shader_program::active_uniform_count() const
{
  return get_program_info(m_handle, GL_ACTIVE_UNIFORMS);
}

std::string shader_program::info_log() const
{
  GLint info_log_length = 0;
  glGetProgramiv(m_handle, GL_INFO_LOG_LENGTH, &info_log_length);
  if (info_log_length == 0)
    return "";

  std::unique_ptr<char> info_log_buffer(new char[info_log_length]);
  glGetProgramInfoLog(m_handle, info_log_length, NULL, info_log_buffer.get());
  std::string info_log(info_log_buffer.get());

  return info_log;
}

GLint shader_program::attribute_location(const std::string& name) const
{
  return glGetAttribLocation(m_handle, name.c_str());
}

GLint shader_program::uniform_location(const std::string& name) const
{
  return glGetUniformLocation(m_handle, name.c_str());
}

shader_program lineage::create_shader_program(const std::initializer_list<shader*>& shaders)
{
  shader_program program;

  for (const auto* shader : shaders)
    program.attach_shader(*shader);

  program.link();

  for (const auto* shader : shaders)
    program.detach_shader(*shader);

  return program;
}
