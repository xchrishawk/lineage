/**
 * @file	input_manager.cpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/12
 */

/* -- Includes -- */

#include <unordered_map>
#include <utility>
#include <vector>

#include "api.hpp"
#include "debug.hpp"
#include "input_manager.hpp"
#include "util.hpp"

/* -- Namespaces -- */

using namespace lineage;

/* -- Types -- */

struct input_manager::implementation
{

  /* -- Constructor -- */

  implementation(const lineage::window& window)
    : window(window),
      states(),
      observers()
  { }

  /* -- Fields -- */

  const lineage::window& window;
  std::unordered_map<lineage::input_type, lineage::input_state, lineage::enum_class_hash> states;
  std::vector<lineage::input_observer*> observers;

  /* -- Methods -- */

  /** Get the input type for the specified key event. */
  lineage::input_type input_type_for_key_event(int key, int mods)
  {
    switch (mods)
    {
    case 0:
      switch (key)
      {
      case GLFW_KEY_ESCAPE:		return input_type::application_exit;
      case GLFW_KEY_F1:			return input_type::mode_camera;
      case GLFW_KEY_F2:			return input_type::mode_background;
      case GLFW_KEY_F3:			return input_type::mode_object;
      case GLFW_KEY_X:			return input_type::generic_reset;
      case GLFW_KEY_TAB:		return input_type::generic_cycle;
      case GLFW_KEY_D:			return input_type::generic_translate_right;
      case GLFW_KEY_A:			return input_type::generic_translate_left;
      case GLFW_KEY_R:			return input_type::generic_translate_up;
      case GLFW_KEY_F:			return input_type::generic_translate_down;
      case GLFW_KEY_W:			return input_type::generic_translate_forward;
      case GLFW_KEY_S:			return input_type::generic_translate_backward;
      case GLFW_KEY_U:			return input_type::generic_color_red_increase;
      case GLFW_KEY_J:			return input_type::generic_color_red_decrease;
      case GLFW_KEY_I:			return input_type::generic_color_green_increase;
      case GLFW_KEY_K:			return input_type::generic_color_green_decrease;
      case GLFW_KEY_O:			return input_type::generic_color_blue_increase;
      case GLFW_KEY_L:			return input_type::generic_color_blue_decrease;
      case GLFW_KEY_LEFT_BRACKET:	return input_type::camera_fov_decrease;
      case GLFW_KEY_RIGHT_BRACKET:	return input_type::camera_fov_increase;
      default:      			return input_type::invalid;
      }

    case GLFW_MOD_SHIFT:
      switch (key)
      {
      case GLFW_KEY_W:			return input_type::generic_rotate_pitch_down;
      case GLFW_KEY_S:			return input_type::generic_rotate_pitch_up;
      case GLFW_KEY_D:			return input_type::generic_rotate_yaw_right;
      case GLFW_KEY_A:			return input_type::generic_rotate_yaw_left;
      case GLFW_KEY_E:			return input_type::generic_rotate_roll_right;
      case GLFW_KEY_Q:			return input_type::generic_rotate_roll_left;
      }

    default:
      return input_type::invalid;
    }
  }

  /** Get the input state for the specified key event. */
  lineage::input_state input_state_for_key_event(int action)
  {
    switch (action)
    {
    case GLFW_PRESS:			return lineage::input_state::active;
    case GLFW_RELEASE:			return lineage::input_state::inactive;
    default:				return lineage::input_state::invalid;
    }
  }

};

/* -- Procedures -- */

input_manager::input_manager(const window& window)
  : impl(std::make_unique<implementation>(window))
{
  impl->window.add_observer(this);
}

input_manager::~input_manager()
{
  impl->window.remove_observer(this);
}

input_state input_manager::input_state(input_type type) const
{
  const auto it = impl->states.find(type);
  return (it != impl->states.end() ? it->second : lineage::input_state::invalid);
}

void input_manager::set_input_state(lineage::input_type type, lineage::input_state state)
{
  const auto current_state = impl->states[type];
  if (state == current_state)
    return;

  impl->states[type] = state;
  for (auto observer : impl->observers)
    observer->input_event(type, state);
}

void input_manager::add_observer(input_observer& observer) const
{
  impl->observers.push_back(&observer);
}

void input_manager::remove_observer(input_observer& observer) const
{
  remove_all(impl->observers, &observer);
}

void input_manager::window_key_event(int key, int action, int mods)
{
  const auto state = impl->input_state_for_key_event(action);
  if (state == lineage::input_state::active)
  {
    // activate this input only
    set_input_state(impl->input_type_for_key_event(key, mods), state);
  }
  else if (state == lineage::input_state::inactive)
  {
    // deactivate every input which shares this "primary" key
    set_input_state(impl->input_type_for_key_event(key, 0), state);
    set_input_state(impl->input_type_for_key_event(key, GLFW_MOD_SHIFT), state);
    set_input_state(impl->input_type_for_key_event(key, GLFW_MOD_CONTROL), state);
    set_input_state(impl->input_type_for_key_event(key, GLFW_MOD_ALT), state);
    set_input_state(impl->input_type_for_key_event(key, GLFW_MOD_SUPER), state);
  }
}
