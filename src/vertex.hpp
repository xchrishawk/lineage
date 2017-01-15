/**
 * @file	vertex.hpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/14
 */

#pragma once

/* -- Includes -- */

#include <glm/glm.hpp>

#include "debug.hpp"
#include "vertex_array.hpp"

/* -- Types -- */

namespace lineage
{

  /**
   * Struct representing a vertex to be rendered.
   */
  struct vertex
  {

    /* -- Types -- */

  private:

    using position_type = glm::vec3;
    using normal_type = glm::vec3;
    using color_type = glm::vec4;

    /* -- Constants -- */

  public:

    /** Number of position values per vertex. */
    static constexpr size_t position_count = sizeof(position_type) / sizeof(position_type::value_type);

    /** Offset of the `position` field in the `lineage::vertex` struct. */
    static constexpr size_t position_offset = 0;

    /** Attribute spec for vertex positions. */
    static const lineage::attribute_spec position_spec;

    /** Number of normal values per vertex. */
    static constexpr size_t normal_count = sizeof(normal_type) / sizeof(normal_type::value_type);

    /** Offset of the `normal` field in the `lineage::vertex` struct. */
    static constexpr size_t normal_offset = position_offset + sizeof(position_type);

    /** Attribute spec for vertex normals. */
    static const lineage::attribute_spec normal_spec;

    /** Number of color values per vertex. */
    static constexpr size_t color_count = sizeof(color_type) / sizeof(color_type::value_type);

    /** Offset of the `color` field in the `lineage::vertex` struct. */
    static constexpr size_t color_offset = normal_offset + sizeof(normal_type);

    /** Attribute spec for vertex colors. */
    static const lineage::attribute_spec color_spec;

    /* -- Fields -- */

  public:

    /** The position of the vertex. */
    position_type position;

    /** The normal vector of the vertex. */
    normal_type normal;

    /** The color of the vertex. */
    color_type color;

  };

  // We can't use the offsetof macro inside the class, because the class hasn't been fully defined
  // yet, so we manually set the offset constants to the expected values. We use static asserts here
  // to verify that the hand-coded values are actually correct.
  static_assert(offsetof(vertex, position) == vertex::position_offset, "Incorrect offset!");
  static_assert(offsetof(vertex, normal) == vertex::normal_offset, "Incorrect offset!");
  static_assert(offsetof(vertex, color) == vertex::color_offset, "Incorrect offset!");

}
