/**
 * @file	scene_builder.hpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/22
 */

/* -- Includes -- */

#include <glm/glm.hpp>
#include "scene_graph.hpp"

/* -- Procedure Prototypes -- */

namespace lineage
{

  /**
   * Creates a scene graph with a single cube at the origin.
   */
  lineage::scene_graph create_single_cube_scene_graph(const glm::vec4& color);

  /**
   * Creates a scene graph with multiple cubes.
   */
  lineage::scene_graph create_multiple_cubes_scene_graph();

}
