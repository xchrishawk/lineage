/**
 * @file	default_state_manager.cpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/15
 */

/* -- Includes -- */

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

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
  const float RATE_CAMERA_POSITION { 2.0f };

  const glm::quat DEFAULT_CAMERA_ROTATION { };
  const float RATE_CAMERA_ROTATION { deg_to_rad(90.0f) };

  const float DEFAULT_CAMERA_CLIP_NEAR { 0.0f };
  const float DEFAULT_CAMERA_CLIP_FAR { 100.0f };

  const float DEFAULT_CAMERA_FOV { deg_to_rad(45.0f) };
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
  update_camera_position(args);
  update_camera_rotation(args);
  update_camera_fov(args);
}

double default_state_manager::target_delta_t() const
{
  return (1.0 / 60.0); // 60 HZ
}

void default_state_manager::update_camera_position(const state_args& args)
{
  if (input_active(input_type::camera_reset))
  {
    m_camera_position = DEFAULT_CAMERA_POSITION;
    return;
  }

  const float delta = RATE_CAMERA_POSITION * args.delta_t;

  glm::vec3 translation(0.0f);

  // right/left
  if (input_active(input_type::camera_translation_right))
    translation.x += delta;
  if (input_active(input_type::camera_translation_left))
    translation.x -= delta;

  // up/down
  if (input_active(input_type::camera_translation_up))
    translation.y += delta;
  if (input_active(input_type::camera_translation_down))
    translation.y -= delta;

  // forward/backward
  if (input_active(input_type::camera_translation_forward))
    translation.z -= delta;
  if (input_active(input_type::camera_translation_backward))
    translation.z += delta;

  m_camera_position += glm::mat3_cast(m_camera_rotation) * translation;
}

void default_state_manager::update_camera_rotation(const state_args& args)
{
  if (input_active(input_type::camera_reset))
  {
    m_camera_rotation = DEFAULT_CAMERA_ROTATION;
    return;
  }

  const float delta = RATE_CAMERA_ROTATION * args.delta_t;

  // pitch
  if (input_active(input_type::camera_rotation_pitch_up))
    m_camera_rotation = glm::rotate(m_camera_rotation, delta, VEC3_UNIT_X);
  if (input_active(input_type::camera_rotation_pitch_down))
    m_camera_rotation = glm::rotate(m_camera_rotation, -delta, VEC3_UNIT_X);

  // roll
  if (input_active(input_type::camera_rotation_roll_right))
    m_camera_rotation = glm::rotate(m_camera_rotation, -delta, VEC3_UNIT_Z);
  if (input_active(input_type::camera_rotation_roll_left))
    m_camera_rotation = glm::rotate(m_camera_rotation, delta, VEC3_UNIT_Z);

  // yaw
  if (input_active(input_type::camera_rotation_yaw_right))
    m_camera_rotation = glm::rotate(m_camera_rotation, -delta, VEC3_UNIT_Y);
  if (input_active(input_type::camera_rotation_yaw_left))
    m_camera_rotation = glm::rotate(m_camera_rotation, delta, VEC3_UNIT_Y);
}

void default_state_manager::update_camera_fov(const state_args& args)
{
  if (input_active(input_type::camera_reset))
  {
    m_camera_fov = DEFAULT_CAMERA_FOV;
    return;
  }

  const float delta = RATE_CAMERA_FOV * args.delta_t;
  if (input_active(input_type::camera_fov_increase))
    m_camera_fov += delta;
  if (input_active(input_type::camera_fov_decrease))
    m_camera_fov -= delta;
  clamp(m_camera_fov, MIN_CAMERA_FOV, MAX_CAMERA_FOV);
}

bool default_state_manager::input_active(input_type type)
{
  return (m_input_manager.input_state(type) == input_state::active);
}
