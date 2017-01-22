/**
 * @file	default_state_manager.hpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/15
 */

#pragma once

/* -- Includes -- */

#include <memory>
#include <glm/glm.hpp>

#include "input_manager.hpp"
#include "state_manager.hpp"

/* -- Types -- */

namespace lineage
{

  class scene_graph;

  /**
   * Default application state manager object.
   */
  class default_state_manager final : public lineage::state_manager
  {

    /* -- Lifecycle -- */

  public:

    /**
     * Constructs a new `lineage::default_state_manager` instance.
     *
     * @param input_manager
     * The input manager in use by the application.
     */
    default_state_manager(const lineage::input_manager& input_manager);

    /**
     * Destructor.
     */
    virtual ~default_state_manager();

  private:

    default_state_manager(const lineage::default_state_manager&) = delete;
    default_state_manager(lineage::default_state_manager&&) = delete;
    lineage::default_state_manager& operator =(const lineage::default_state_manager&) = delete;
    lineage::default_state_manager& operator =(lineage::default_state_manager&&) = delete;

    /* -- Public Methods -- */

  public:

    /**
     * The scene graph which should be rendered.
     */
    const lineage::scene_graph& scene_graph() const;

    /**
     * The current camera position.
     */
    glm::vec3 camera_position() const;

    /**
     * The current camera rotation.
     */
    glm::quat camera_rotation() const;

    /**
     * The current camera field of view in the Y axis, in radians.
     */
    float camera_fov() const;

    /**
     * The current camera near clip distance.
     */
    float camera_clip_near() const;

    /**
     * The current camera far clip distance.
     */
    float camera_clip_far() const;

    /**
     * The background color which should be rendered.
     */
    glm::vec4 background_color() const;

    /* -- `lineage::state_manager` Implementation -- */

  public:

    virtual void run(const lineage::state_args& args);
    virtual double target_delta_t() const;

    /* -- Implementation -- */

  private:

    struct implementation;
    const std::unique_ptr<implementation> impl;

  };

}
