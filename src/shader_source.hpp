/**
 * @file	shader_source.hpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/12
 */

#pragma once

/* -- Includes -- */

#include <string>

/* -- Types -- */

namespace lineage
{

  /**
   * Enumeration of the shader source files known to the app.
   */
  enum class shader_source
  {
    prototype_vertex_shader,
    prototype_fragment_shader,
    default_vertex_shader,
    default_fragment_shader,
  };

}

/* -- Procedure Prototypes -- */

namespace lineage
{

  /**
   * Gets the source code for the specified shader, as a `std::string`.
   *
   * @exception std::invalid_argument
   * Thrown if the requested shader is invalid.
   */
  const std::string& shader_source_string(lineage::shader_source shader);

}
