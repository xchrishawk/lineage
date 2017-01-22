/**
 * @file	input_manager.hpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/12
 */

#pragma once

/* -- Includes -- */

#include <memory>

#include "window.hpp"

/* -- Types -- */

namespace lineage
{

  /**
   * Enumeration of input types supported by the application.
   */
  enum class input_type
  {
    invalid,
    application_exit,
    mode_camera,
    mode_background,
    mode_object,
    generic_reset,
    generic_cycle,
    generic_translate_right,
    generic_translate_left,
    generic_translate_up,
    generic_translate_down,
    generic_translate_forward,
    generic_translate_backward,
    generic_rotate_pitch_up,
    generic_rotate_pitch_down,
    generic_rotate_yaw_right,
    generic_rotate_yaw_left,
    generic_rotate_roll_right,
    generic_rotate_roll_left,
    generic_color_red_increase,
    generic_color_red_decrease,
    generic_color_green_increase,
    generic_color_green_decrease,
    generic_color_blue_increase,
    generic_color_blue_decrease,
    camera_fov_increase,
    camera_fov_decrease,
  };

  /**
   * Enumeration of the states that an input may be in.
   */
  enum class input_state
  {
    invalid,
    inactive,
    active,
  };

  /**
   * Abstract interface for types observing a `lineage::input_manager`.
   */
  class input_observer
  {
  public:

    /**
     * An input event occurred.
     *
     * @param type
     * The type of input whose state changed.
     *
     * @param state
     * The current state of the input.
     */
    virtual void input_event(lineage::input_type type, lineage::input_state state) = 0;

  };

  /**
   * Class responsible for collecting and distributing inputs to the application.
   */
  class input_manager : public lineage::window_observer
  {

    /* -- Lifecycle -- */

  public:

    /**
     * Constructs a new `lineage::input_manager` instance.
     *
     * @param window
     * The application's main window.
     */
    input_manager(const lineage::window& window);

    /**
     * Destructor.
     */
    ~input_manager();

  private:

    input_manager(const lineage::input_manager&) = delete;
    input_manager(lineage::input_manager&&) = delete;
    lineage::input_manager& operator =(const lineage::input_manager&) = delete;
    lineage::input_manager& operator =(lineage::input_manager&&) = delete;

    /* -- Public Methods -- */

  public:

    /**
     * Gets the state of the specified input type.
     */
    lineage::input_state input_state(lineage::input_type type) const;

    /**
     * Sets the state of the specified input type, notifying observers if the state changes.
     */
    void set_input_state(lineage::input_type type, lineage::input_state state);

    /**
     * Adds an observer to the input manager.
     */
    void add_observer(lineage::input_observer& observer) const;

    /**
     * Removes an observer from the input manager.
     */
    void remove_observer(lineage::input_observer& observer) const;

    /* -- `lineage::window_observer` Implementation -- */

  public:

    virtual void window_key_event(int key, int action, int mods) override;

    /* -- Implementation -- */

  private:

    struct implementation;
    const std::unique_ptr<implementation> impl;

  };

}
