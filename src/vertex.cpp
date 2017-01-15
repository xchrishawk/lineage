/**
 * @file	vertex.cpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/14
 */

/* -- Includes -- */

#include "vertex.hpp"

/* -- Namespaces -- */

using namespace lineage;

/* -- Constants -- */

const attribute_spec vertex::position_spec =
{
  vertex::position_count,	// count
  GL_FLOAT,			// type
  false,			// normalized
  vertex::position_offset,	// relative_offset
};

constexpr attribute_spec vertex::normal_spec =
{
  vertex::normal_count,		// count
  GL_FLOAT,			// type
  false,			// normalized
  vertex::normal_offset,	// relative_offset
};

constexpr attribute_spec vertex::color_spec =
{
  vertex::color_count,		// count
  GL_FLOAT,			// type
  false,			// normalized
  vertex::color_offset,		// relative_offset
};
