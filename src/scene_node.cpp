/**
 * @file	scene_node.cpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/20
 */

/* -- Includes -- */

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "constants.hpp"
#include "scene_node.hpp"

/* -- Namespaces -- */

using namespace lineage;

/* -- Procedures -- */

scene_node::scene_node()
  : m_meshes(),
    m_children(),
    m_position(POSITION_NONE),
    m_rotation(ROTATION_NONE),
    m_scale(SCALE_NONE)
{
}

scene_node::scene_node(std::vector<size_t> meshes,
                       std::vector<lineage::scene_node> children,
                       const glm::vec3& position,
                       const glm::quat& rotation,
                       const glm::vec3& scale)
  : m_meshes(std::move(meshes)),
    m_children(std::move(children)),
    m_position(position),
    m_rotation(rotation),
    m_scale(scale)
{
}

std::vector<size_t>& scene_node::meshes()
{
  return m_meshes;
}

const std::vector<size_t>& scene_node::meshes() const
{
  return m_meshes;
}

std::vector<lineage::scene_node>& scene_node::children()
{
  return m_children;
}

const std::vector<lineage::scene_node>& scene_node::children() const
{
  return m_children;
}

glm::vec3 scene_node::position() const
{
  return m_position;
}

void scene_node::set_position(const glm::vec3& position)
{
  m_position = position;
}

glm::quat scene_node::rotation() const
{
  return m_rotation;
}

void scene_node::set_rotation(const glm::quat& rotation)
{
  m_rotation = rotation;
}

glm::vec3 scene_node::scale() const
{
  return m_scale;
}

void scene_node::set_scale(const glm::vec3& scale)
{
  m_scale = scale;
}
