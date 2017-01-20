/**
 * @file	mesh.hpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/19
 */

#pragma once

/* -- Includes -- */

#include <type_traits>
#include <utility>
#include <vector>

#include "api.hpp"
#include "buffer.hpp"
#include "vertex.hpp"

/* -- Types -- */

namespace lineage
{
  namespace templates
  {

    /**
     * Class representing a renderable mesh.
     */
    template <typename TVertex, typename TIndex>
    class basic_mesh
    {

      /* -- Types -- */

    public:

      /** The type of vertex that this mesh uses. */
      using vertex_type = TVertex;

      /** The type of index that this mesh uses. */
      using index_type = TIndex;

      /* -- Lifecycle -- */

    public:

      /**
       * Constructs a new mesh with the specified parameters.
       */
      basic_mesh(GLenum draw_mode,
                 const std::vector<TVertex>& vertices,
                 const std::vector<TIndex>& indices)
        : m_draw_mode(draw_mode),
          m_vertex_buffer(vertices, 0),
          m_vertex_count(vertices.size()),
          m_index_buffer(indices, 0),
          m_index_count(indices.size())
      { }

      /**
       * Destructor.
       */
      ~basic_mesh() = default;

    private:

      basic_mesh(const lineage::templates::basic_mesh<TVertex, TIndex>&) = delete;
      basic_mesh(lineage::templates::basic_mesh<TVertex, TIndex>&&) = delete;
      lineage::templates::basic_mesh<TVertex, TIndex>& operator =(const lineage::templates::basic_mesh<TVertex, TIndex>&) = delete;
      lineage::templates::basic_mesh<TVertex, TIndex>& operator =(lineage::templates::basic_mesh<TVertex, TIndex>&&) = delete;

      /* -- Public Methods -- */

    public:

      /**
       * The OpenGL draw mode for this mesh.
       */
      GLenum draw_mode() const
      {
        return m_draw_mode;
      }

      /**
       * The buffer containing the vertex data for this mesh.
       */
      const lineage::buffer& vertex_buffer() const
      {
        return m_vertex_buffer;
      }

      /**
       * The number of vertices in the vertex buffer.
       */
      size_t vertex_count() const
      {
        return m_vertex_count;
      }

      /**
       * The size of each vertex, in bytes.
       */
      size_t vertex_size() const
      {
        return sizeof(vertex_type);
      }

      /**
       * The buffer containing the index data for this mesh.
       */
      const lineage::buffer& index_buffer() const
      {
        return m_index_buffer;
      }

      /**
       * The number of indices in the index buffer.
       */
      size_t index_count() const
      {
        return m_index_count;
      }

      /**
       * The OpenGL data type enum for the indices in the index buffer.
       */
      template <typename T = TIndex>
      GLenum index_datatype(typename std::enable_if<std::is_same<T, GLubyte>::value, void*>::type _ = nullptr) const
      {
        return GL_UNSIGNED_BYTE;
      }

      /**
       * The OpenGL data type enum for the indices in the index buffer.
       */
      template <typename T = TIndex>
      GLenum index_datatype(typename std::enable_if<std::is_same<T, GLushort>::value, void*>::type _ = nullptr) const
      {
        return GL_UNSIGNED_SHORT;
      }

      /**
       * The OpenGL data type enum for the indices in the index buffer.
       */
      template <typename T = TIndex>
      GLenum index_datatype(typename std::enable_if<std::is_same<T, GLuint>::value, void*>::type _ = nullptr) const
      {
        return GL_UNSIGNED_INT;
      }

      /* -- Implementation -- */

    private:

      const GLenum m_draw_mode;
      const lineage::immutable_buffer m_vertex_buffer;
      const size_t m_vertex_count;
      const lineage::immutable_buffer m_index_buffer;
      const size_t m_index_count;

    };

  }

  /**
   * Standard mesh type used by the application.
   */
  using mesh = lineage::templates::basic_mesh<lineage::vertex, GLuint>;

}
