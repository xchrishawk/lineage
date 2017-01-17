#
# CMakeLists.txt
# Chris Vig (chris@invictus.so)
#

# -- Project Configuration --

# CMake configuration
cmake_minimum_required(VERSION 2.8.7)
find_package(PkgConfig)

# Project name
project(lineage)

# Targets
set(MAIN_TARGET ${CMAKE_PROJECT_NAME})

# Toolchain configuration
set(CMAKE_CXX_FLAGS "-std=gnu++14 -Wall -Wpedantic")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-Werror -O2 -s")

# -- Inputs --

# Directories
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(SHADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/shader)
set(BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR})

# Source files
set(MAIN_TARGET_SOURCES
  ${SOURCE_DIR}/application.cpp
  ${SOURCE_DIR}/buffer.cpp
  ${SOURCE_DIR}/debug.cpp
  ${SOURCE_DIR}/default_render_manager.cpp
  ${SOURCE_DIR}/default_state_manager.cpp
  ${SOURCE_DIR}/input_manager.cpp
  ${SOURCE_DIR}/main.cpp
  ${SOURCE_DIR}/opengl.cpp
  ${SOURCE_DIR}/opengl_error.cpp
  ${SOURCE_DIR}/prototype_render_manager.cpp
  ${SOURCE_DIR}/prototype_state_manager.cpp
  ${SOURCE_DIR}/shader.cpp
  ${SOURCE_DIR}/shader_program.cpp
  ${SOURCE_DIR}/shader_source.cpp
  ${SOURCE_DIR}/util.cpp
  ${SOURCE_DIR}/vertex_array.cpp
  ${SOURCE_DIR}/window.cpp)

# Shader files
set(MAIN_TARGET_SHADERS
  ${SHADER_DIR}/default_fragment_shader.glsl
  ${SHADER_DIR}/default_vertex_shader.glsl
  ${SHADER_DIR}/prototype_fragment_shader.glsl
  ${SHADER_DIR}/prototype_vertex_shader.glsl)

# Allow relative includes for source files
list(APPEND MAIN_TARGET_INCLUDE_DIRECTORIES ${SOURCE_DIR})

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

  # Must manually include /usr/local/include since compiler doesn't include it automatically
  # for some baffling reason
  list(APPEND MAIN_TARGET_INCLUDE_DIRECTORIES /usr/local/include)

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

# Requires...
# - GLEW (OpenGL Extension Wrangler)
# - GLFW3 (OpenGL window library)
set(PKG_CONFIG_LIBRARIES glew glfw3)

# Configure each required library with pkg-config
foreach(LIBNAME ${PKG_CONFIG_LIBRARIES})
  pkg_check_modules(${LIBNAME} REQUIRED ${LIBNAME})
  list(APPEND MAIN_TARGET_COMPILE_OPTIONS ${${LIBNAME}_CFLAGS_OTHER})
  list(APPEND MAIN_TARGET_INCLUDE_DIRECTORIES ${${LIBNAME}_INCLUDE_DIRS})
  list(APPEND MAIN_TARGET_LINK_LIBRARIES ${${LIBNAME}_LIBRARIES})
  link_directories(${${LIBNAME}_LIBRARY_DIRS})
endforeach(LIBNAME)

# -- Shader Processing --

# Create directory for shader include files
set(PROCESSED_SHADER_DIR ${CMAKE_BINARY_DIR}/shader)
file(MAKE_DIRECTORY ${PROCESSED_SHADER_DIR})
list(APPEND MAIN_TARGET_INCLUDE_DIRECTORIES ${PROCESSED_SHADER_DIR})

# Process each shader
foreach(SHADER ${MAIN_TARGET_SHADERS})
  get_filename_component(SHADER_FILE_NAME ${SHADER} NAME)
  set(SHADER_OUTPUT_PATH "${PROCESSED_SHADER_DIR}/${SHADER_FILE_NAME}.inc")
  list(APPEND MAIN_TARGET_PROCESSED_SHADERS ${SHADER_OUTPUT_PATH})
  add_custom_command(
    DEPENDS ${SHADER}
    OUTPUT ${SHADER_OUTPUT_PATH}
    COMMAND cat ${SHADER} | xxd -i > ${SHADER_OUTPUT_PATH}
    COMMENT "Processing ${SHADER}")
endforeach(SHADER)

# -- Main Executable --

# Build main executable
add_executable(${MAIN_TARGET}
  ${MAIN_TARGET_SOURCES}
  ${MAIN_TARGET_PROCESSED_SHADERS})
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
