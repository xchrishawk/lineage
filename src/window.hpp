/**
 * @file	window.hpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/11
 */

#pragma once

/* -- Includes -- */

#include <algorithm>
#include <string>
#include <vector>

#include "api.hpp"

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

    window(lineage::window&& other) noexcept;
    ~window();

  private:

    window(const lineage::window&) = delete;
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
     * Adds an observer to this window.
     */
    void add_observer(lineage::window_observer* observer)
    {
      m_observers.push_back(observer);
    }

    /**
     * Removes an observer from this window.
     */
    void remove_observer(lineage::window_observer* observer)
    {
      m_observers.erase(std::remove(m_observers.begin(),
                                    m_observers.end(),
                                    observer),
                        m_observers.end());
    }

    /* -- Implementation -- */

  private:

    static lineage::window* s_instance;
    static void error_callback(int error, const char* description);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

    GLFWwindow* m_handle;
    std::vector<window_observer*> m_observers;

  };

}
