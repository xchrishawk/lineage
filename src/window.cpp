/**
 * @file	window.cpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/11
 */

/* -- Includes -- */

#include <sstream>
#include <stdexcept>
#include <vector>

#include "api.hpp"
#include "debug.hpp"
#include "util.hpp"
#include "window.hpp"

/* -- Namespaces -- */

using namespace lineage;

/* -- Types -- */

struct window::implementation
{

  /* -- Fields -- */

  static window* s_instance;
  GLFWwindow* handle;
  std::vector<window_observer*> observers;

  /* -- Methods -- */

  /** GLFW error callback. */
  static void error_callback(int error, const char* description)
  {
#if defined(LINEAGE_DEBUG)
    std::ostringstream message;
    message << "GLFW error " << error << "! " << description;
    lineage_log_warning(message.str());
#endif
  }

  /** GLFW key event callback. */
  static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
  {
    if (s_instance == nullptr)
    {
      lineage_assert_fail("Received key callback with no active window!");
      return;
    }

    for (auto* const observer : s_instance->impl->observers)
      observer->window_key_event(key, action, mods);
  }

};

/* -- Variables -- */

window* window::implementation::s_instance = nullptr;

/* -- Procedures -- */

window::window(const window_args& args)
  : impl(std::make_unique<implementation>())
{
  if (implementation::s_instance != nullptr)
    throw std::logic_error("Only one window is allowed at a time!");
  implementation::s_instance = this;

  try
  {
    glfwSetErrorCallback(implementation::error_callback);
    if (!glfwInit())
      throw std::runtime_error("Failed to initialize GLFW!");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, args.context_version_major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, args.context_version_minor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, args.context_profile);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, static_cast<int>(args.context_forward_compatibility));

    impl->handle = glfwCreateWindow(args.width,
                                args.height,
                                args.title.c_str(),
                                nullptr,
                                nullptr);
    if (!impl->handle)
      throw std::runtime_error("Failed to create GLFW window!");

    glfwMakeContextCurrent(impl->handle);
    glfwSetKeyCallback(impl->handle, implementation::key_callback);
    glfwSwapInterval(args.swap_interval);

    lineage_log_status("GLFW initialized!", "API Version:\t\t\t" + api_version());
  }
  catch (...)
  {
    implementation::s_instance = nullptr;
    throw;
  }
}

window::~window()
{
  glfwDestroyWindow(impl->handle);
  glfwTerminate();

  implementation::s_instance = nullptr;
  lineage_log_status("GLFW terminated.");
}

std::string window::api_version() const
{
  return std::string(glfwGetVersionString());
}

double window::time() const
{
  return glfwGetTime();
}

void window::poll_events() const
{
  glfwPollEvents();
}

void window::swap_buffers()
{
  glfwSwapBuffers(impl->handle);
}

bool window::should_close() const
{
  return static_cast<bool>(glfwWindowShouldClose(impl->handle));
}

void window::set_should_close(bool should_close)
{
  glfwSetWindowShouldClose(impl->handle, static_cast<int>(should_close));
}

void window::window_size(int* width, int* height) const
{
  glfwGetWindowSize(impl->handle, width, height);
}

void window::framebuffer_size(int* width, int* height) const
{
  glfwGetFramebufferSize(impl->handle, width, height);
}

void window::add_observer(lineage::window_observer* observer) const
{
  impl->observers.push_back(observer);
}

void window::remove_observer(lineage::window_observer* observer) const
{
  remove_all(impl->observers, observer);
}
