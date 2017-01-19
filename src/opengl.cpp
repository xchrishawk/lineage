/**
 * @file	opengl.cpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/11
 */

/* -- Includes -- */

#include <stdexcept>
#include <string>
#include <vector>

#include <glm/gtc/type_ptr.hpp>

#include "api.hpp"
#include "debug.hpp"
#include "opengl.hpp"
#include "opengl_error.hpp"
#include "shader_program.hpp"
#include "vertex_array.hpp"

/* -- Namespaces -- */

using namespace std::string_literals;
using namespace lineage;

/* -- Types -- */

struct opengl::implementation
{

  /* -- Fields -- */

  static opengl* s_instance;

  std::vector<GLuint> programs;
  std::vector<GLuint> vertex_arrays;

  /* -- Procedures -- */

  /** Get the OpenGL string with the specified name. */
  static std::string get_string(GLenum name)
  {
    const GLubyte* ustr = glGetString(name);
    const char* cstr = reinterpret_cast<const char*>(ustr);
    return std::string(cstr);
  }

};

/* -- Variables -- */

opengl* opengl::implementation::s_instance = nullptr;

/* -- Procedures -- */

opengl::opengl()
  : impl(std::make_unique<implementation>())
{
  if (implementation::s_instance)
    throw std::logic_error("Attempted to initialize OpenGL while it was already initialized!");
  implementation::s_instance = this;

  try
  {
    glewExperimental = GL_TRUE;
    if (glewInit() != GL_NO_ERROR)
      throw std::runtime_error("Failed to initialize GLEW!");

    // there is a bug where GLEW triggers an error on init. flush it from the queue.
    // http://stackoverflow.com/q/20034615/434245
    GLenum error __attribute__((unused)) = opengl_error::last_error();
    lineage_assert(error == GL_INVALID_ENUM);

    lineage_log_status("OpenGL initialized.",
                       "API Version:\t\t\t" + api_version(),
                       "GLSL Version:\t\t\t" + shading_language_version(),
                       "Renderer:\t\t\t" + renderer(),
                       "Vendor:\t\t\t" + vendor(),
                       "ARB_buffer_storage:\t\t"
                         + (is_supported("GL_ARB_buffer_storage") ? "supported"s : "not supported"s),
                       "ARB_direct_state_access:\t"
                         + (is_supported("GL_ARB_direct_state_access") ? "supported"s : "not supported"s));
  }
  catch (...)
  {
    implementation::s_instance = nullptr;
    throw;
  }
}

opengl::~opengl()
{
  implementation::s_instance = nullptr;
  lineage_log_status("OpenGL terminated.");
}

std::string opengl::api_version() const
{
  return implementation::get_string(GL_VERSION);
}

std::string opengl::shading_language_version() const
{
  return implementation::get_string(GL_SHADING_LANGUAGE_VERSION);
}

std::string opengl::renderer() const
{
  return implementation::get_string(GL_RENDERER);
}

std::string opengl::vendor() const
{
  return implementation::get_string(GL_VENDOR);
}

bool opengl::is_supported(const std::string& extension) const
{
  return (glewIsSupported(extension.c_str()) == GL_TRUE);
}

void opengl::set_uniform(GLuint uniform, const glm::mat4& matrix)
{
  glUniformMatrix4fv(uniform,			// location
                     1,				// count
                     GL_FALSE,			// transpose
                     glm::value_ptr(matrix));	// value
}

void opengl::push_program(const shader_program& program)
{
  impl->programs.push_back(program.m_handle);
  glUseProgram(program.m_handle);
}

void opengl::pop_program()
{
  if (impl->programs.empty())
  {
    lineage_assert_fail("Attempted to pop shader program with no active shader program!");
    return;
  }
  impl->programs.pop_back();
  glUseProgram(impl->programs.empty() ? 0 : impl->programs.back());
}

void opengl::push_vertex_array(const vertex_array& vao)
{
  impl->vertex_arrays.push_back(vao.m_handle);
  glBindVertexArray(vao.m_handle);
}

void opengl::pop_vertex_array()
{
  if (impl->vertex_arrays.empty())
  {
    lineage_assert_fail("Attempted to pop vertex array with no active vertex array!");
    return;
  }
  impl->vertex_arrays.pop_back();
  glBindVertexArray(impl->vertex_arrays.empty() ? 0 : impl->vertex_arrays.back());
}
