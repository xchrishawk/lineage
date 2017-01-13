/**
 * @file	opengl_error.cpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/12
 */

/* -- Includes -- */

#include "api.hpp"
#include "debug.hpp"
#include "opengl_error.hpp"

/* -- Namespaces -- */

using namespace lineage;

/* -- Procedures -- */

GLenum opengl_error::last_error()
{
  GLenum last_error = GL_NO_ERROR;
  GLenum this_error = GL_NO_ERROR;

  while ((this_error = glGetError()) != GL_NO_ERROR)
  {
#if defined(LINEAGE_DEBUG)
    if (last_error != GL_NO_ERROR)
    {
      // we are about to lose this error forever, so print it so someone hears about it
      lineage_log_warning("Suppressing OpenGL error! " + error_string(last_error));
    }
#endif
    last_error = this_error;
  }

  return last_error;
}

void opengl_error::throw_last_error()
{
  throw opengl_error(last_error());
}

const std::string& opengl_error::error_string(GLenum error) noexcept
{
  static const std::string GL_NO_ERROR_STRING
    = "GL_NO_ERROR (0x0000)";
  static const std::string GL_INVALID_ENUM_STRING
    = "GL_INVALID_ENUM (0x0500)";
  static const std::string GL_INVALID_VALUE_STRING
    = "GL_INVALID_VALUE (0x0501)";
  static const std::string GL_INVALID_OPERATION_STRING
    = "GL_INVALID_OPERATION (0x0502)";
  static const std::string GL_STACK_OVERFLOW_STRING
    = "GL_STACK_OVERFLOW (0x0503)";
  static const std::string GL_STACK_UNDERFLOW_STRING
    = "GL_STACK_UNDERFLOW (0x0504)";
  static const std::string GL_OUT_OF_MEMORY_STRING
    = "GL_OUT_OF_MEMORY (0x0505)";
  static const std::string GL_INVALID_FRAMEBUFFER_OPERATION_STRING
    = "GL_INVALID_FRAMEBUFFER_OPERATION (0x0506)";
  static const std::string UNKNOWN_ERROR_STRING
    = "Unknown Error";

  switch (error)
  {
  case GL_NO_ERROR:				return GL_NO_ERROR_STRING;
  case GL_INVALID_ENUM:				return GL_INVALID_ENUM_STRING;
  case GL_INVALID_VALUE:			return GL_INVALID_VALUE_STRING;
  case GL_INVALID_OPERATION:			return GL_INVALID_OPERATION_STRING;
  case GL_STACK_OVERFLOW:			return GL_STACK_OVERFLOW_STRING;
  case GL_STACK_UNDERFLOW:			return GL_STACK_UNDERFLOW_STRING;
  case GL_OUT_OF_MEMORY:			return GL_OUT_OF_MEMORY_STRING;
  case GL_INVALID_FRAMEBUFFER_OPERATION:	return GL_INVALID_FRAMEBUFFER_OPERATION_STRING;
  default:					return UNKNOWN_ERROR_STRING;
  }
}
