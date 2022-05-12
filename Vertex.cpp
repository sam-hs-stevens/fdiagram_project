
#include "Vertex.h"

#include <string>
#include <vector>
#include <memory>

#include "Particle.h"

namespace fdiagram_project {

//Vertex public member functions:
vertex::vertex(vertex&& vertex_in) noexcept(false)
{
	key = vertex_in.key;
	left = std::move(vertex_in.left);
	right = std::move(vertex_in.right);
	vertex_in.key = 0;
}
} //namespace fdiagram_project