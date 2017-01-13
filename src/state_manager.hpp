/**
 * @file	state_manager.hpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/12
 */

#pragma once

/* -- Types -- */

namespace lineage
{

  /**
   * Struct containing arguments required to run an interation of the state loop.
   */
  struct state_args
  {
    double abs_t;		/**< Total elapsed time, in seconds. */
    double delta_t;		/**< Time since last loop, in seconds. */
  };

  /**
   * Abstract interface for types managing the current application state.
   */
  class state_manager
  {

    /* -- Lifecycle -- */

  protected:

    state_manager() = default;

  public:

    virtual ~state_manager() = default;

  private:

    state_manager(const lineage::state_manager&) = delete;
    state_manager(lineage::state_manager&&) = delete;
    lineage::state_manager& operator =(const lineage::state_manager&) = delete;
    lineage::state_manager& operator =(lineage::state_manager&&) = delete;

    /* -- Public Methods -- */

  public:

    /**
     * Runs an iteration of the state loop.
     *
     * @param args
     * The arguments for this iteration of the state loop.
     */
    virtual void run(const state_args& args) = 0;

    /**
     * The target time between state loop iterations, in seconds.
     */
    virtual double target_delta_t() const = 0;

  };

}
