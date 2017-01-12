#
# CMakeLists.txt
# Chris Vig (chris@invictus.so)
#

# -- Project Configuration --

cmake_minimum_required(VERSION 2.8.7)
project(lineage)

# Targets
set(MAIN_TARGET ${CMAKE_PROJECT_NAME})

# Directories
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR})

# Toolchain configuration
set(CMAKE_CXX_FLAGS "-std=gnu++14 -Wall -Wpedantic")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-Werror -O2 -s")

# -- Main Executable --

# Build main executable
add_executable(${MAIN_TARGET}
  ${SOURCE_DIR}/debug.cpp
  ${SOURCE_DIR}/main.cpp)
target_compile_definitions(${MAIN_TARGET}
  PRIVATE -DLINEAGE_DEBUG)
target_include_directories(${MAIN_TARGET}
  PRIVATE ${SOURCE_DIR})

# Run main executable
add_custom_target(run
  COMMAND ${MAIN_TARGET}
  DEPENDS ${MAIN_TARGET}
  WORKING_DIRECTORY ${BUILD_DIR}
  COMMENT "Running ${CMAKE_PROJECT_NAME}...")
