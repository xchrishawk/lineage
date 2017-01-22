/**
 * @file	prototype_state_manager.cpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/12
 */

/* -- Includes -- */

#include "prototype_state_manager.hpp"
#include "state_manager.hpp"
#include "util.hpp"

/* -- Namespaces -- */

using namespace lineage;

/* -- Procedures -- */

prototype_state_manager::prototype_state_manager(const lineage::input_manager& input_manager)
  : m_input_manager(input_manager),
    m_background_color(0.0f, 0.0f, 0.0f, 1.0f)
{
}

void prototype_state_manager::run(const state_args& args)
{
  update_background_color(args);
}

double prototype_state_manager::target_delta_t() const
{
  return (1.0 / 60.0); // 60 Hz
}

void prototype_state_manager::update_background_color(const state_args& args)
{
  static const float RATE_PER_SECOND = 0.5;
  const float delta = RATE_PER_SECOND * static_cast<float>(args.delta_t);

  // red
  if (m_input_manager.input_state(input_type::generic_color_red_increase) == input_state::active)
    m_background_color.r += delta;
  if (m_input_manager.input_state(input_type::generic_color_red_decrease) == input_state::active)
    m_background_color.r -= delta;
  clamp(m_background_color.r, 0.0f, 1.0f);

  // green
  if (m_input_manager.input_state(input_type::generic_color_green_increase) == input_state::active)
    m_background_color.g += delta;
  if (m_input_manager.input_state(input_type::generic_color_green_decrease) == input_state::active)
    m_background_color.g -= delta;
  clamp(m_background_color.g, 0.0f, 1.0f);

  // blue
  if (m_input_manager.input_state(input_type::generic_color_blue_increase) == input_state::active)
    m_background_color.b += delta;
  if (m_input_manager.input_state(input_type::generic_color_blue_decrease) == input_state::active)
    m_background_color.b -= delta;
  clamp(m_background_color.b, 0.0f, 1.0f);
}
