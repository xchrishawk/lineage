/**
 * @file	default_state_manager.cpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/15
 */

/* -- Includes -- */

#include <iostream>

#include <glm/glm.hpp>

#include "default_state_manager.hpp"
#include "input_manager.hpp"
#include "state_manager.hpp"
#include "util.hpp"

/* -- Namespaces -- */

using namespace lineage;

/* -- Constants -- */

namespace
{
  const glm::vec3 DEFAULT_CAMERA_POSITION { 0.0, 0.0, 5.0 };
  const glm::quat DEFAULT_CAMERA_ROTATION { };
  const float DEFAULT_CAMERA_FOV { deg_to_rad(45.0f) };
  const float DEFAULT_CAMERA_CLIP_NEAR { 0.5f };
  const float DEFAULT_CAMERA_CLIP_FAR { 100.0f };

  const float RATE_CAMERA_FOV { deg_to_rad(15.0f) };
  const float MIN_CAMERA_FOV { deg_to_rad(10.0f) };
  const float MAX_CAMERA_FOV { deg_to_rad(170.0f) };
}

/* -- Procedures -- */

default_state_manager::default_state_manager(const input_manager& input_manager)
  : m_input_manager(input_manager),
    m_camera_position(DEFAULT_CAMERA_POSITION),
    m_camera_rotation(DEFAULT_CAMERA_ROTATION),
    m_camera_fov(DEFAULT_CAMERA_FOV),
    m_camera_clip_near(DEFAULT_CAMERA_CLIP_NEAR),
    m_camera_clip_far(DEFAULT_CAMERA_CLIP_FAR)
{
}

void default_state_manager::run(const state_args& args)
{
  update_camera_fov(args);
}

double default_state_manager::target_delta_t() const
{
  return (1.0 / 60.0); // 60 HZ
}

void default_state_manager::update_camera_fov(const state_args& args)
{
  if (input_active(input_type::camera_reset))
  {
    m_camera_fov = DEFAULT_CAMERA_FOV;
    return;
  }

  const double delta = RATE_CAMERA_FOV * args.delta_t;
  if (input_active(input_type::camera_fov_increase))
    m_camera_fov += delta;
  if (input_active(input_type::camera_fov_decrease))
    m_camera_fov -= delta;

  clamp(m_camera_fov, MIN_CAMERA_FOV, MAX_CAMERA_FOV);
  std::cout << m_camera_fov << std::endl;
}

bool default_state_manager::input_active(input_type type)
{
  return (m_input_manager.input_state(type) == input_state::active);
}
