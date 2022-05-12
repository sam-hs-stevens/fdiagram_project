#pragma once

#ifndef ENDPOINT_H
#define ENDPOINT_H

#include <string>
#include <vector>
#include <memory>

#include "Particle.h"
#include "Vertex.h"

namespace fdiagram_project{

class endpoint : public vertex
{
	//Used for leaf nodes (diagram endpoints).
protected:
	std::unique_ptr<particle> end_particle;
public:
	//Constructors, destructor:
	endpoint() : vertex(), end_particle{ std::make_unique<particle>() } {}
	endpoint(vertex&& blank_vertex) : vertex(blank_vertex), end_particle{ std::make_unique<particle>() } {}
	endpoint(vertex&& blank_vertex, particle&& particle_in) : vertex(blank_vertex), end_particle{ std::make_unique<particle>(particle_in) } { }
	~endpoint() override {}

	//Quasi-copy constructor and virtual clone method (does not copy left & right):
	endpoint(const endpoint& source_endpoint) : vertex(source_endpoint.key), end_particle{ std::make_unique<particle>(*source_endpoint.end_particle) } {}
	std::unique_ptr<vertex> clone() const override { return std::make_unique<endpoint>(*this); }

	//Other member functions:
	void assign_particle(const particle& source_particle) override { end_particle = std::make_unique<particle>(source_particle); }
	particle get_particle_0() const override { return *end_particle; }
	std::string info() const override;
};
} // namespace fdiagram_project

#endif