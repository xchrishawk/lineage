/**
 * @file	mesh.hpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/17
 */

#pragma once

/* -- Includes -- */

#include <memory>
#include <utility>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "buffer.hpp"
#include "util.hpp"
#include "vertex.hpp"

/* -- Types -- */

namespace lineage
{

  /**
   * Class representing a renderable mesh of vertices.
   */
  template <typename TVertex>
  class mesh final
  {

    /* -- Types -- */

  public:

    /** The type of vertex that this mesh uses. */
    using vertex_type = TVertex;

    /* -- Lifecycle -- */

  public:

    /**
     * Constructs a new `lineage::mesh` with the specified vertices.
     *
     * @note
     * The position, rotation, and scale will be set to default values.
     */
    mesh(const std::vector<TVertex>& vertices)
      : mesh(POSITION_ZERO, ROTATION_ZERO, SCALE_NONE, vertices)
    { }

    /**
     * Constructs a new `lineage::mesh` with the specified parameters.
     */
    mesh(const glm::vec3& position,
         const glm::quat& rotation,
         const glm::vec3& scale,
         const std::vector<TVertex>& vertices)
      : m_position(position),
        m_rotation(rotation),
        m_scale(scale),
        m_vertex_count(vertices.size()),
        m_buffer(sizeof(vertex_type) * vertices.size(), vertices.data(), 0)
    { }

    /**
     * Move constructor.
     */
    mesh(lineage::mesh<TVertex>&& other) noexcept
      : m_position(other.m_position),
        m_rotation(other.m_rotation),
        m_scale(other.m_scale),
        m_vertex_count(other.m_vertex_count),
        m_buffer(std::move(other.m_buffer))
    { }

    /**
     * Move assignment operator.
     */
    lineage::mesh<TVertex>& operator =(lineage::mesh<TVertex>&& other)
    {
      m_position = other.m_position;
      m_rotation = other.m_rotation;
      m_scale = other.m_scale;
      m_vertex_count = other.m_vertex_count;
      m_buffer = std::move(other.m_buffer);
      return *this;
    }

    /**
     * Destructor.
     */
    ~mesh() = default;

  private:

    mesh(const lineage::mesh<TVertex>&) = delete;
    lineage::mesh<TVertex>& operator =(const lineage::mesh<TVertex>&) = delete;

    /* -- Public Methods -- */

  public:

    /**
     * The position of this mesh, relative to the object which owns it.
     */
    const glm::vec3& position() const
    {
      return m_position;
    }

    /**
     * The rotation of this mesh, relative to the object which owns it.
     */
    const glm::quat& rotation() const
    {
      return m_rotation;
    }

    /**
     * The scale of this mesh, relative to the object which owns it.
     */
    const glm::vec3& scale() const
    {
      return m_scale;
    }

    /**
     * The number of vertices in this mesh.
     */
    const size_t vertex_count() const
    {
      return m_vertex_count;
    }

    /**
     * The vertex data buffer for this mesh.
     */
    const lineage::buffer& buffer() const
    {
      return m_buffer;
    }

    /* -- Implementation -- */

  private:

    glm::vec3 m_position;
    glm::quat m_rotation;
    glm::vec3 m_scale;
    size_t m_vertex_count;
    lineage::immutable_buffer m_buffer;

  };

}
