
#include "Fdiagrams.h"

#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <iostream>

#include "Particle.h"
#include "Vertex.h"
#include "Interaction.h"
#include "Endpoint.h"
#include "Fdiagram.h"

namespace fdiagram_project {

//Fdiagrams private member functions:
std::vector<std::unique_ptr<fdiagram>> fdiagrams::construct_blank_diagrams(const size_t& order)
{
	//Recursively constructs all possible binary trees of given order, these are appended to "diagrams".
	std::vector<std::unique_ptr<fdiagram>> subdiagrams;

	if (order > 10) {
		std::cout << "Diagram order too high!" << std::endl;
	}
	else if (order == 0) {
		subdiagrams.push_back(nullptr);
		return subdiagrams;
	}
	for (size_t i{}; i <= order - 1; i++) {
		for (auto& diagram_l : construct_blank_diagrams(i)) {
			for (auto& diagram_r : construct_blank_diagrams(order - i - 1)) {
				//Create a primitive diagram and append all permutations of subdiagrams:
				std::unique_ptr<fdiagram> primitive_fdiagram{ std::make_unique<fdiagram>(1) };
				if (diagram_l != nullptr) {
					std::unique_ptr<fdiagram> diagram_l_copy{ std::make_unique<fdiagram>(*diagram_l) };
					primitive_fdiagram->append(true, diagram_l_copy);
				}
				if (diagram_r != nullptr) {
					primitive_fdiagram->append(false, diagram_r);
				}
				subdiagrams.push_back(std::move(primitive_fdiagram));
			}
		}
	}
	return subdiagrams;
}
void fdiagrams::assign_keys()
{
	//Assigns keys to all member diagrams:
	for (auto& diagram : diagrams) {
		diagram->assign_keys();
	}
}
void fdiagrams::assign_particle_permutations(std::vector<particle>& particles)
{
	//Assigns all endpoint particle permutations to the diagrams and appends to "diagrams_with_endpoints".
	//The particle in position 1 is kept constant, "check_endpoints" is called to apply constraints to the permutations.
	std::sort(particles.begin() + 1, particles.end(),
		[](const particle& a, const particle& b) { return a.get_index() < b.get_index(); });
	for (auto& diagram : diagrams) {
		do {
			if (check_endpoints(particles)) {
				std::unique_ptr<fdiagram> diagram_copy{ std::make_unique<fdiagram>(*diagram) };
				diagram_copy->assign_endpoint_particles(particles);
				diagrams_with_endpoints.push_back(std::move(diagram_copy));
			}
		} while (std::next_permutation(particles.begin() + 1, particles.end(),
			[](const particle& a, const particle& b) { return a.get_index() < b.get_index(); }));
	}
}
void fdiagrams::assign_interactions()
{
	for (auto& diagram : diagrams_with_endpoints) {
		for (auto& complete_diagram : diagram->assign_interactions(diagram)) {
			complete_diagrams.push_back(std::move(complete_diagram));
		}
	}
};
bool fdiagrams::check_endpoints(const std::vector<particle>& particles)
{
	//Enforces certain constraints on the allowed endpoint particle permutations.
	//There are no constraints if the root particle is a boson.
	//If the root particle is a fermion we enforce its spinor line to be on the outer edge.
	size_t n{};

	if (particles[0].is_outgoing) return false;
	if (!particles[0].is_fermion) return true;
	if (particles[0].is_matter) {
		n = 1;
		if (!particles[n].is_fermion) return false;
	}
	else {
		n = particles.size() - 1;
		if (!particles[n].is_fermion) return false;
	}
	if ((particles[0].is_matter == particles[n].is_outgoing) == particles[n].is_matter) {
		//Quark spinor line:
		if (particles[0].is_quark && particles[n].is_quark) return true;
		//Lepton spinor line:
		else if (particles[0].flavour == particles[n].flavour) return true;
	}
	return false;
}

//Fdiagrams public member functions:
fdiagrams::fdiagrams(const size_t& order_in) : order{ order_in }
{
	diagrams = construct_blank_diagrams(order);
	assign_keys();
}
fdiagrams::fdiagrams(const size_t& order_in, std::vector<particle>& particles_in) : order{ order_in }
{
	diagrams = construct_blank_diagrams(order);
	assign_keys();
	assign_particle_permutations(particles_in);
	assign_interactions();
}
void fdiagrams::print()
{
	//Prints all member diagrams:
	for (auto& diagram : complete_diagrams) {
		diagram->print();
	}
}
} //namespace fdiagram_project