/**
 * @file	input_manager.cpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/12
 */

/* -- Includes -- */

#include <utility>

#include "api.hpp"
#include "debug.hpp"
#include "input_manager.hpp"

/* -- Namespaces -- */

using namespace lineage;

/* -- Procedures -- */

input_manager::input_manager()
  : m_states(),
    m_observers()
{
}

input_manager::input_manager(input_manager&& other) noexcept
  : m_states(),
    m_observers()
{
  std::swap(m_observers, other.m_observers);
  std::swap(m_states, other.m_states);
}

input_manager::~input_manager()
{
}

void input_manager::set_input_state(lineage::input_type type, lineage::input_state state)
{
  const auto current_state = m_states[type];
  if (state == current_state)
    return;

  m_states[type] = state;
  for (auto observer : m_observers)
    observer->input_event(type, state);
}

void input_manager::window_key_event(int key, int action, int mods)
{
  const auto type = input_type_for_key_event(key, mods);
  const auto state = input_state_for_key_event(action);
  if (type == input_type::invalid || state == input_state::invalid)
    return;

  set_input_state(type, state);
}

input_type input_manager::input_type_for_key_event(int key, int mods)
{
  switch (mods)
  {
  case 0:
    switch (key)
    {
    case GLFW_KEY_ESCAPE:	return input_type::application_exit;
    default:      		return input_type::invalid;
    }

  default:
    return input_type::invalid;
  }
}

input_state input_manager::input_state_for_key_event(int action)
{
  switch (action)
  {
  case GLFW_PRESS:		return input_state::active;
  case GLFW_RELEASE:		return input_state::inactive;
  default:			return input_state::invalid;
  }
}
