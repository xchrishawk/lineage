/**
 * @file	shader_source.cpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/12
 */

/* -- Includes -- */

#include <stdexcept>

#include "shader_source.hpp"
#include "util.hpp"

/* -- Namespaces -- */

using namespace lineage;

/* -- Constants -- */

namespace
{
  // prototype vertex shader
  const char PROTOTYPE_VERTEX_SHADER_SOURCE_ARRAY[] =
  {
    #include "prototype_vertex_shader.glsl.inc"
  };
  const std::string PROTOTYPE_VERTEX_SHADER_SOURCE(
    PROTOTYPE_VERTEX_SHADER_SOURCE_ARRAY,
    array_size(PROTOTYPE_VERTEX_SHADER_SOURCE_ARRAY));

  // prototype fragment shader
  const char PROTOTYPE_FRAGMENT_SHADER_SOURCE_ARRAY[] =
  {
    #include "prototype_fragment_shader.glsl.inc"
  };
  const std::string PROTOTYPE_FRAGMENT_SHADER_SOURCE(
    PROTOTYPE_FRAGMENT_SHADER_SOURCE_ARRAY,
    array_size(PROTOTYPE_FRAGMENT_SHADER_SOURCE_ARRAY));

  // default vertex shader
  const char DEFAULT_VERTEX_SHADER_SOURCE_ARRAY[] =
  {
    #include "default_vertex_shader.glsl.inc"
  };
  const std::string DEFAULT_VERTEX_SHADER_SOURCE(
    DEFAULT_VERTEX_SHADER_SOURCE_ARRAY,
    array_size(DEFAULT_VERTEX_SHADER_SOURCE_ARRAY));

  // default fragment shader
  const char DEFAULT_FRAGMENT_SHADER_SOURCE_ARRAY[] =
  {
    #include "default_fragment_shader.glsl.inc"
  };
  const std::string DEFAULT_FRAGMENT_SHADER_SOURCE(
    DEFAULT_FRAGMENT_SHADER_SOURCE_ARRAY,
    array_size(DEFAULT_FRAGMENT_SHADER_SOURCE_ARRAY));
}

/* -- Procedures -- */

const std::string& lineage::shader_source_string(shader_source shader)
{
  switch (shader)
  {
  case shader_source::prototype_vertex_shader:
    return PROTOTYPE_VERTEX_SHADER_SOURCE;
  case shader_source::prototype_fragment_shader:
    return PROTOTYPE_FRAGMENT_SHADER_SOURCE;
  case shader_source::default_vertex_shader:
    return DEFAULT_VERTEX_SHADER_SOURCE;
  case shader_source::default_fragment_shader:
    return DEFAULT_FRAGMENT_SHADER_SOURCE;
  default:
    throw std::invalid_argument("Source code for unknown shader requested!");
  }
}
