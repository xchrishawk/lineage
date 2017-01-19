/**
 * @file	util.hpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/12
 */

#pragma once

/* -- Includes -- */

#include <algorithm>
#include <cmath>
#include <functional>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

/* -- Constants -- */

namespace lineage
{

  /** `glm::vec3` representing a unit vector in the X direction. */
  extern const glm::vec3 VEC3_UNIT_X;

  /** `glm::vec3` representing a unit vector in the Y direction. */
  extern const glm::vec3 VEC3_UNIT_Y;

  /** `glm::vec3` representing a unit vector in the Z direction. */
  extern const glm::vec3 VEC3_UNIT_Z;

  /** `glm::vec3` representing a position at the origin. */
  extern const glm::vec3 POSITION_ZERO;

  /** `glm::quat` representing no rotation. */
  extern const glm::quat ROTATION_ZERO;

  /** `glm::vec3` representing no scaling. */
  extern const glm::vec3 SCALE_NONE;

}

/* -- Types -- */

namespace lineage
{

  /**
   * Allows using enum classes as a key in `std::unordered_map`.
   */
  struct enum_class_hash
  {

    template <typename T>
    std::size_t operator()(T t) const
    {
        return static_cast<std::size_t>(t);
    }

  };

  /**
   * Class which performs a deferred action when it is destroyed.
   */
  class defer
  {
  public:

    /**
     * Constructs a new `lineage::defer` instance.
     *
     * @param action
     * The action to execute when this object is destroyed.
     */
    defer(std::function<void(void)> action)
      : m_action(action)
    { }

    ~defer()
    {
      m_action();
    }

  private:
    std::function<void(void)> m_action;
  };

}

/* -- Procedures -- */

namespace lineage
{

  /**
   * Returns the size of the specified array.
   */
  template <typename T, size_t size>
  constexpr size_t array_size(const T(&)[size])
  {
    return size;
  }

  /**
   * Clamps `value` to between `min` and `max`.
   */
  template <typename T>
  constexpr void clamp(T& value, T min, T max)
  {
    if (value < min)
      value = min;
    else if (value > max)
      value = max;
  }

  /**
   * Converts an angle in degrees to radians.
   */
  template <typename T>
  constexpr T deg_to_rad(T deg)
  {
    return (deg * (M_PI / 180));
  }

  /**
   * Converts an angle in radians to degrees.
   */
  template <typename T>
  constexpr T rad_to_deg(T rad)
  {
    return (rad * (180 / M_PI));
  }

  /**
   * Removes all instances of `value` from `container`.
   */
  template <typename TContainer, typename TValue>
  void remove_all(TContainer& container, const TValue& value)
  {
    container.erase(std::remove(container.begin(),
                                container.end(),
                                value),
                    container.end());
  }

}
