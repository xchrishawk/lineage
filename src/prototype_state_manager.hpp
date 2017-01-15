/**
 * @file	prototype_state_manager.hpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/12
 */

#pragma once

/* -- Includes -- */

#include <glm/glm.hpp>

#include "input_manager.hpp"
#include "state_manager.hpp"

/* -- Types -- */

namespace lineage
{

  /**
   * State manager used for prototyping and testing.
   */
  class prototype_state_manager final : public lineage::state_manager
  {

    /* -- Lifecycle -- */

  public:

    /**
     * Constructs a `lineage::prototype_state_manager` instance.
     *
     * @param input_manager
     * The input manager currently in use by the application.
     */
    prototype_state_manager(const lineage::input_manager& input_manager);

    virtual ~prototype_state_manager() = default;

  private:

    prototype_state_manager(const lineage::prototype_state_manager&) = delete;
    prototype_state_manager(lineage::prototype_state_manager&&) = delete;
    lineage::prototype_state_manager& operator =(const lineage::prototype_state_manager&) = delete;
    lineage::prototype_state_manager& operator =(lineage::prototype_state_manager&&) = delete;

    /* -- Public Methods -- */

  public:

    /**
     * The background color to render.
     */
    const glm::vec4& background_color() const
    {
      return m_background_color;
    }

    /* -- `lineage::state_manager` Implementation -- */

  public:

    virtual void run(const lineage::state_args& args) override;
    virtual double target_delta_t() const override;

    /* -- Implementation -- */

  private:

    const lineage::input_manager& m_input_manager;
    glm::vec4 m_background_color;

    void update_background_color(const lineage::state_args& args);

  };

}
