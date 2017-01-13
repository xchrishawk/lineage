/**
 * @file	shader.cpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/12
 */

/* -- Includes -- */

#include <memory>
#include <sstream>

#include "api.hpp"
#include "opengl_error.hpp"
#include "shader.hpp"

/* -- Namespaces -- */

using namespace lineage;

/* -- Constants -- */

namespace
{
  const GLuint INVALID_HANDLE = 0;
}

/* -- Procedures -- */

shader::shader(GLenum type)
  : m_type(type),
    m_handle(glCreateShader(type))
{
  if (m_handle == INVALID_HANDLE)
    opengl_error::throw_last_error();
}

shader::~shader()
{
  glDeleteShader(m_handle);
}

void shader::set_source(const std::string& source)
{
  auto c_str = source.c_str();
  glShaderSource(m_handle, 1, &c_str, nullptr);
}

void shader::compile()
{
  glCompileShader(m_handle);
  if (!is_compiled())
  {
    auto error = opengl_error::last_error();
    if (error != GL_NO_ERROR)
      throw opengl_error(error);
    else
    {
      std::ostringstream message;
      message << "Failed to compile shader of type " << type_string() << "! " << std::endl;
      message << info_log();
      throw shader_compile_error(message.str());
    }
  }
}

bool shader::is_compiled() const
{
  GLint success = GL_FALSE;
  glGetShaderiv(m_handle, GL_COMPILE_STATUS, &success);
  return (success != GL_FALSE);
}

std::string shader::info_log() const
{
  GLint info_log_length = 0;
  glGetShaderiv(m_handle, GL_INFO_LOG_LENGTH, &info_log_length);
  if (info_log_length == 0)
    return "";

  std::unique_ptr<char> info_log_buffer(new char[info_log_length]);
  glGetShaderInfoLog(m_handle, info_log_length, NULL, info_log_buffer.get());
  std::string info_log(info_log_buffer.get());

  return info_log;
}

const std::string& shader::type_string(GLenum shader_type)
{
  static const std::string GL_COMPUTE_SHADER_STRING 		= "GL_COMPUTE_SHADER";
  static const std::string GL_FRAGMENT_SHADER_STRING 		= "GL_FRAGMENT_SHADER";
  static const std::string GL_GEOMETRY_SHADER_STRING 		= "GL_GEOMETRY_SHADER";
  static const std::string GL_TESS_CONTROL_SHADER_STRING 	= "GL_TESS_CONTROL_SHADER";
  static const std::string GL_TESS_EVALUATION_SHADER_STRING	= "GL_TESS_EVALUATION_SHADER";
  static const std::string GL_VERTEX_SHADER_STRING 		= "GL_VERTEX_SHADER";
  static const std::string UNKNOWN_SHADER_STRING 		= "Unknown Shader Type";

  switch (shader_type)
  {
  case GL_COMPUTE_SHADER:		return GL_COMPUTE_SHADER_STRING;
  case GL_FRAGMENT_SHADER:		return GL_FRAGMENT_SHADER_STRING;
  case GL_GEOMETRY_SHADER:		return GL_GEOMETRY_SHADER_STRING;
  case GL_TESS_CONTROL_SHADER:		return GL_TESS_CONTROL_SHADER_STRING;
  case GL_TESS_EVALUATION_SHADER:	return GL_TESS_EVALUATION_SHADER_STRING;
  case GL_VERTEX_SHADER:		return GL_VERTEX_SHADER_STRING;
  default:				return UNKNOWN_SHADER_STRING;
  }
}
