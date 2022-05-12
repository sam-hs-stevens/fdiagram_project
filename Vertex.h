#pragma once

#ifndef VERTEX_H
#define VERTEX_H

#include <string>
#include <vector>
#include <memory>

#include "Particle.h"

namespace fdiagram_project {

class vertex
{
	//Vertex base class, used as nodes in a binary tree structure.
	friend class fdiagram;
protected:
	size_t key;
	//Pointers to child vertices:
	std::unique_ptr<vertex> left;
	std::unique_ptr<vertex> right;
public:
	//Constructors, virtual destructor:
	vertex() { key = 0; left = nullptr; right = nullptr; }
	vertex(size_t key_in) : key{ key_in } {}
	virtual ~vertex() { key = 0; left.reset(); right.reset(); }

	//Move constructor:
	vertex(vertex&&) noexcept(false);

	//Quasi-Copy constructor and virtual clone method (does not copy left & right):
	vertex(const vertex& source_vertex) : key{ source_vertex.key } {}
	virtual std::unique_ptr<vertex> clone() const { return std::make_unique<vertex>(*this); }

	//Other virtual member functions:
	virtual void assign_particle(const particle&) {}
	virtual void assign_particles(const std::vector<particle>& source_particles) {}
	virtual particle get_particle_0() const { return particle{}; }
	virtual std::string info() const { return std::to_string(key); }
	virtual std::vector<std::vector<particle>> checking_procedure()
	{
		std::vector<std::vector<particle>> blank;
		return blank;
	}
};
} //namespace fdiagram_project
#endif