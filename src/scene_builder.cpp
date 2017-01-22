/**
 * @file	scene_builder.cpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/22
 */

/* -- Includes -- */

#include <memory>
#include <vector>

#include <glm/glm.hpp>

#include "api.hpp"
#include "constants.hpp"
#include "mesh.hpp"
#include "scene_builder.hpp"
#include "scene_graph.hpp"
#include "scene_node.hpp"
#include "vertex.hpp"

/* -- Namespaces -- */

using namespace lineage;

/* -- Private Procedures -- */

namespace
{

  /** Creates a square mesh. */
  std::unique_ptr<mesh> square_mesh(const glm::vec4& color)
  {
    static const GLenum DRAW_MODE = GL_TRIANGLE_FAN;
    static const std::vector<GLuint> INDICES { 0, 1, 2, 3 };

    std::vector<vertex> vertices
    {
      { { -0.5f, -0.5f, 0.0f }, { }, color, { } },
      { { 0.5f, -0.5f, 0.0f }, { }, color, { } },
      { { 0.5f, 0.5f, 0.0f }, { }, color, { } },
      { { -0.5f, 0.5f, 0.0f }, { }, color, { } },
    };

    return std::make_unique<mesh>(DRAW_MODE, vertices, INDICES);
  }

  /** Creates a node for a cube. */
  scene_node cube_node(GLuint square_mesh_index)
  {
    scene_node parent;

    auto& children = parent.children();
    children.resize(6);

    // front face
    children[0].meshes().assign({ square_mesh_index });
    children[0].set_rotation(ROTATION_NONE);
    children[0].set_position(glm::vec3(0.0f, 0.0f, 0.5f));

    // back face
    children[1].meshes().assign({ square_mesh_index });
    children[1].set_rotation(glm::rotate(ROTATION_NONE, deg_to_rad(180.0f), VEC3_UNIT_Y));
    children[1].set_position(glm::vec3(0.0f, 0.0f, -0.5f));

    // left face
    children[2].meshes().assign({ square_mesh_index });
    children[2].set_rotation(glm::rotate(ROTATION_NONE, deg_to_rad(90.0f), VEC3_UNIT_Y));
    children[2].set_position(glm::vec3(-0.5f, 0.0f, 0.0f));

    // right face
    children[3].meshes().assign({ square_mesh_index });
    children[3].set_rotation(glm::rotate(ROTATION_NONE, deg_to_rad(-90.0f), VEC3_UNIT_Y));
    children[3].set_position(glm::vec3(0.5f, 0.0f, 0.0f));

    // top face
    children[4].meshes().assign({ square_mesh_index });
    children[4].set_rotation(glm::rotate(ROTATION_NONE, deg_to_rad(-90.0f), VEC3_UNIT_X));
    children[4].set_position(glm::vec3(0.0f, 0.5f, 0.0f));

    // bottom face
    children[5].meshes().assign({ square_mesh_index });
    children[5].set_rotation(glm::rotate(ROTATION_NONE, deg_to_rad(90.0f), VEC3_UNIT_X));
    children[5].set_position(glm::vec3(0.0f, -0.5f, 0.0f));

    return parent;
  }

}

/* -- Procedures -- */

scene_graph lineage::create_single_cube_scene_graph(const glm::vec4& color)
{
  scene_graph graph;
  graph.meshes().push_back(square_mesh(color));
  graph.nodes().push_back(cube_node(0));
  return graph;
}


scene_graph lineage::create_multiple_cubes_scene_graph()
{
  scene_graph graph;

  auto& meshes = graph.meshes();
  meshes.push_back(square_mesh(COLOR_WHITE));
  meshes.push_back(square_mesh(COLOR_RED));
  meshes.push_back(square_mesh(COLOR_GREEN));
  meshes.push_back(square_mesh(COLOR_BLUE));
  meshes.push_back(square_mesh(COLOR_CYAN));
  meshes.push_back(square_mesh(COLOR_MAGENTA));
  meshes.push_back(square_mesh(COLOR_YELLOW));

  auto& nodes = graph.nodes();

  scene_node center = cube_node(0);
  center.set_scale(glm::vec3(2.0f, 2.0f, 2.0f));
  nodes.push_back(center);

  scene_node left = cube_node(1);
  left.set_position(glm::vec3(-3.0f, 0.0f, 0.0f));
  nodes.push_back(left);

  scene_node top = cube_node(2);
  top.set_position(glm::vec3(0.0f, 3.0f, 0.0f));
  nodes.push_back(top);

  scene_node front = cube_node(3);
  front.set_position(glm::vec3(0.0f, 0.0f, 3.0f));
  nodes.push_back(front);

  scene_node right = cube_node(4);
  right.set_position(glm::vec3(3.0f, 0.0f, 0.0f));
  nodes.push_back(right);

  scene_node bottom = cube_node(5);
  bottom.set_position(glm::vec3(0.0f, -3.0f, 0.0f));
  nodes.push_back(bottom);

  scene_node back = cube_node(6);
  back.set_position(glm::vec3(0.0f, 0.0f, -3.0f));
  nodes.push_back(back);

  return graph;
}
