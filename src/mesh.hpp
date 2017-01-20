/**
 * @file	mesh.hpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/19
 */

#pragma once

/* -- Includes -- */

#include <vector>

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
    template <typename TVertex>
    class basic_mesh
    {

      /* -- Types -- */

    public:

      /** The type of vertex that this mesh uses. */
      using vertex_type = TVertex;

      /* -- Lifecycle -- */

    public:

      /**
       * Constructs a new mesh with the specified parameters.
       */
      basic_mesh(GLenum type, const std::vector<TVertex>& vertices)
        : m_type(type),
          m_vertex_buffer(vertices, 0),
          m_vertex_count(vertices.size())
      { }

      /**
       * Destructor.
       */
      ~basic_mesh() = default;

    private:

      basic_mesh(const lineage::templates::basic_mesh<TVertex>&) = delete;
      basic_mesh(lineage::templates::basic_mesh<TVertex>&&) = delete;
      lineage::templates::basic_mesh<TVertex>& operator =(const lineage::templates::basic_mesh<TVertex>&) = delete;
      lineage::templates::basic_mesh<TVertex>& operator =(lineage::templates::basic_mesh<TVertex>&&) = delete;

      /* -- Public Methods -- */

    public:

      /**
       * The type of mesh being drawn.
       */
      GLenum type() const
      {
        return m_type;
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

      /* -- Implementation -- */

    private:

      const GLenum m_type;
      const lineage::immutable_buffer m_vertex_buffer;
      const size_t m_vertex_count;

    };

  }

  /**
   * Standard mesh type used by the application.
   */
  using mesh = lineage::templates::basic_mesh<lineage::vertex>;

}
