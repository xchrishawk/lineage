/**
 * @file	window.cpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/11
 */

/* -- Includes -- */

#include <sstream>
#include <stdexcept>

#include "api.hpp"
#include "debug.hpp"
#include "window.hpp"

/* -- Namespaces -- */

using namespace lineage;

/* -- Variables -- */

window* window::s_instance = nullptr;

/* -- Procedures -- */

window::window(const window_args& args)
  : m_handle(nullptr),
    m_observers()
{
  if (s_instance != nullptr)
    throw std::logic_error("Only one window is allowed at a time!");
  s_instance = this;

  try
  {
    glfwSetErrorCallback(window::error_callback);
    if (!glfwInit())
      throw std::runtime_error("Failed to initialize GLFW!");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, args.context_version_major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, args.context_version_minor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, args.context_profile);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, static_cast<int>(args.context_forward_compatibility));

    m_handle = glfwCreateWindow(args.width,
                                args.height,
                                args.title.c_str(),
                                nullptr,
                                nullptr);
    if (!m_handle)
      throw std::runtime_error("Failed to create GLFW window!");

    glfwMakeContextCurrent(m_handle);
    glfwSetKeyCallback(m_handle, window::key_callback);
    glfwSwapInterval(args.swap_interval);

    lineage_log_status("GLFW initialized!", "API Version:\t\t" + api_version());
  }
  catch (...)
  {
    s_instance = nullptr;
    throw;
  }
}

window::window(window&& other) noexcept
  : m_handle(nullptr),
    m_observers()
{
  std::swap(m_handle, other.m_handle);
  std::swap(m_observers, other.m_observers);
  s_instance = this;
}

window::~window()
{
  if (this != s_instance)
    return;

  glfwDestroyWindow(m_handle);
  glfwTerminate();

  s_instance = nullptr;
  lineage_log_status("GLFW terminated.");
}

void window::error_callback(int error, const char* description)
{
#if defined(LINEAGE_DEBUG)
  std::ostringstream message;
  message << "GLFW error " << error << "! " << description;
  lineage_log_warning(message.str());
#endif
}

void window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (s_instance == nullptr)
  {
    lineage_assert_fail("Received key callback with no active window!");
    return;
  }

  for (auto* const observer : s_instance->m_observers)
    observer->window_key_event(key, action, mods);
}
