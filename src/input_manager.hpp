/**
 * @file	input_manager.hpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/12
 */

#pragma once

/* -- Includes -- */

#include <unordered_map>
#include <vector>

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
    camera_reset,
    camera_fov_increase,
    camera_fov_decrease,
    color_red_increase,
    color_red_decrease,
    color_green_increase,
    color_green_decrease,
    color_blue_increase,
    color_blue_decrease,
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

    const lineage::window& m_window;
    std::unordered_map<lineage::input_type, lineage::input_state, lineage::enum_class_hash> m_states;
    mutable std::vector<lineage::input_observer*> m_observers;

    lineage::input_type input_type_for_key_event(int key, int mods);
    lineage::input_state input_state_for_key_event(int action);

  };

}
