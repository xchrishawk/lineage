/**
 * @file	default_state_manager.hpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/15
 */

#pragma once

/* -- Includes -- */

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

    /* -- `lineage::state_manager` Implementation -- */

  public:

    virtual void run(const lineage::state_args& args);
    virtual double target_delta_t() const;

    /* -- Implementation -- */

  private:

    const lineage::input_manager& m_input_manager;

  };

}
