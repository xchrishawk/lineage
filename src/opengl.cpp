/**
 * @file	opengl.cpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/11
 */

/* -- Includes -- */

#include <stdexcept>

#include "api.hpp"
#include "debug.hpp"
#include "opengl.hpp"

/* -- Namespaces -- */

using namespace lineage;

/* -- Variables -- */

opengl* opengl::s_instance = nullptr;

/* -- Procedures -- */

opengl::opengl()
{
  if (s_instance)
    throw std::logic_error("Attempted to initialize OpenGL while it was already initialized!");
  s_instance = this;

  try
  {
    glewExperimental = GL_TRUE;
    if (glewInit() != GL_NO_ERROR)
      throw std::runtime_error("Failed to initialize GLEW!");

    lineage_log_status("OpenGL initialized.",
                       "API Version:\t\t" + api_version(),
                       "GLSL Version:\t\t" + shading_language_version(),
                       "Renderer:\t\t" + renderer(),
                       "Vendor:\t\t" + vendor());
  }
  catch (...)
  {
    s_instance = nullptr;
    throw;
  }
}

opengl::~opengl()
{
  if (this != s_instance)
    return;

  lineage_log_status("OpenGL terminated.");
}

opengl::opengl(opengl&& other)
{
  s_instance = this;
}
