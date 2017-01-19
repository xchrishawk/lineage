/**
 * @file	object.hpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/18
 */

#pragma once

/* -- Includes -- */

#include <utility>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "mesh.hpp"
#include "util.hpp"

/* -- Types -- */

namespace lineage
{

  /**
   * Class representing a renderable object.
   */
  template <typename TVertex>
  class object final
  {

    /* -- Types -- */

  public:

    /** The type of mesh that this object uses. */
    using mesh_type = lineage::mesh<TVertex>;

    /* -- Lifecycle -- */

  public:

    /**
     * Constructs a new `lineage::object` with the specified meshes.
     *
     * @note
     * The position, rotation, and scale will be set to default values.
     */
    object(std::vector<mesh_type> meshes)
      : object(POSITION_ZERO, ROTATION_ZERO, SCALE_NONE, std::move(meshes))
    { }

    /**
     * Constructs a new `lineage::object` with the specified parameters.
     */
    object(const glm::vec3& position,
           const glm::quat& rotation,
           const glm::vec3& scale,
           std::vector<mesh_type> meshes)
      : m_position(position),
        m_rotation(rotation),
        m_scale(scale),
        m_meshes(std::move(meshes))
    { }

    /**
     * Move constructor.
     */
    object(lineage::object<TVertex>&& other) noexcept
      : m_position(other.m_position),
        m_rotation(other.m_rotation),
        m_scale(other.m_scale),
        m_meshes(std::move(other.m_meshes))
    { }

    /**
     * Move assignment operator.
     */
    lineage::object<TVertex>& operator =(lineage::object<TVertex>&& other)
    {
      m_position = other.m_position;
      m_rotation = other.m_rotation;
      m_scale = other.m_scale;
      m_meshes = std::move(other.m_meshes);
      return *this;
    }

    /**
     * Destructor.
     */
    ~object() = default;

  private:

    object(const lineage::object<TVertex>&) = delete;
    lineage::object<TVertex>& operator =(const lineage::object<TVertex>&) = delete;

    /* -- Public Methods -- */

  public:

    /**
     * The position of the object, relative to the world origin.
     */
    const glm::vec3& position() const
    {
      return m_position;
    }

    /**
     * The rotation of the object, relative to the world origin.
     */
    const glm::quat& rotation() const
    {
      return m_rotation;
    }

    /**
     * The scale of this object.
     */
    const glm::vec3& scale() const
    {
      return m_scale;
    }

    /**
     * The meshes which make up this object.
     */
    const std::vector<mesh_type>& meshes() const
    {
      return m_meshes;
    }

    /* -- Implementation -- */

  private:

    glm::vec3 m_position;
    glm::quat m_rotation;
    glm::vec3 m_scale;
    std::vector<mesh_type> m_meshes;

  };

}
