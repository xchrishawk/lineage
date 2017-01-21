/**
 * @file	scene_node.hpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/20
 */

#pragma once

/* -- Includes -- */

#include <memory>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "util.hpp"

/* -- Types -- */

namespace lineage
{

  /**
   * Class representing a node in a scene graph.
   */
  class scene_node
  {

    /* -- Lifecycle -- */

  public:

    /**
     * Constructs a new `lineage::scene_node` instance with default values.
     */
    scene_node();

    /**
     * Constructs a new `lineage::scene_node` instance with the specified parameters.
     */
    scene_node(std::vector<size_t> meshes,
               std::vector<lineage::scene_node> children,
               const glm::vec3& position,
               const glm::quat& rotation,
               const glm::vec3& scale);

    /* -- Public Methods -- */

  public:

    /**
     * The meshes which should be rendered for this node.
     */
    std::vector<size_t>& meshes();

    /**
     * The meshes which should be rendered for this node.
     */
    const std::vector<size_t>& meshes() const;

    /**
     * The child nodes of this node.
     */
    std::vector<lineage::scene_node>& children();

    /**
     * The child nodes of this node.
     */
    const std::vector<lineage::scene_node>& children() const;

    /**
     * Returns the position of this node, relative to its parent.
     */
    glm::vec3 position() const;

    /**
     * Sets the position of this node, relative to its parent.
     */
    void set_position(const glm::vec3& position);

    /**
     * Returns the rotation of this node, relative to its parent.
     */
    glm::quat rotation() const;

    /**
     * Sets the rotation of this node, relative to its parent.
     */
    void set_rotation(const glm::quat& rotation);

    /**
     * Returns the scale of this node, relative to its parent.
     */
    glm::vec3 scale() const;

    /**
     * Sets the scale of this node, relative to its parent.
     */
    void set_scale(const glm::vec3& scale);

    /* -- Implementation -- */

  private:

    std::vector<size_t> m_meshes;
    std::vector<lineage::scene_node> m_children;
    glm::vec3 m_position;
    glm::quat m_rotation;
    glm::vec3 m_scale;

  };

}
