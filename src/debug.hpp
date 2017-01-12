/**
 * @file	debug.hpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/11
 */

/* -- Includes -- */

#include <iostream>
#include <string>
#include <vector>

/* -- Debug Macros -- */

#if defined(LINEAGE_DEBUG)

/** Calls `lineage::debug::fail()` if this is a debug build. */
#define lineage_dbg_fail()							\
  lineage::debug::fail()

/** Calls `lineage::debug::breakpoint()` if this is a debug build. */
#define lineage_dbg_breakpoint()						\
  lineage::debug::breakpoint()

#else

#define lineage_dbg_fail()
#define lineage_dbg_breakpoint()

#endif /* defined(LINEAGE_DEBUG) */

/* -- Logging Macros -- */

#if !defined(LINEAGE_NOLOG)

/** Prints a status debug message to the debug stream. */
#define lineage_log_status(...)							\
  do										\
  {										\
    lineage::debug::debug_stream()						\
      << lineage::debug::trace_message("lineage_log_status",			\
                                       __PRETTY_FUNCTION__,             	\
                                       __FILE__,                        	\
                                       __LINE__,	                    	\
                                       { __VA_ARGS__ })                 	\
      << std::endl;								\
  }										\
  while (false)

/** Prints a warning debug message to the debug stream. */
#define lineage_log_warning(...)						\
  do										\
  {										\
    lineage::debug::debug_stream()						\
      << lineage::debug::trace_message("lineage_log_warning",			\
                                       __PRETTY_FUNCTION__,             	\
                                       __FILE__,                        	\
                                       __LINE__,                    		\
                                       { __VA_ARGS__ })                 	\
      << std::endl;								\
  }										\
  while (false)

/** Prints an error debug message to the debug stream. */
#define lineage_log_error(...)							\
  do										\
  {										\
    lineage::debug::error_stream()						\
      << lineage::debug::trace_message("lineage_log_error",			\
                                       __PRETTY_FUNCTION__,             	\
                                       __FILE__,                        	\
                                       __LINE__,                    		\
                                       { __VA_ARGS__ })                 	\
      << std::endl;								\
  }										\
  while (false)

#else

#define lineage_log_status(...)
#define lineage_log_warning(...)
#define lineage_log_error(...)

#endif /* !defined(LINEAGE_NOLOG) */

/* -- Assert Macros -- */

#if !defined(LINEAGE_NOASSERT)

/** Performs an assertion, and fails the application if the assertion does not pass. */
#define lineage_assert(cond)							\
  do										\
  {										\
    if ((cond))									\
      break;									\
    lineage::debug::error_stream()						\
      << lineage::debug::trace_message("lineage_assert FAILED",			\
                                       __PRETTY_FUNCTION__,            		\
                                       __FILE__,                        	\
                                       __LINE__,                    		\
                                       { #cond })                       	\
      << std::endl;								\
    lineage::debug::fail();							\
  }										\
  while (false)

/** Unconditionally fails an assert. */
#define lineage_assert_fail(message)						\
  do										\
  {										\
    lineage::debug::error_stream()						\
      << lineage::debug::trace_message("lineage_assert FAILED",			\
                                       __PRETTY_FUNCTION__,             	\
                                       __FILE__,                        	\
                                       __LINE__,                    		\
                                       { message })                     	\
      << std::endl;								\
    lineage::debug::fail();							\
  }										\
  while (false)

#else

#define lineage_assert(cond)
#define lineage_assert_fail(message)

#endif

/* -- Types -- */

namespace lineage
{

  /**
   * Class containing lineageity functions for debugging purposes.
   */
  class debug
  {
  public:

    /** Immediately crashes the application. */
    static void fail() __attribute__((noreturn));

    /** Triggers a platform-specific breakpoint. */
    static void breakpoint();

    /** The output stream to use for debug printing. */
    static std::ostream& debug_stream()
    { return std::cout; }

    /** The output stream to use for error printing. */
    static std::ostream& error_stream()
    { return std::cerr; }

    /** Returns a trace message string with the specified parameters. */
    static std::string trace_message(const std::string& title,
				     const std::string& function,
				     const std::string& file,
				     int lineagee,
				     const std::vector<std::string>& messages);

  private:

    debug() = delete;
    debug(const debug&) = delete;
    debug& operator =(const debug&) = delete;

  };

}
