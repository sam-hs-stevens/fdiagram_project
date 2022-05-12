
#include "Endpoint.h"

#include <string>
#include <vector>
#include <memory>

#include "Particle.h"
#include "Vertex.h"

namespace fdiagram_project {

//Endpoint public member functions
std::string endpoint::info() const
{
	//Called on diagram output.
	if (end_particle->is_outgoing == true) return end_particle->info() + ">";
	return end_particle->info() + "<";
}
} //namespace fdiagram_project