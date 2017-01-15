/**
 * @file	default_state_manager.cpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/15
 */

/* -- Includes -- */

#include "default_state_manager.hpp"
#include "input_manager.hpp"
#include "state_manager.hpp"

/* -- Namespaces -- */

using namespace lineage;

/* -- Procedures -- */

default_state_manager::default_state_manager(const input_manager& input_manager)
  : m_input_manager(input_manager)
{
}

void default_state_manager::run(const state_args& args)
{
}

double default_state_manager::target_delta_t() const
{
  return (1.0 / 60.0); // 60 HZ
}
