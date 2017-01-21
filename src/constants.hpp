/**
 * @file	constants.hpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/20
 */

#pragma once

/* -- Includes -- */

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

/* -- Constants -- */

namespace lineage
{

  /* -- Zero Vectors -- */

  /** 2-dimensional zero vector. */
  extern const glm::vec2 VEC2_ZERO;

  /** 3-dimensional zero vector. */
  extern const glm::vec3 VEC3_ZERO;

  /** 4-dimensional zero vector. */
  extern const glm::vec4 VEC4_ZERO;

  /* -- Unit Vectors -- */

  /** 2-dimensional unit vector in the X direction. */
  extern const glm::vec2 VEC2_UNIT_X;

  /** 2-dimensional unit vector in the Y direction. */
  extern const glm::vec2 VEC2_UNIT_Y;

  /** 3-dimensional unit vector in the X direction. */
  extern const glm::vec3 VEC3_UNIT_X;

  /** 3-dimensional unit vector in the Y direction. */
  extern const glm::vec3 VEC3_UNIT_Y;

  /** 3-dimensional unit vector in the Z direction. */
  extern const glm::vec3 VEC3_UNIT_Z;

  /** 4-dimensional unit vector in the X direction. */
  extern const glm::vec4 VEC4_UNIT_X;

  /** 4-dimensional unit vector in the Y direction. */
  extern const glm::vec4 VEC4_UNIT_Y;

  /** 4-dimensional unit vector in the Z direction. */
  extern const glm::vec4 VEC4_UNIT_Z;

  /** 4-dimensional unit vector in the W direction. */
  extern const glm::vec4 VEC4_UNIT_W;

  /* -- Defaults -- */

  /** Zero position. */
  extern const glm::vec3 POSITION_NONE;

  /** Zero rotation. */
  extern const glm::quat ROTATION_NONE;

  /** Unity scaling. */
  extern const glm::vec3 SCALE_NONE;

  /* -- Colors -- */

  /** White color. */
  extern const glm::vec4 COLOR_WHITE;

  /** Black color. */
  extern const glm::vec4 COLOR_BLACK;

  /** Red color. */
  extern const glm::vec4 COLOR_RED;

  /** Green color. */
  extern const glm::vec4 COLOR_GREEN;

  /** Blue color. */
  extern const glm::vec4 COLOR_BLUE;

  /** Cyan color. */
  extern const glm::vec4 COLOR_CYAN;

  /** Magenta color. */
  extern const glm::vec4 COLOR_MAGENTA;

  /** Yellow color. */
  extern const glm::vec4 COLOR_YELLOW;

}
