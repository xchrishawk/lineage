/**
 * @file	scene_graph.hpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/20
 */

#pragma once

/* -- Includes -- */

#include <memory>
#include <vector>

#include "mesh.hpp"
#include "scene_node.hpp"

/* -- Types -- */

namespace lineage
{

  /**
   * Class for objects representing a renderable scene graph.
   */
  class scene_graph
  {

    /* -- Lifecycle -- */

  public:

    /**
     * Constructs a new `lineage::scene_graph` instance.
     */
    scene_graph();

    /**
     * Constructs a new `lineage::scene_graph` instance with the specified parameters.
     */
    scene_graph(std::vector<std::unique_ptr<lineage::mesh>> meshes,
                std::vector<lineage::scene_node> nodes);

    /**
     * Move constructor.
     */
    scene_graph(lineage::scene_graph&& other) noexcept;

    /**
     * Move assignment operator.
     */
    lineage::scene_graph& operator =(lineage::scene_graph&& other) noexcept;

    /**
     * Destructor.
     */
    ~scene_graph() = default;

  private:

    scene_graph(const lineage::scene_graph&) = delete;
    lineage::scene_graph& operator =(const lineage::scene_graph&) = delete;

    /* -- Public Methods -- */

  public:

    /**
     * The meshes used in this scene graph.
     */
    std::vector<std::unique_ptr<lineage::mesh>>& meshes();

    /**
     * The meshes used in this scene graph.
     */
    const std::vector<std::unique_ptr<lineage::mesh>>& meshes() const;

    /**
     * The top-level nodes in this scene graph.
     */
    std::vector<lineage::scene_node>& nodes();

    /**
     * The top-level nodes in this scene graph.
     */
    const std::vector<lineage::scene_node>& nodes() const;

    /* -- Implementation -- */

  private:

    std::vector<std::unique_ptr<lineage::mesh>> m_meshes;
    std::vector<lineage::scene_node> m_nodes;

  };

}
