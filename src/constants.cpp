/**
 * @file	constants.cpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/20
 */

/* -- Includes -- */

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "constants.hpp"

/* -- Constants -- */

const glm::vec2 lineage::VEC2_ZERO 	{ 0.0f, 0.0f };
const glm::vec3 lineage::VEC3_ZERO 	{ 0.0f, 0.0f, 0.0f };
const glm::vec4 lineage::VEC4_ZERO 	{ 0.0f, 0.0f, 0.0f, 0.0f };

const glm::vec2 lineage::VEC2_UNIT_X 	{ 1.0f, 0.0f };
const glm::vec2 lineage::VEC2_UNIT_Y 	{ 0.0f, 1.0f };

const glm::vec3 lineage::VEC3_UNIT_X 	{ 1.0f, 0.0f, 0.0f };
const glm::vec3 lineage::VEC3_UNIT_Y 	{ 0.0f, 1.0f, 0.0f };
const glm::vec3 lineage::VEC3_UNIT_Z 	{ 0.0f, 0.0f, 1.0f };

const glm::vec4 lineage::VEC4_UNIT_X 	{ 1.0f, 0.0f, 0.0f, 0.0f };
const glm::vec4 lineage::VEC4_UNIT_Y 	{ 0.0f, 1.0f, 0.0f, 0.0f };
const glm::vec4 lineage::VEC4_UNIT_Z 	{ 0.0f, 0.0f, 1.0f, 0.0f };
const glm::vec4 lineage::VEC4_UNIT_W 	{ 0.0f, 0.0f, 0.0f, 1.0f };

const glm::vec3 lineage::POSITION_NONE	{ 0.0f, 0.0f, 0.0f };
const glm::quat lineage::ROTATION_NONE	{ };
const glm::vec3 lineage::SCALE_NONE	{ 1.0f, 1.0f, 1.0f };

const glm::vec4 lineage::COLOR_WHITE 	{ 1.0f, 1.0f, 1.0f, 1.0f };
const glm::vec4 lineage::COLOR_BLACK 	{ 0.0f, 0.0f, 0.0f, 1.0f };
const glm::vec4 lineage::COLOR_RED 	{ 1.0f, 0.0f, 0.0f, 1.0f };
const glm::vec4 lineage::COLOR_GREEN 	{ 0.0f, 1.0f, 0.0f, 1.0f };
const glm::vec4 lineage::COLOR_BLUE 	{ 0.0f, 0.0f, 1.0f, 1.0f };
const glm::vec4 lineage::COLOR_CYAN 	{ 0.0f, 1.0f, 1.0f, 1.0f };
const glm::vec4 lineage::COLOR_MAGENTA	{ 1.0f, 0.0f, 1.0f, 1.0f };
const glm::vec4 lineage::COLOR_YELLOW 	{ 1.0f, 1.0f, 0.0f, 1.0f };
