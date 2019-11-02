#pragma once

// STL
#include <vector>

/**
 * \brief Namespace of MyGAL
 */
namespace mygal
{

/**
 * \brief Data structure representing a triangulation
 *
 * \author Pierre Vigier
 */
class Triangulation
{
public:
    /**
     * \brief Constructor of Triangulation
     *
     * \param neighbors Neighbors for each vertex of the triangulation
     */
    explicit Triangulation(std::vector<std::vector<std::size_t>> neighbors) : mNeighbors(std::move(neighbors))
    {

    }

    /**
     * \brief Get the number of vertices
     *
     * \return The number of vertices
     */
    std::size_t getNbVertices() const
    {
        return mNeighbors.size();
    }

    /**
     * \brief Get the neighbors of a vertex
     *
     * \param i Index of the vertex
     *
     * \return The neighbors of vertex `i`
     */
    const std::vector<std::size_t>& getNeighbors(std::size_t i) const
    {
        return mNeighbors[i];
    }

private:
    std::vector<std::vector<std::size_t>> mNeighbors;
};

}
