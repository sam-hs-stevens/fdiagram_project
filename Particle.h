#pragma once

#ifndef PARTICLE_H
#define PARTICLE_H

#include <string>
#include <vector>
#include <memory>

namespace fdiagram_project {

class particle
{
public:
	std::string flavour;
	bool is_matter;
	bool is_outgoing;
	bool is_fermion;
	bool is_lepton;
	bool is_neutrino; //Is_neutrino is currently unused, but would be in future versions.
	bool is_quark;
	size_t index; //Index is used for various processes, index 0 corresponds to an unassigned particle.

	//Constructors, destructor:
	particle();
	particle(std::string, bool, bool, bool, bool, bool, bool, size_t);
	~particle() {}

	//Move constructor:
	particle(particle&&) noexcept(false);
	//Copy constructor:
	particle(const particle&);
	//Move assignment operator:
	particle& operator=(particle&&) noexcept(false);
	//Copy assignment operator:
	particle& operator=(const particle&);

	//Other member functions:
	void switch_direction();
	bool set_flavour(std::string flavour_in);
	size_t get_index() const { return index; }
	std::string info() const;

	//Operator overloads:
	friend bool operator==(particle&, particle&);
	friend bool operator!=(particle&, particle&);

	static particle electron;
	static particle muon;
	static particle tau;
	static particle up;
	static particle down;
	static particle charm;
	static particle strange;
	static particle photon;
	static particle gluon;
	static std::vector<particle> generic_particles;
	static std::vector<std::vector<std::string>> allowed_flavour_combinations;
	static std::vector<particle> flavourless_particles;
};

//Static particle variables:
static particle electron{ "e", true, true, true, true, false, false , 2 };
static particle muon{ "m", true, true, true, true, false, false , 3 };
static particle tau{ "t", true, true, true, true, false, false, 4 };
static particle up{ "u", true, true, true, false, false, true, 5 };
static particle down{ "d", true, true, true, false, false, true, 6 };
static particle charm{ "c", true, true, true, false, false, true, 7 };
static particle strange{ "s", true, true, true, false, false, true, 8 };
static particle photon{ "y", true, true, false, false, false, false, 9 };
static particle gluon{ "g", true, true, false, false, false, false, 10 };
static std::vector<particle> generic_particles{ electron, muon, tau, up, down, charm, strange, photon, gluon };
static std::vector<std::vector<std::string>> allowed_flavour_combinations{
	{"y", "e", "e"}, {"y", "t", "t"}, {"y", "m", "m"}, {"y", "u", "u"}, {"y", "d", "d"},
	{"y", "s", "s"}, {"y", "c", "c"}, {"g", "u", "u"}, {"g", "d", "d"}, {"g", "c", "c"},
	{"g", "s", "s"}, {"g", "g", "g"}
};
static std::vector<particle> flavourless_particles{
	//Used to find allowed combinations of is_matter and is_outgoing.
	//Fermions:
	particle{ "", true, true, true, true, false, false, 1},
	particle{ "", false, true, true, true, false, false, 1},
	particle{ "", true, false, true, true, false, false, 1},
	particle{ "", false, false, true, true, false, false, 1},
	//Bosons:
	particle{ "", true, true, false, false, false, false, 1},
	particle{ "", false, true, false, false, false, false, 1},
};

//Particle non-member functions:
bool operator==(particle&, particle&);
bool operator!=(particle&, particle&);
particle instantiate_particle(std::string);
} //namespace fdiagram_project
#endif