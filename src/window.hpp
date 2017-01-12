/**
 * @file	window.hpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/11
 */

/* -- Includes -- */

#include <string>
#include "api.hpp"

/* -- Types -- */

namespace lineage
{

  /**
   * Struct containing arguments required to build a `lineage::window` instance.
   */
  struct window_args
  {
    int context_version_major;
    int context_version_minor;
    int width;
    int height;
    std::string title;
    int swap_interval;
  };

  /**
   * Class representing the GLFW window used by the application.
   */
  class window
  {

    /* -- Lifecycle -- */

  public:

    /**
     * Creates a `lineage::window` instance with the specified arguments.
     *
     * @exception std::logic_error
     * Thrown if there is already an active window instance.
     *
     * @exception std::runtime_error
     * Thrown if any GLFW error occurs while attempting to create the window.
     */
    window(const lineage::window_args& args);

    window(lineage::window&& other) noexcept;
    ~window();

  private:

    window(const lineage::window&) = delete;
    lineage::window& operator =(const lineage::window&) = delete;
    lineage::window& operator =(lineage::window&&) = delete;

    /* -- Public Methods -- */

  public:

    /** Returns the GLFW version string. */
    std::string api_version() const
    {
      return std::string(glfwGetVersionString());
    }

    /** Returns the current GLFW elapsed time, in seconds. */
    double time() const
    {
      return glfwGetTime();
    }

    /** Polls the GLFW API for events. */
    void poll_events() const
    {
      glfwPollEvents();
    }

    /** Swaps the window's front and back buffers. */
    void swap_buffers()
    {
      glfwSwapBuffers(m_handle);
    }

    /** Returns the window's "should close" flag. */
    bool should_close() const
    {
      return static_cast<bool>(glfwWindowShouldClose(m_handle));
    }

    /** Sets the window's "should close" flag. */
    void set_should_close(bool should_close)
    {
      glfwSetWindowShouldClose(m_handle, static_cast<int>(should_close));
    }

    /* -- Implementation -- */

  private:

    static lineage::window* s_instance;
    static void error_callback(int error, const char* description);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

    GLFWwindow* m_handle;

  };

}
