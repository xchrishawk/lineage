/**
 * @file	debug.cpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/11
 */

/* -- Includes -- */

#include <csignal>
#include <cstdlib>
#include <iostream>
#include <sstream>

#include "debug.hpp"

/* -- Namespaces -- */

using namespace lineage;

/* -- Procedures -- */

void debug::fail()
{
  std::abort();
}

void debug::breakpoint()
{
  std::raise(SIGINT);
}

std::string debug::trace_message(const std::string& title,
				 const std::string& function,
				 const std::string& file,
				 int line,
				 const std::vector<std::string>& messages)
{
  std::ostringstream ret;
  ret << "* " << title << ": " << function << " (" << file << ":" << line << ")" << std::endl;
  for (const auto& message : messages)
    ret << "  " << message << std::endl;
  return ret.str();
}
