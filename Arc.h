#pragma once

// My includes
#include "Diagram.h"

/**
 * \brief Namespace of MyGAL
 */
namespace mygal
{

template<typename T>
class Event;

template<typename T>
struct Arc
{
    enum class Color{Red, Black};
    enum class Side{Left, Right};

    // Hierarchy
    Arc<T>* parent;
    Arc<T>* left;
    Arc<T>* right;
    // Diagram
    typename Diagram<T>::Site* site;
    typename Diagram<T>::HalfEdge* leftHalfEdge;
    typename Diagram<T>::HalfEdge* rightHalfEdge;
    Event<T>* event;
    // Optimizations
    Arc<T>* prev;
    Arc<T>* next;
    // Only for balancing
    Color color;
    // To know if the arc is towards -inf or +inf
    Side side;
};

}
