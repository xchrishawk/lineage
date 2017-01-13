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

prototype_state_manager::~prototype_state_manager()
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
  m_background_color.r = sin(args.abs_t);
  m_background_color.g = cos(args.abs_t);
}
