/**
 * @file	default_state_manager.cpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/15
 */

/* -- Includes -- */

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

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

  const float DEFAULT_CAMERA_CLIP_NEAR { 0.0f };
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
    // first mesh
    static const GLenum MESH_1_DRAW_MODE = GL_TRIANGLE_FAN;
    static const std::vector<vertex> MESH_1_VERTICES =
    {
      { { 0.0f, 0.0f, 0.0f }, VEC3_ZERO, COLOR_RED, VEC2_ZERO },
      { { 0.5f, 0.0f, 0.0f }, VEC3_ZERO, COLOR_BLUE, VEC2_ZERO },
      { { 0.5f, 0.5f, 0.0f }, VEC3_ZERO, COLOR_GREEN, VEC2_ZERO },
      { { 0.0f, 0.5f, 0.0f }, VEC3_ZERO, COLOR_BLACK, VEC2_ZERO },
    };
    static const std::vector<GLuint> MESH_1_INDICES = { 0, 1, 2, 3 };

    // second mesh
    static const GLenum MESH_2_DRAW_MODE = GL_TRIANGLES;
    static const std::vector<vertex> MESH_2_VERTICES =
    {
      { { 0.0f, 0.0f, 0.0f }, VEC3_ZERO, COLOR_CYAN, VEC2_ZERO },
      { { -0.5f, 0.0f, 0.0f }, VEC3_ZERO, COLOR_MAGENTA, VEC2_ZERO },
      { { 0.0f, -0.5f, 0.0f }, VEC3_ZERO, COLOR_YELLOW, VEC2_ZERO },
    };
    static const std::vector<GLuint> MESH_2_INDICES = { 0, 1, 2 };

    lineage::scene_graph graph;
    graph.meshes().emplace_back(std::make_unique<mesh>(MESH_1_DRAW_MODE, MESH_1_VERTICES, MESH_1_INDICES));
    graph.meshes().emplace_back(std::make_unique<mesh>(MESH_2_DRAW_MODE, MESH_2_VERTICES, MESH_2_INDICES));

    scene_node node1;
    node1.meshes().push_back(0);
    graph.nodes().push_back(std::move(node1));

    scene_node node2;
    node2.meshes().push_back(1);
    graph.nodes().push_back(std::move(node2));

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
