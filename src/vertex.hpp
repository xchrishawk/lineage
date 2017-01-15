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
  namespace internal
  {

    /**
     * Template for selectively enabling functions based on the value type of a vector.
     */
    template <typename TVector, typename TVectorValue, typename TResult>
    using if_value =
      typename std::enable_if<
        std::is_same<
          typename TVector::value_type,
          TVectorValue>::value,
        TResult>::type;

    /**
     * Mix-in adding vertex position information to a struct.
     */
    template <typename T>
    struct vertex_position
    {

      /* -- Types -- */

    public:

      /** The vector type used for the vertex's position. */
      using position_vector = T;

      /* -- Constants -- */

    public:

      /** The size of the `position` field, in bytes. */
      static constexpr size_t position_size()
      {
        return sizeof(position_vector);
      }

      /** The number of values in the `position` field. */
      static constexpr size_t position_count()
      {
        return sizeof(position_vector) / sizeof(typename position_vector::value_type);
      }

      /* -- `float` Specializations -- */

    public:

      /** The OpenGL data type used for the `position` field. */
      template <typename TPos = position_vector>
      static constexpr lineage::internal::if_value<TPos, float, GLenum> position_type()
      {
        return GL_FLOAT;
      }

      /** If `true`, position values should be normalized. */
      template <typename TPos = position_vector>
      static constexpr lineage::internal::if_value<TPos, float, bool> position_normalized()
      {
        return false;
      }

      /* -- Lifecycle -- */

    protected:

      /**
       * Constructs a new instance with the specified arguments.
       */
      template <typename... TArgs>
      vertex_position(TArgs... args)
        : position(std::forward<TArgs>(args)...)
      { }

      /* -- Fields -- */

    public:

      /** The position of the vertex. */
      position_vector position;

    };

    /**
     * Mix-in adding vertex normal vector information to a struct.
     */
    template <typename T>
    struct vertex_normal
    {

      /* -- Types -- */

      /** The vector type used for the vertex's normal vector. */
      using normal_vector = T;

      /* -- Constants -- */

    public:

      /** The size of the `normal` field, in bytes. */
      static constexpr size_t normal_size()
      {
        return sizeof(normal_vector);
      }

      /** The number of values in the `normal` field. */
      static constexpr size_t normal_count()
      {
        return sizeof(normal_vector) / sizeof(typename normal_vector::value_type);
      }

      /* -- `float` Specializations -- */

    public:

      /** The OpenGL data type used for the `normal` field. */
      template <typename TPos = normal_vector>
      static constexpr lineage::internal::if_value<TPos, float, GLenum> normal_type()
      {
        return GL_FLOAT;
      }

      /** If `true`, normal values should be normalized. */
      template <typename TPos = normal_vector>
      static constexpr lineage::internal::if_value<TPos, float, bool> normal_normalized()
      {
        return false;
      }

      /* -- Lifecycle -- */

    protected:

      /**
       * Constructs a new instance with the specified arguments.
       */
      template <typename... TArgs>
      vertex_normal(TArgs... args)
        : normal(std::forward<TArgs>(args)...)
      { }

      /* -- Fields -- */

    public:

      /** The normal vector of the vertex. */
      normal_vector normal;

    };

    /**
     * Mix-in adding vertex color information to a struct.
     */
    template <typename T>
    struct vertex_color
    {

      /* -- Types -- */

    public:

      /** The vector type used for the vertex's color. */
      using color_vector = T;

      /* -- Constants -- */

    public:

      /** The size of the `color` field, in bytes. */
      static constexpr size_t color_size()
      {
        return sizeof(color_vector);
      }

      /** The number of values in the `color` field. */
      static constexpr size_t color_count()
      {
        return sizeof(color_vector) / sizeof(typename color_vector::value_type);
      }

      /* -- `float` Specializations -- */

    public:

      /** The OpenGL data type used for the `color` field. */
      template <typename TPos = color_vector>
      static constexpr lineage::internal::if_value<TPos, float, GLenum> color_type()
      {
        return GL_FLOAT;
      }

      /** If `true`, color values should be normalized. */
      template <typename TPos = color_vector>
      static constexpr lineage::internal::if_value<TPos, float, bool> color_normalized()
      {
        return false;
      }

      /* -- Lifecycle -- */

    protected:

      /**
       * Constructs a new instance with the specified arguments.
       */
      template <typename... TArgs>
      vertex_color(TArgs... args)
        : color(std::forward<TArgs>(args)...)
      { }

      /* -- Fields -- */

    public:

      /** The color of the vertex. */
      color_vector color;

    };

    /**
     * Struct representing a vertex with only position data.
     */
    template <typename TPosition>
    struct vertex_p : public lineage::internal::vertex_position<TPosition>
    {

      /* -- Lifecycle -- */

    public:

      /**
       * Constructs a new vertex with default values.
       */
      vertex_p()
        : lineage::internal::vertex_position<TPosition>()
      { }

      /**
       * Constructs a new vertex with the specified position.
       */
      vertex_p(const TPosition& position)
        : lineage::internal::vertex_position<TPosition>(position)
      { }

    };

    /**
     * Struct representing a vertex with position and normal data.
     */
    template <typename TPosition, typename TNormal>
    struct vertex_pn : public lineage::internal::vertex_position<TPosition>,
                       public lineage::internal::vertex_normal<TNormal>
    {

      /* -- Lifecycle -- */

    public:

      /**
       * Constructs a new vertex with default values.
       */
      vertex_pn()
        : lineage::internal::vertex_position<TPosition>(),
          lineage::internal::vertex_normal<TNormal>()
      { }

      /**
       * Constructs a new vertex with the specified position and normal.
       */
      vertex_pn(const TPosition& position, const TNormal& normal)
        : lineage::internal::vertex_position<TPosition>(position),
          lineage::internal::vertex_normal<TNormal>(normal)
      { }

    };

    /**
     * Struct representing a vertex with position and color data.
     */
    template <typename TPosition, typename TColor>
    struct vertex_pc : public lineage::internal::vertex_position<TPosition>,
                       public lineage::internal::vertex_color<TColor>
    {

      /* -- Lifecycle -- */

    public:

      /**
       * Constructs a new vertex with default values.
       */
      vertex_pc()
        : lineage::internal::vertex_position<TPosition>(),
          lineage::internal::vertex_color<TColor>()
      { }

      /**
       * Constructs a new vertex with the specified position and color.
       */
      vertex_pc(const TPosition& position, const TColor& color)
        : lineage::internal::vertex_position<TPosition>(position),
          lineage::internal::vertex_color<TColor>(color)
      { }

    };

    /**
     * Struct representing a vertex with position, normal, and color data.
     */
    template <typename TPosition, typename TNormal, typename TColor>
    struct vertex_pnc : public lineage::internal::vertex_position<TPosition>,
                        public lineage::internal::vertex_normal<TNormal>,
                        public lineage::internal::vertex_color<TColor>
    {

      /* -- Lifecycle -- */

    public:

      /**
       * Constructs a new vertex with default values.
       */
      vertex_pnc()
        : lineage::internal::vertex_position<TPosition>(),
          lineage::internal::vertex_normal<TNormal>(),
          lineage::internal::vertex_color<TColor>()
      { }

      /**
       * Constructs a new vertex with the specified position, normal, and color.
       */
      vertex_pnc(const TPosition& position, const TNormal& normal, const TColor& color)
        : lineage::internal::vertex_position<TPosition>(position),
          lineage::internal::vertex_normal<TNormal>(normal),
          lineage::internal::vertex_color<TColor>(color)
      { }

    };

  }

  /**
   * Typedef for vertices using 3-dimensional positions.
   */
  using vertex3xx = lineage::internal::vertex_p<glm::vec3>;

  /**
   * Typedef for vertices using 3-dimensional positions and normals.
   */
  using vertex33x = lineage::internal::vertex_pn<glm::vec3, glm::vec3>;

  /**
   * Typedef for vertices using 3-dimensional positions and colors.
   */
  using vertex3x3 = lineage::internal::vertex_pc<glm::vec3, glm::vec3>;

  /**
   * Typedef for vertices using 3-dimensional positions and 4-dimensional colors.
   */
  using vertex3x4 = lineage::internal::vertex_pc<glm::vec3, glm::vec4>;

  /**
   * Typedef for vertices using 3-dimensional positions, normals, and colors.
   */
  using vertex333 = lineage::internal::vertex_pnc<glm::vec3, glm::vec3, glm::vec3>;

  /**
   * Typedef for vertices using 3-dimensional positions and normals and 4-dimensional colors.
   */
  using vertex334 = lineage::internal::vertex_pnc<glm::vec3, glm::vec3, glm::vec4>;

}

/* -- Procedures -- */

namespace lineage
{

  /**
   * Creates an attribute spec for the `position` field of the specified vertex type.
   */
  template <typename T>
  constexpr lineage::attribute_spec position_attribute_spec()
  {
    constexpr lineage::attribute_spec spec =
    {
      T::position_count(),
      T::position_type(),
      T::position_normalized(),
      offsetof(T, position),
    };
    return spec;
  }

  /**
   * Creates an attribute spec for the `normal` field of the specified vertex type.
   */
  template <typename T>
  constexpr lineage::attribute_spec normal_attribute_spec()
  {
    constexpr lineage::attribute_spec spec =
    {
      T::normal_count(),
      T::normal_type(),
      T::normal_normalized(),
      offsetof(T, normal),
    };
    return spec;
  }

  /**
   * Creates an attribute spec for the `color` field of the specified vertex type.
   */
  template <typename T>
  constexpr lineage::attribute_spec color_attribute_spec()
  {
    constexpr lineage::attribute_spec spec =
    {
      T::color_count(),
      T::color_type(),
      T::color_normalized(),
      offsetof(T, color),
    };
    return spec;
  }

}
