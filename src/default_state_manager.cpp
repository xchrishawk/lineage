/**
 * @file	default_state_manager.cpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/15
 */

/* -- Includes -- */

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/transform.hpp>

#include "constants.hpp"
#include "default_state_manager.hpp"
#include "input_manager.hpp"
#include "mesh.hpp"
#include "scene_graph.hpp"
#include "state_manager.hpp"
#include "vertex.hpp"

/* -- Namespaces -- */

using namespace lineage;

/* -- Constants -- */

namespace
{
  const glm::vec3 DEFAULT_CAMERA_POSITION { 0.0, 0.0, 5.0 };
  const float RATE_CAMERA_POSITION { 2.0f };

  const glm::quat DEFAULT_CAMERA_ROTATION { };
  const float RATE_CAMERA_ROTATION { deg_to_rad(90.0f) };

  const float DEFAULT_CAMERA_CLIP_NEAR { 0.1f };
  const float DEFAULT_CAMERA_CLIP_FAR { 100.0f };

  const float DEFAULT_CAMERA_FOV { deg_to_rad(45.0f) };
  const float RATE_CAMERA_FOV { deg_to_rad(15.0f) };
  const float MIN_CAMERA_FOV { deg_to_rad(10.0f) };
  const float MAX_CAMERA_FOV { deg_to_rad(170.0f) };
}

/* -- Types -- */

/**
 * Implementation for the `lineage::default_state_manager` class.
 */
struct default_state_manager::implementation
{

  /* -- Constructor -- */

  implementation(const lineage::input_manager& input_manager)
    : input_manager(input_manager),
      scene_graph(create_scene_graph()),
      camera_position(DEFAULT_CAMERA_POSITION),
      camera_rotation(DEFAULT_CAMERA_ROTATION),
      camera_fov(DEFAULT_CAMERA_FOV),
      camera_clip_near(DEFAULT_CAMERA_CLIP_NEAR),
      camera_clip_far(DEFAULT_CAMERA_CLIP_FAR)
  { }

  /* -- Fields -- */

  const lineage::input_manager& input_manager;
  const lineage::scene_graph scene_graph;
  glm::vec3 camera_position;
  glm::quat camera_rotation;
  float camera_fov;
  float camera_clip_near;
  float camera_clip_far;

  /* -- Methods -- */

  /** Updates the camera position. */
  void update_camera_position(const state_args& args)
  {
    if (input_active(input_type::camera_reset))
    {
      camera_position = DEFAULT_CAMERA_POSITION;
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

    camera_position += glm::mat3_cast(camera_rotation) * translation;
  }

  /** Updates the camera rotation. */
  void update_camera_rotation(const state_args& args)
  {
    if (input_active(input_type::camera_reset))
    {
      camera_rotation = DEFAULT_CAMERA_ROTATION;
      return;
    }

    const float delta = RATE_CAMERA_ROTATION * args.delta_t;

    // pitch
    if (input_active(input_type::camera_rotation_pitch_up))
      camera_rotation = glm::rotate(camera_rotation, delta, VEC3_UNIT_X);
    if (input_active(input_type::camera_rotation_pitch_down))
      camera_rotation = glm::rotate(camera_rotation, -delta, VEC3_UNIT_X);

    // roll
    if (input_active(input_type::camera_rotation_roll_right))
      camera_rotation = glm::rotate(camera_rotation, -delta, VEC3_UNIT_Z);
    if (input_active(input_type::camera_rotation_roll_left))
      camera_rotation = glm::rotate(camera_rotation, delta, VEC3_UNIT_Z);

    // yaw
    if (input_active(input_type::camera_rotation_yaw_right))
      camera_rotation = glm::rotate(camera_rotation, -delta, VEC3_UNIT_Y);
    if (input_active(input_type::camera_rotation_yaw_left))
      camera_rotation = glm::rotate(camera_rotation, delta, VEC3_UNIT_Y);
  }

  /** Updates the camera field of view. */
  void update_camera_fov(const state_args& args)
  {
    if (input_active(input_type::camera_reset))
    {
      camera_fov = DEFAULT_CAMERA_FOV;
      return;
    }

    const float delta = RATE_CAMERA_FOV * args.delta_t;
    if (input_active(input_type::camera_fov_increase))
      camera_fov += delta;
    if (input_active(input_type::camera_fov_decrease))
      camera_fov -= delta;
    clamp(camera_fov, MIN_CAMERA_FOV, MAX_CAMERA_FOV);
  }

  /** Returns `true` if the specified input is active. */
  bool input_active(input_type type)
  {
    return (input_manager.input_state(type) == input_state::active);
  }

  /** Builds the scene graph to display. */
  static lineage::scene_graph create_scene_graph()
  {
    static const GLenum SQUARE_DRAW_MODE = GL_TRIANGLE_FAN;
    static const std::vector<vertex> SQUARE_VERTICES =
    {
      { { -0.5f, -0.5f, 0.0f }, VEC3_ZERO, COLOR_RED, VEC2_ZERO },
      { { 0.5f, -0.5f, 0.0f }, VEC3_ZERO, COLOR_GREEN, VEC2_ZERO },
      { { 0.5f, 0.5f, 0.0f }, VEC3_ZERO, COLOR_BLUE, VEC2_ZERO },
      { { -0.5f, 0.5f, 0.0f }, VEC3_ZERO, COLOR_BLACK, VEC2_ZERO }
    };
    static const std::vector<GLuint> SQUARE_INDICES = { 0, 1, 2, 3 };

    scene_node front_face;
    front_face.meshes().push_back(0);
    front_face.set_position(glm::vec3(0.0f, 0.0f, 0.5f));
    front_face.set_rotation(ROTATION_NONE);

    scene_node back_face;
    back_face.meshes().push_back(0);
    back_face.set_rotation(glm::rotate(ROTATION_NONE, deg_to_rad(180.0f), VEC3_UNIT_Y));
    back_face.set_position(glm::vec3(0.0f, 0.0f, -0.5f));

    scene_node left_face;
    left_face.meshes().push_back(0);
    left_face.set_rotation(glm::rotate(ROTATION_NONE, deg_to_rad(-90.0f), VEC3_UNIT_Y));
    left_face.set_position(glm::vec3(-0.5f, 0.0f, 0.0f));

    scene_node right_face;
    right_face.meshes().push_back(0);
    right_face.set_rotation(glm::rotate(ROTATION_NONE, deg_to_rad(90.0f), VEC3_UNIT_Y));
    right_face.set_position(glm::vec3(0.5f, 0.0f, 0.0f));

    scene_node top_face;
    top_face.meshes().push_back(0);
    top_face.set_rotation(glm::rotate(ROTATION_NONE, deg_to_rad(-90.0f), VEC3_UNIT_X));
    top_face.set_position(glm::vec3(0.0f, 0.5f, 0.0f));

    scene_node bottom_face;
    bottom_face.meshes().push_back(0);
    bottom_face.set_rotation(glm::rotate(ROTATION_NONE, deg_to_rad(90.0f), VEC3_UNIT_X));
    bottom_face.set_position(glm::vec3(0.0f, -0.5f, 0.0f));

    scene_node cube_node;
    cube_node.children().push_back(front_face);
    cube_node.children().push_back(back_face);
    cube_node.children().push_back(left_face);
    cube_node.children().push_back(right_face);
    cube_node.children().push_back(top_face);
    cube_node.children().push_back(bottom_face);

    lineage::scene_graph graph;
   graph.meshes().emplace_back(std::make_unique<mesh>(SQUARE_DRAW_MODE, SQUARE_VERTICES, SQUARE_INDICES));
   graph.nodes().push_back(cube_node);

    return graph;
  }

};

/* -- Procedures -- */

default_state_manager::default_state_manager(const input_manager& input_manager)
  : impl(std::make_unique<implementation>(input_manager))
{
}

default_state_manager::~default_state_manager() = default;

const lineage::scene_graph& default_state_manager::scene_graph() const
{
  return impl->scene_graph;
}

glm::vec3 default_state_manager::camera_position() const
{
  return impl->camera_position;
}

glm::quat default_state_manager::camera_rotation() const
{
  return impl->camera_rotation;
}

float default_state_manager::camera_fov() const
{
  return impl->camera_fov;
}

float default_state_manager::camera_clip_near() const
{
  return impl->camera_clip_near;
}

float default_state_manager::camera_clip_far() const
{
  return impl->camera_clip_far;
}

void default_state_manager::run(const state_args& args)
{
  impl->update_camera_position(args);
  impl->update_camera_rotation(args);
  impl->update_camera_fov(args);
}

double default_state_manager::target_delta_t() const
{
  return (1.0 / 60.0); // 60 HZ
}
