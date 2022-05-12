#pragma once

#ifndef FDIAGRAM_H
#define FDIAGRAM_H

#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <iostream>

#include "Particle.h"
#include "Vertex.h"
#include "Interaction.h"
#include "Endpoint.h"

namespace fdiagram_project{

class fdiagram
{
	//Contains the root vertex for the binary tree structure.
	//The methods build up a feynman diagram in multiple stages. // ADD MORE/CHANGE HERE!!!!!!!!!!
private:
	size_t order;
	std::unique_ptr<vertex> root;
	bool is_complete;

	//Recursive helper functions:
	void construct_primitive_diagram(const std::unique_ptr<vertex>&);
	void destroy_diagram(std::unique_ptr<vertex>&);

	std::pair<size_t, size_t> key_assigner(std::unique_ptr<vertex>&, std::pair<size_t, size_t>);
	std::vector<particle>::iterator particle_assigner(std::unique_ptr<vertex>&,
		std::vector<particle>::iterator);

	std::vector<std::unique_ptr<fdiagram>> interaction_assigner(std::unique_ptr<vertex>&);

	//Other recursive functions:
	std::unique_ptr<vertex> deep_copy(const std::unique_ptr<vertex>&);
	void print(const std::string&, const std::unique_ptr<vertex>&, bool) const;

public:
	//Constructors, destructors:
	fdiagram() : order{ 0 }, is_complete{ false } { root = nullptr; }
	fdiagram(const size_t&);
	fdiagram(const std::unique_ptr<vertex>& root_in) : root{ deep_copy(root_in) } {}
	~fdiagram() { destroy_diagram(root); }

	//Copy constructor:
	fdiagram(const fdiagram&);

	//Assignment functions:
	void assign_keys();
	void assign_endpoint_particles(std::vector<particle>);
	std::vector<std::unique_ptr<fdiagram>> assign_interactions(std::unique_ptr<fdiagram>&);


	//Other member functions:
	void append(const bool, const std::unique_ptr<fdiagram>&);
	void append_replace(const bool, const std::unique_ptr<fdiagram>&);
	void make_complete() { is_complete = true; }
	bool get_is_complete() { return is_complete; }
	void print() { print("", root, false); }
};

} //namespace fdiagram_project

#endif