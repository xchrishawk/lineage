/**
 * @file	window.hpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/11
 */

#pragma once

/* -- Includes -- */

#include <string>
#include <vector>

#include "api.hpp"
#include "util.hpp"

/* -- Types -- */

namespace lineage
{

  /**
   * Abstract interface for classes observing events from a `lineage::window`.
   */
  class window_observer
  {
  public:

    /**
     * The window received a key press event.
     *
     * @param key
     * The key that was pressed or release.
     *
     * @param action
     * The action that was taken on the key.
     *
     * @param mods
     * The modifiers (if any) that are held down.
     */
    virtual void window_key_event(int key, int action, int mods) = 0;

  };

  /**
   * Struct containing arguments required to build a `lineage::window` instance.
   */
  struct window_args
  {

    /* -- OpenGL Configuration -- */

    int context_version_major;		/**< OpenGL context major version. */
    int context_version_minor;		/**< OpenGL context minor version. */
    int context_profile;		/**< OpenGL context profile to use. */
    bool context_forward_compatibility;	/**< If `true`, OpenGL context should be forward compatible. */

    /* -- Window Configuration -- */

    int width;				/**< Initial width of window. */
    int height;				/**< Initial height of window. */
    std::string title;			/**< Initial title of window. */
    int swap_interval;			/**< Swap interval to use. */

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

    ~window();

  private:

    window(const lineage::window&) = delete;
    window(lineage::window&&) = delete;
    lineage::window& operator =(const lineage::window&) = delete;
    lineage::window& operator =(lineage::window&&) = delete;

    /* -- Public Methods -- */

  public:

    /**
     * Returns the GLFW version string.
     */
    std::string api_version() const
    {
      return std::string(glfwGetVersionString());
    }

    /**
     * Returns the current GLFW elapsed time, in seconds.
     */
    double time() const
    {
      return glfwGetTime();
    }

    /**
     * Polls the GLFW API for events.
     */
    void poll_events() const
    {
      glfwPollEvents();
    }

    /**
     * Swaps the window's front and back buffers.
     */
    void swap_buffers()
    {
      glfwSwapBuffers(m_handle);
    }

    /**
     * Returns the window's "should close" flag.
     */
    bool should_close() const
    {
      return static_cast<bool>(glfwWindowShouldClose(m_handle));
    }

    /**
     * Sets the window's "should close" flag.
     */
    void set_should_close(bool should_close)
    {
      glfwSetWindowShouldClose(m_handle, static_cast<int>(should_close));
    }

    /**
     * Gets the size of the window.
     */
    void window_size(int* width, int* height) const
    {
      glfwGetWindowSize(m_handle, width, height);
    }

    /**
     * Gets the size of the window's frame buffer.
     */
    void framebuffer_size(int* width, int* height) const
    {
      glfwGetFramebufferSize(m_handle, width, height);
    }

    /**
     * Adds an observer to this window.
     */
    void add_observer(lineage::window_observer* observer) const
    {
      m_observers.push_back(observer);
    }

    /**
     * Removes an observer from this window.
     */
    void remove_observer(lineage::window_observer* observer) const
    {
      lineage::remove_all(m_observers, observer);
    }

    /* -- Implementation -- */

  private:

    static lineage::window* s_instance;
    static void error_callback(int error, const char* description);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

    GLFWwindow* m_handle;
    mutable std::vector<window_observer*> m_observers;

  };

}
