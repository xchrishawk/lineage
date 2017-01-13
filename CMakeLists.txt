#
# CMakeLists.txt
# Chris Vig (chris@invictus.so)
#

# -- Project Configuration --

cmake_minimum_required(VERSION 2.8.7)
project(lineage)

# CMake packages
find_package(PkgConfig)

# Targets
set(MAIN_TARGET ${CMAKE_PROJECT_NAME})

# Directories
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR})

# Include directory
list(APPEND MAIN_TARGET_INCLUDE_DIRECTORIES ${SOURCE_DIR})

# Source files
set(MAIN_TARGET_SOURCES
  ${SOURCE_DIR}/application.cpp
  ${SOURCE_DIR}/debug.cpp
  ${SOURCE_DIR}/input_manager.cpp
  ${SOURCE_DIR}/main.cpp
  ${SOURCE_DIR}/opengl.cpp
  ${SOURCE_DIR}/prototype_render_manager.cpp
  ${SOURCE_DIR}/prototype_state_manager.cpp
  ${SOURCE_DIR}/window.cpp)

# Toolchain configuration
set(CMAKE_CXX_FLAGS "-std=gnu++14 -Wall -Wpedantic")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-Werror -O2 -s")

# -- Platform Configuration --

if(UNIX AND NOT APPLE)

  # Preprocessor macros
  list(APPEND MAIN_TARGET_COMPILE_DEFINITIONS -DLINEAGE_LINUX)

elseif(APPLE)

  # Preprocessor macros
  list(APPEND MAIN_TARGET_COMPILE_DEFINITIONS -DLINEAGE_MACOS)

  # Must link against OpenGL.framework
  find_package(OpenGL REQUIRED)
  list(APPEND MAIN_TARGET_LINK_LIBRARIES ${OPENGL_LIBRARIES})

else()

  # Unknown platform?
  message(FATAL_ERROR "Unsupported or unknown platform!")

endif()

# -- Build Type Configuration --

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")

  # Preprocessor macros
  list(APPEND MAIN_TARGET_COMPILE_DEFINITIONS -DLINEAGE_DEBUG)

elseif(${CMAKE_BUILD_TYPE} STREQUAL "Release")

  # Preprocessor macros
  list(APPEND MAIN_TARGET_COMPILE_DEFINITIONS
    -DLINEAGE_NOASSERT
    -DLINEAGE_NOLOG
    -DLINEAGE_RELEASE)

else()

  # Unknown build type?
  message(FATAL_ERROR "Unknown build type!")

endif()

# -- Third Party Libraries --

# GLEW
pkg_check_modules(GLEW REQUIRED glew)
list(APPEND MAIN_TARGET_COMPILE_OPTIONS ${GLEW_CFLAGS_OTHER})
list(APPEND MAIN_TARGET_INCLUDE_DIRECTORIES ${GLEW_INCLUDE_DIRS})
list(APPEND MAIN_TARGET_LINK_LIBRARIES ${GLEW_LIBRARIES})
link_directories(${GLEW_LIBRARY_DIRS})

# GLFW3
pkg_check_modules(GLFW REQUIRED glfw3)
list(APPEND MAIN_TARGET_COMPILE_OPTIONS ${GLFW_CFLAGS_OTHER})
list(APPEND MAIN_TARGET_INCLUDE_DIRECTORIES ${GLFW_INCLUDE_DIRS})
list(APPEND MAIN_TARGET_LINK_LIBRARIES ${GLFW_LIBRARIES})
link_directories(${GLFW_LIBRARY_DIRS})

# -- Main Executable --

# Build main executable
add_executable(${MAIN_TARGET} ${MAIN_TARGET_SOURCES})
target_compile_definitions(${MAIN_TARGET} PRIVATE ${MAIN_TARGET_COMPILE_DEFINITIONS})
target_compile_options(${MAIN_TARGET} PRIVATE ${MAIN_TARGET_COMPILE_OPTIONS})
target_include_directories(${MAIN_TARGET} PRIVATE ${MAIN_TARGET_INCLUDE_DIRECTORIES})
target_link_libraries(${MAIN_TARGET} ${MAIN_TARGET_LINK_LIBRARIES})

# Run main executable
add_custom_target(run
  COMMAND ${MAIN_TARGET}
  DEPENDS ${MAIN_TARGET}
  WORKING_DIRECTORY ${BUILD_DIR}
  COMMENT "Running ${CMAKE_PROJECT_NAME}...")
