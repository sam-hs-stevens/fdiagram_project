#pragma once

#ifndef FDIAGRAMS_H
#define FDIAGRAMS_H

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

class fdiagrams
{
	//Contains vectors of fdiagrams, and the methods required to construct them.
private:
	size_t order;
	std::vector<std::unique_ptr<fdiagram>> diagrams;
	std::vector<std::unique_ptr<fdiagram>> diagrams_with_endpoints;
	std::vector<std::unique_ptr<fdiagram>> complete_diagrams;

	//Diagram construction methods:
	std::vector<std::unique_ptr<fdiagram>> construct_blank_diagrams(const size_t&);
	void assign_keys();
	void assign_particle_permutations(std::vector<particle>&);
	void assign_interactions();

	bool check_endpoints(const std::vector<particle>&);
public:
	//Constructors, destructor
	fdiagrams() : order{}, diagrams{}, diagrams_with_endpoints{} {};
	~fdiagrams() { diagrams.clear(); diagrams_with_endpoints.clear(); complete_diagrams.clear(); }
	//Constructor including key assignment:
	fdiagrams(const size_t&);
	//Constructor to produce complete diagrams:
	fdiagrams(const size_t&, std::vector<particle>&);

	size_t size()
	{
		return complete_diagrams.size();
	}
	void print();
};
} //namespace fdiagram_project
#endif