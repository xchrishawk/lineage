/**
 * @file	default_state_manager.cpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/15
 */

/* -- Includes -- */

#include <limits>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/transform.hpp>

#include "constants.hpp"
#include "default_state_manager.hpp"
#include "input_manager.hpp"
#include "mesh.hpp"
#include "scene_builder.hpp"
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
  const glm::vec4 DEFAULT_AMBIENT_LIGHT_COLOR { COLOR_WHITE };
  const float DEFAULT_AMBIENT_LIGHT_INTENSITY { 1.0f };

  /* -- Minimums/Maximums -- */

  const float MIN_CAMERA_FOV { deg_to_rad(10.0f) };
  const float MAX_CAMERA_FOV { deg_to_rad(170.0f) };
  const float MIN_COLOR_COMPONENT { 0.0f };
  const float MAX_COLOR_COMPONENT { 1.0f };
  const float MIN_LIGHT_INTENSITY { 0.0f };
  const float MAX_LIGHT_INTENSITY { std::numeric_limits<float>::max() };

  /* -- Rates of Change -- */

  const float RATE_CAMERA_POSITION { 2.0f };
  const float RATE_CAMERA_ROTATION { deg_to_rad(90.0f) };
  const float RATE_CAMERA_FOV { deg_to_rad(45.0f) };
  const float RATE_COLOR_COMPONENT { 0.5f };
  const float RATE_OBJECT_POSITION { 1.0f };
  const float RATE_OBJECT_ROTATION { deg_to_rad(45.0f) };
  const float RATE_LIGHT_INTENSITY { 0.5f };

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
    ambient_light,
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
      scene_graph(create_multiple_cubes_scene_graph()),
      mode(input_mode::camera),
      camera_position(DEFAULT_CAMERA_POSITION),
      camera_rotation(DEFAULT_CAMERA_ROTATION),
      camera_fov(DEFAULT_CAMERA_FOV),
      camera_clip_near(DEFAULT_CAMERA_CLIP_NEAR),
      camera_clip_far(DEFAULT_CAMERA_CLIP_FAR),
      background_color(DEFAULT_BACKGROUND_COLOR),
      ambient_light_color(DEFAULT_AMBIENT_LIGHT_COLOR),
      ambient_light_intensity(DEFAULT_AMBIENT_LIGHT_INTENSITY),
      selected_node_index(0)
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
  glm::vec4 ambient_light_color;
  float ambient_light_intensity;
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

    case input_type::mode_ambient_light:
      mode = input_mode::ambient_light;
      lineage_log_status("Input mode set to input_mode::ambient_light.");
      break;

    case input_type::generic_cycle:
      switch (mode)
      {
      case input_mode::object:
        selected_node_index = ((selected_node_index + 1) % scene_graph.nodes().size());
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
    camera_position = update_position(camera_position, RATE_CAMERA_POSITION * args.delta_t, camera_rotation);
  }

  /** Updates the camera rotation. */
  void update_camera_rotation(const state_args& args)
  {
    if (input_active(input_type::generic_reset))
    {
      camera_rotation = DEFAULT_CAMERA_ROTATION;
      return;
    }
    camera_rotation = update_rotation(camera_rotation, RATE_CAMERA_ROTATION * args.delta_t);
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
    background_color = update_color(background_color, RATE_COLOR_COMPONENT * args.delta_t);
  }

  /** Updates the position of the selected object. */
  void update_object_position(const state_args& args)
  {
    auto& node = scene_graph.nodes()[selected_node_index];
    glm::vec3 position = update_position(node.position(), RATE_OBJECT_POSITION * args.delta_t, node.rotation());
    node.set_position(position);
  }

  /** Updates the rotation of the selected object. */
  void update_object_rotation(const state_args& args)
  {
    auto& node = scene_graph.nodes()[selected_node_index];
    glm::quat rotation = update_rotation(node.rotation(), RATE_OBJECT_ROTATION * args.delta_t);
    node.set_rotation(rotation);
  }

  /** Updates the ambient lighting color. */
  void update_ambient_light_color(const state_args& args)
  {
    if (input_active(input_type::generic_reset))
    {
      ambient_light_color = DEFAULT_AMBIENT_LIGHT_COLOR;
      return;
    }
    ambient_light_color = update_color(ambient_light_color, RATE_COLOR_COMPONENT * args.delta_t);
  }

  /** Updates the ambient lighting intensity. */
  void update_ambient_light_intensity(const state_args& args)
  {
    if (input_active(input_type::generic_reset))
    {
      ambient_light_intensity = DEFAULT_AMBIENT_LIGHT_INTENSITY;
      return;
    }

    const float delta = RATE_LIGHT_INTENSITY * args.delta_t;

    if (input_active(input_type::lighting_intensity_increase))
      ambient_light_intensity += delta;
    if (input_active(input_type::lighting_intensity_decrease))
      ambient_light_intensity -= delta;

    clamp(ambient_light_intensity, MIN_LIGHT_INTENSITY, MAX_LIGHT_INTENSITY);
  }

  /** Translates a position. */
  glm::vec3 update_position(const glm::vec3& position, float delta, const glm::quat& rotation)
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
  glm::quat update_rotation(glm::quat rotation, float delta)
  {
    // pitch
    if (input_active(input_type::generic_rotate_pitch_up))
      rotation = glm::rotate(rotation, delta, VEC3_UNIT_X);
    if (input_active(input_type::generic_rotate_pitch_down))
      rotation = glm::rotate(rotation, -delta, VEC3_UNIT_X);

    // roll
    if (input_active(input_type::generic_rotate_roll_right))
      rotation = glm::rotate(rotation, -delta, VEC3_UNIT_Z);
    if (input_active(input_type::generic_rotate_roll_left))
      rotation = glm::rotate(rotation, delta, VEC3_UNIT_Z);

    // yaw
    if (input_active(input_type::generic_rotate_yaw_right))
      rotation = glm::rotate(rotation, -delta, VEC3_UNIT_Y);
    if (input_active(input_type::generic_rotate_yaw_left))
      rotation = glm::rotate(rotation, delta, VEC3_UNIT_Y);

    return rotation;
  }

  /** Updates a color. */
  glm::vec4 update_color(glm::vec4 color, float delta)
  {
    // red
    if (input_active(input_type::generic_color_red_increase))
      color.r += delta;
    if (input_active(input_type::generic_color_red_decrease))
      color.r -= delta;

    // green
    if (input_active(input_type::generic_color_green_increase))
      color.g += delta;
    if (input_active(input_type::generic_color_green_decrease))
      color.g -= delta;

    // blue
    if (input_active(input_type::generic_color_blue_increase))
      color.b += delta;
    if (input_active(input_type::generic_color_blue_decrease))
      color.b -= delta;

    clamp(color.r, MIN_COLOR_COMPONENT, MAX_COLOR_COMPONENT);
    clamp(color.g, MIN_COLOR_COMPONENT, MAX_COLOR_COMPONENT);
    clamp(color.b, MIN_COLOR_COMPONENT, MAX_COLOR_COMPONENT);

    return color;
  }

  /** Returns `true` if the specified input is active. */
  bool input_active(input_type type)
  {
    return (input_manager.input_state(type) == input_state::active);
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

glm::vec4 default_state_manager::ambient_light_color() const
{
  return impl->ambient_light_color;
}

float default_state_manager::ambient_light_intensity() const
{
  return impl->ambient_light_intensity;
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
  else if (impl->mode == input_mode::ambient_light)
  {
    impl->update_ambient_light_color(args);
    impl->update_ambient_light_intensity(args);
  }
}

double default_state_manager::target_delta_t() const
{
  return (1.0 / 60.0); // 60 HZ
}
