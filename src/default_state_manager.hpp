/**
 * @file	default_state_manager.hpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/15
 */

#pragma once

/* -- Includes -- */

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "input_manager.hpp"
#include "state_manager.hpp"

/* -- Types -- */

namespace lineage
{

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

    virtual ~default_state_manager() = default;

  private:

    default_state_manager(const lineage::default_state_manager&) = delete;
    default_state_manager(lineage::default_state_manager&&) = delete;
    lineage::default_state_manager& operator =(const lineage::default_state_manager&) = delete;
    lineage::default_state_manager& operator =(lineage::default_state_manager&&) = delete;

    /* -- Public Methods -- */

  public:

    /**
     * The current camera position.
     */
    const glm::vec3& camera_position() const
    {
      return m_camera_position;
    }

    /**
     * The current camera rotation.
     */
    const glm::quat& camera_rotation() const
    {
      return m_camera_rotation;
    }

    /**
     * The current camera field of view in the Y axis, in radians.
     */
    float camera_fov() const
    {
      return m_camera_fov;
    }

    /**
     * The current camera near clip distance.
     */
    float camera_clip_near() const
    {
      return m_camera_clip_near;
    }

    /**
     * The current camera far clip distance.
     */
    float camera_clip_far() const
    {
      return m_camera_clip_far;
    }

    /* -- `lineage::state_manager` Implementation -- */

  public:

    virtual void run(const lineage::state_args& args);
    virtual double target_delta_t() const;

    /* -- Implementation -- */

  private:

    const lineage::input_manager& m_input_manager;
    glm::vec3 m_camera_position;
    glm::quat m_camera_rotation;
    float m_camera_fov;
    float m_camera_clip_near;
    float m_camera_clip_far;

    void update_camera_position(const lineage::state_args& args);
    void update_camera_rotation(const lineage::state_args& args);
    void update_camera_fov(const lineage::state_args& args);
    bool input_active(lineage::input_type type);

  };

}
