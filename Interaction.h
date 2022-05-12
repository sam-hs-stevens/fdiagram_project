#pragma once

#ifndef INTERACTION_H
#define INTERACTION_H

#include <string>
#include <vector>
#include <memory>
#include <algorithm>

#include "Particle.h"
#include "Vertex.h"

namespace fdiagram_project {

class interaction : public vertex
{
	//Used for branch nodes of the diagram (particle interactions).
protected:
	std::vector<std::unique_ptr<particle>> particles;
public:
	//Default constructor:
	interaction();
	~interaction() override {}

	//Vertex move constructor:
	interaction(vertex&&);
	//Quasi-copy constructor and virtual clone method (does not copy left & right):
	interaction(const interaction&);
	std::unique_ptr<vertex> clone() const override { return std::make_unique<interaction>(*this); }

	//Other member functions:
	void assign_particle(const particle& source_particle) override;
	void assign_particles(const std::vector<particle>& source_particles) override;
	particle get_particle_0() const override;
	void get_next_particles();
	std::string info() const override;

	std::vector<std::vector<particle>> checking_procedure() override;
};
} //namespace fdiagram_project

#endif