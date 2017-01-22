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

  /* -- Defaults -- */

  const glm::vec3 DEFAULT_CAMERA_POSITION { 0.0, 0.0, 5.0 };
  const glm::quat DEFAULT_CAMERA_ROTATION { };
  const float DEFAULT_CAMERA_CLIP_NEAR { 0.1f };
  const float DEFAULT_CAMERA_CLIP_FAR { 100.0f };
  const float DEFAULT_CAMERA_FOV { deg_to_rad(45.0f) };
  const glm::vec4 DEFAULT_BACKGROUND_COLOR { COLOR_BLACK };

  /* -- Minimums/Maximums -- */

  const float MIN_CAMERA_FOV { deg_to_rad(10.0f) };
  const float MAX_CAMERA_FOV { deg_to_rad(170.0f) };
  const float MIN_COLOR_COMPONENT { 0.0f };
  const float MAX_COLOR_COMPONENT { 1.0f };

  /* -- Rates of Change -- */

  const float RATE_CAMERA_POSITION { 2.0f };
  const float RATE_CAMERA_ROTATION { deg_to_rad(90.0f) };
  const float RATE_CAMERA_FOV { deg_to_rad(45.0f) };
  const float RATE_COLOR_COMPONENT { 0.5f };
  const float RATE_OBJECT_POSITION { 1.0f };
  const float RATE_OBJECT_ROTATION { deg_to_rad(45.0f) };

}

/* -- Types -- */

namespace
{

  /** Enumeration of user input modes. */
  enum class input_mode
  {
    camera,
    background,
    object,
  };

}

/**
 * Implementation for the `lineage::default_state_manager` class.
 */
struct default_state_manager::implementation : public input_observer
{

  /* -- Constructor -- */

  implementation(const lineage::input_manager& input_manager)
    : input_manager(input_manager),
      scene_graph(create_scene_graph()),
      mode(input_mode::camera),
      camera_position(DEFAULT_CAMERA_POSITION),
      camera_rotation(DEFAULT_CAMERA_ROTATION),
      camera_fov(DEFAULT_CAMERA_FOV),
      camera_clip_near(DEFAULT_CAMERA_CLIP_NEAR),
      camera_clip_far(DEFAULT_CAMERA_CLIP_FAR),
      background_color(DEFAULT_BACKGROUND_COLOR)
  {
    input_manager.add_observer(*this);
  }

  ~implementation()
  {
    input_manager.remove_observer(*this);
  }

  /* -- Fields -- */

  const lineage::input_manager& input_manager;
  lineage::scene_graph scene_graph;

  input_mode mode;

  glm::vec3 camera_position;
  glm::quat camera_rotation;
  float camera_fov;
  float camera_clip_near;
  float camera_clip_far;

  glm::vec4 background_color;

  size_t selected_node_index;

  /* -- `lineage::input_observer` Implementation -- */

  /** Handles an input event. */
  virtual void input_event(input_type type, input_state state)
  {
    if (state != input_state::active)
      return;

    switch (type)
    {
    case input_type::mode_camera:
      mode = input_mode::camera;
      lineage_log_status("Input mode set to input_mode::camera.");
      break;

    case input_type::mode_background:
      mode = input_mode::background;
      lineage_log_status("Input mode set to input_mode::background.");
      break;

    case input_type::mode_object:
      selected_node_index = 0;
      mode = input_mode::object;
      lineage_log_status("Input mode set to input_mode::object.");
      break;

    case input_type::generic_cycle:
      switch (mode)
      {
      case input_mode::object:
        selected_node_index = (selected_node_index + 1 % scene_graph.nodes().size());
        break;
      default:
        break;
      }
      break;

    default:
      break;
    }
  }

  /* -- Methods -- */

  /** Updates the camera position. */
  void update_camera_position(const state_args& args)
  {
    if (input_active(input_type::generic_reset))
    {
      camera_position = DEFAULT_CAMERA_POSITION;
      return;
    }
    camera_position = translate(camera_position, RATE_CAMERA_POSITION * args.delta_t, camera_rotation);
  }

  /** Updates the camera rotation. */
  void update_camera_rotation(const state_args& args)
  {
    if (input_active(input_type::generic_reset))
    {
      camera_rotation = DEFAULT_CAMERA_ROTATION;
      return;
    }
    camera_rotation = rotate(camera_rotation, RATE_CAMERA_ROTATION * args.delta_t);
  }

  /** Updates the camera field of view. */
  void update_camera_fov(const state_args& args)
  {
    if (input_active(input_type::generic_reset))
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

  /** Updates the background color. */
  void update_background_color(const state_args& args)
  {
    if (input_active(input_type::generic_reset))
    {
      background_color = DEFAULT_BACKGROUND_COLOR;
      return;
    }

    const float delta = RATE_COLOR_COMPONENT * args.delta_t;

    // red
    if (input_active(input_type::generic_color_red_increase))
      background_color.r += delta;
    if (input_active(input_type::generic_color_red_decrease))
      background_color.r -= delta;

    // green
    if (input_active(input_type::generic_color_green_increase))
      background_color.g += delta;
    if (input_active(input_type::generic_color_green_decrease))
      background_color.g -= delta;

    // blue
    if (input_active(input_type::generic_color_blue_increase))
      background_color.b += delta;
    if (input_active(input_type::generic_color_blue_decrease))
      background_color.b -= delta;

    clamp(background_color.r, MIN_COLOR_COMPONENT, MAX_COLOR_COMPONENT);
    clamp(background_color.g, MIN_COLOR_COMPONENT, MAX_COLOR_COMPONENT);
    clamp(background_color.b, MIN_COLOR_COMPONENT, MAX_COLOR_COMPONENT);
  }

  /** Updates the position of the selected object. */
  void update_object_position(const state_args& args)
  {
    auto& node = scene_graph.nodes()[selected_node_index];
    glm::vec3 position = translate(node.position(), RATE_OBJECT_POSITION * args.delta_t, node.rotation());
    node.set_position(position);
  }

  void update_object_rotation(const state_args& args)
  {
    auto& node = scene_graph.nodes()[selected_node_index];
    glm::quat rotation = rotate(node.rotation(), RATE_OBJECT_ROTATION * args.delta_t);
    node.set_rotation(rotation);
  }

  /** Translates a position. */
  glm::vec3 translate(const glm::vec3& position, float delta, const glm::quat& rotation)
  {
    glm::vec3 translation(0.0f);

    // right/left
    if (input_active(input_type::generic_translate_right))
      translation.x += delta;
    if (input_active(input_type::generic_translate_left))
      translation.x -= delta;

    // up/down
    if (input_active(input_type::generic_translate_up))
      translation.y += delta;
    if (input_active(input_type::generic_translate_down))
      translation.y -= delta;

    // forward/backward
    if (input_active(input_type::generic_translate_forward))
      translation.z -= delta;
    if (input_active(input_type::generic_translate_backward))
      translation.z += delta;

    return position + (glm::mat3_cast(rotation) * translation);
  }

  /** Rotates a quaternion. */
  glm::quat rotate(const glm::quat& rotation, float delta)
  {
    glm::quat ret = rotation;

    // pitch
    if (input_active(input_type::generic_rotate_pitch_up))
      ret = glm::rotate(ret, delta, VEC3_UNIT_X);
    if (input_active(input_type::generic_rotate_pitch_down))
      ret = glm::rotate(ret, -delta, VEC3_UNIT_X);

    // roll
    if (input_active(input_type::generic_rotate_roll_right))
      ret = glm::rotate(ret, -delta, VEC3_UNIT_Z);
    if (input_active(input_type::generic_rotate_roll_left))
      ret = glm::rotate(ret, delta, VEC3_UNIT_Z);

    // yaw
    if (input_active(input_type::generic_rotate_yaw_right))
      ret = glm::rotate(ret, -delta, VEC3_UNIT_Y);
    if (input_active(input_type::generic_rotate_yaw_left))
      ret = glm::rotate(ret, delta, VEC3_UNIT_Y);

    return ret;
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

glm::vec4 default_state_manager::background_color() const
{
  return impl->background_color;
}

void default_state_manager::run(const state_args& args)
{
  if (impl->mode == input_mode::camera)
  {
    impl->update_camera_position(args);
    impl->update_camera_rotation(args);
    impl->update_camera_fov(args);
  }
  else if (impl->mode == input_mode::background)
  {
    impl->update_background_color(args);
  }
  else if (impl->mode == input_mode::object)
  {
    if (impl->selected_node_index >= impl->scene_graph.nodes().size())
      return;
    impl->update_object_position(args);
    impl->update_object_rotation(args);
  }
}

double default_state_manager::target_delta_t() const
{
  return (1.0 / 60.0); // 60 HZ
}
