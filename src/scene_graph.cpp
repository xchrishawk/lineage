/**
 * @file	scene_graph.cpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/20
 */

/* -- Includes -- */

#include <memory>
#include <vector>

#include "mesh.hpp"
#include "scene_graph.hpp"
#include "scene_node.hpp"

/* -- Namespaces -- */

using namespace lineage;

/* -- Procedures -- */

scene_graph::scene_graph()
  : m_meshes(),
    m_nodes()
{
}

scene_graph::scene_graph(std::vector<std::unique_ptr<mesh>> meshes,
                         std::vector<scene_node> nodes)
  : m_meshes(std::move(meshes)),
    m_nodes(std::move(nodes))
{
}

scene_graph::scene_graph(scene_graph&& other) noexcept
  : m_meshes(std::move(other.m_meshes)),
    m_nodes(std::move(other.m_nodes))
{
}

scene_graph& scene_graph::operator =(scene_graph&& other) noexcept
{
  m_meshes = std::move(other.m_meshes);
  m_nodes = std::move(other.m_nodes);
  return *this;
}

std::vector<std::unique_ptr<mesh>>& scene_graph::meshes()
{
  return m_meshes;
}

const std::vector<std::unique_ptr<mesh>>& scene_graph::meshes() const
{
  return m_meshes;
}

std::vector<scene_node>& scene_graph::nodes()
{
  return m_nodes;
}

const std::vector<scene_node>& scene_graph::nodes() const
{
  return m_nodes;
}
