/**
 * @file	input_manager.hpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/12
 */

#pragma once

/* -- Includes -- */

#include <unordered_map>
#include <vector>

#include "util.hpp"
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
    virtual void input_event(lineage::input_type type, lineage::input_state active) = 0;

  };

  /**
   * Class responsible for collecting and distributing inputs to the application.
   */
  class input_manager : public lineage::window_observer
  {

    /* -- Lifecycle -- */

  public:

    input_manager();
    input_manager(lineage::input_manager&& other) noexcept;
    ~input_manager();

  private:

    input_manager(const lineage::input_manager&) = delete;
    lineage::input_manager& operator =(const lineage::input_manager&) = delete;
    lineage::input_manager& operator =(lineage::input_manager&&) = delete;

    /* -- Public Methods -- */

  public:

    /**
     * Gets the state of the specified input type.
     */
    lineage::input_state input_state(lineage::input_type type) const
    {
      const auto it = m_states.find(type);
      return (it != m_states.end() ? it->second : lineage::input_state::invalid);
    }

    /**
     * Sets the state of the specified input type, notifying observers if the state changes.
     */
    void set_input_state(lineage::input_type type, lineage::input_state state);

    /**
     * Adds an observer to the input manager.
     */
    void add_observer(lineage::input_observer* observer)
    {
      m_observers.push_back(observer);
    }

    /**
     * Removes an observer from the input manager.
     */
    void remove_observer(lineage::input_observer* observer)
    {
      lineage::remove_all(m_observers, observer);
    }

    /* -- `lineage::window_observer` Implementation -- */

  public:

    virtual void window_key_event(int key, int action, int mods) override;

    /* -- Implementation -- */

  private:

    std::unordered_map<lineage::input_type, lineage::input_state, lineage::enum_class_hash> m_states;
    std::vector<lineage::input_observer*> m_observers;

    lineage::input_type input_type_for_key_event(int key, int mods);
    lineage::input_state input_state_for_key_event(int action);

  };

}
