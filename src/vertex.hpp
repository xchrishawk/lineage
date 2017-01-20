/**
 * @file	vertex.hpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/14
 */

#pragma once

/* -- Includes -- */

#include <type_traits>
#include <glm/glm.hpp>

#include "api.hpp"
#include "debug.hpp"
#include "vertex_array.hpp"

/* -- Types -- */

namespace lineage
{
  namespace templates
  {


    /**
     * Struct representing a vertex in a mesh.
     */
    template <typename TPosition, typename TNormal, typename TColor, typename TTexture>
    struct basic_vertex
    {

      /* -- Utility -- */

    private:

      /** Utility for conditionally enabling functions. */
      template <typename TVector, typename TVectorValue, typename TResult>
      using if_value = typename std::enable_if<std::is_same<typename TVector::value_type, TVectorValue>::value, TResult>::type;

      /* -- Types -- */

    public:

      /** Data type used for the `position` field. */
      using position_type = TPosition;

      /** Data type used for the `normal` field. */
      using normal_type = TNormal;

      /** Data type used for the `color` field. */
      using color_type = TColor;

      /** Data type used for the `texture` field. */
      using texture_type = TTexture;

      /* -- Constants -- */

    public:

      /** The size of the `position` field, in bytes. */
      static constexpr size_t position_size()
      {
        return sizeof(position_type);
      }

      /** The number of values in the `position` field. */
      static constexpr size_t position_count()
      {
        return sizeof(position_type) / sizeof(typename position_type::value_type);
      }

      /** The size of the `normal` field, in bytes. */
      static constexpr size_t normal_size()
      {
        return sizeof(normal_type);
      }

      /** The number of values in the `normal` field. */
      static constexpr size_t normal_count()
      {
        return sizeof(normal_type) / sizeof(typename normal_type::value_type);
      }

      /** The size of the `color` field, in bytes. */
      static constexpr size_t color_size()
      {
        return sizeof(color_type);
      }

      /** The number of values in the `color` field. */
      static constexpr size_t color_count()
      {
        return sizeof(color_type) / sizeof(typename color_type::value_type);
      }

      /** The size of the `texture` field, in bytes. */
      static constexpr size_t texture_size()
      {
        return sizeof(texture_type);
      }

      /** The number of values in the `texture` field. */
      static constexpr size_t texture_count()
      {
        return sizeof(texture_type) / sizeof(typename texture_type::value_type);
      }

      /* -- `float` Specializations -- */

    public:

      /** The OpenGL data type used for the `position` field. */
      template <typename TPos = position_type>
      static constexpr if_value<TPos, float, GLenum> position_datatype()
      {
        return GL_FLOAT;
      }

      /** If `true`, values in the `position` field should be normalized. */
      template <typename TPos = position_type>
      static constexpr if_value<TPos, float, bool> position_normalized()
      {
        return false;
      }

      /** The OpenGL data type used for the `normal` field. */
      template <typename TPos = normal_type>
      static constexpr if_value<TPos, float, GLenum> normal_datatype()
      {
        return GL_FLOAT;
      }

      /** If `true`, values in the `normal` field should be normalized. */
      template <typename TPos = normal_type>
      static constexpr if_value<TPos, float, bool> normal_normalized()
      {
        return false;
      }

      /** The OpenGL data type used for the `color` field. */
      template <typename TPos = color_type>
      static constexpr if_value<TPos, float, GLenum> color_datatype()
      {
        return GL_FLOAT;
      }

      /** If `true`, values in the `color` field should be normalized. */
      template <typename TPos = color_type>
      static constexpr if_value<TPos, float, bool> color_normalized()
      {
        return false;
      }

      /** The OpenGL data type used for the `texture` field. */
      template <typename TPos = texture_type>
      static constexpr if_value<TPos, float, GLenum> texture_datatype()
      {
        return GL_FLOAT;
      }

      /** If `true`, values in the `texture` field should be normalized. */
      template <typename TPos = texture_type>
      static constexpr if_value<TPos, float, bool> texture_normalized()
      {
        return false;
      }

      /* -- Fields -- */

    public:

      /** The position of the vertex. */
      position_type position;

      /** The normal vector of the vertex. */
      normal_type normal;

      /** The color of the vertex. */
      color_type color;

      /** The texture coordinate of the vertex. */
      texture_type texture;

    };

  }

  /**
   * Standard vertex type used by the application.
   */
  using vertex = lineage::templates::basic_vertex<glm::vec3, glm::vec3, glm::vec4, glm::vec2>;

}

/* -- Procedures -- */

namespace lineage
{

  /**
   * Creates an attribute spec for the `position` field of the specified vertex type.
   */
  template <typename TVertex>
  constexpr lineage::attribute_spec position_attribute_spec()
  {
    constexpr lineage::attribute_spec spec =
    {
      TVertex::position_count(),
      TVertex::position_datatype(),
      TVertex::position_normalized(),
      offsetof(TVertex, position),
    };
    return spec;
  }

  /**
   * Creates an attribute spec for the `normal` field of the specified vertex type.
   */
  template <typename TVertex>
  constexpr lineage::attribute_spec normal_attribute_spec()
  {
    constexpr lineage::attribute_spec spec =
    {
      TVertex::normal_count(),
      TVertex::normal_datatype(),
      TVertex::normal_normalized(),
      offsetof(TVertex, normal),
    };
    return spec;
  }

  /**
   * Creates an attribute spec for the `color` field of the specified vertex type.
   */
  template <typename TVertex>
  constexpr lineage::attribute_spec color_attribute_spec()
  {
    constexpr lineage::attribute_spec spec =
    {
      TVertex::color_count(),
      TVertex::color_datatype(),
      TVertex::color_normalized(),
      offsetof(TVertex, color),
    };
    return spec;
  }

  /**
   * Creates an attribute spec for the `texture` field of the specified vertex type.
   */
  template <typename TVertex>
  constexpr lineage::attribute_spec texture_attribute_spec()
  {
    constexpr lineage::attribute_spec spec =
    {
      TVertex::texture_count(),
      TVertex::texture_datatype(),
      TVertex::texture_normalized(),
      offsetof(TVertex, texture),
    };
    return spec;
  }

}
