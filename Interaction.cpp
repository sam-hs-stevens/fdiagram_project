
#include "Interaction.h"

#include <string>
#include <vector>
#include <memory>
#include <algorithm>

#include "Particle.h"
#include "Vertex.h"

namespace fdiagram_project {

//Interaction non-member functions:
bool checker_helper(const std::vector<particle>& test_particles)
{
	//Applies constraints on the properties is_matter, is_outgoing and is_fermion. Returns true if constraints are satisfied.
	//Note that we choose spinor lines to go to the left and anti-spinor lines to the right.
	size_t n{};
	if (test_particles[0].is_fermion == true) {
		//Check that the interaction has two fermions and a boson:
		if (test_particles[1].is_fermion == true && test_particles[2].is_fermion == true) {
			return false;
		}
		//Here n denotes the position of the other fermion:
		if (test_particles[1].is_fermion == true) {
			n = 1;
		}
		else {
			n = 2;
		}
		//Lepton/baryon number conservation:
		if (test_particles[0].is_outgoing == test_particles[n].is_outgoing) {
			if (test_particles[0].is_matter != test_particles[n].is_matter) {
				return true;
			}
			else return false;
		}
		else if (test_particles[0].is_matter == test_particles[n].is_matter) {
			return true;
		}
		else return false;
	}
	else {
		//Check that the interaction has two fermions and a boson:
		if (test_particles[1].is_fermion == false || test_particles[2].is_fermion == false) {
			//Interactions between three bosons have no constraints:
			if (test_particles[1].is_fermion == false && test_particles[2].is_fermion == false) {
				return true;
			}
			return false;
		}
		//Lepton and Baryon number conservation (here particles 1 and 2 are fermions):
		if (test_particles[1].is_outgoing == test_particles[2].is_outgoing)
		{
			if (test_particles[1].is_matter != test_particles[2].is_matter) {
				return true;
			}
			else return false;
		}
		else if (test_particles[1].is_matter == test_particles[2].is_matter) {
			return true;
		}
		else return false;
	}
}
bool flavour_checker_3(const std::string& flavour_0, const std::string& flavour_1, const std::string& flavour_2)
{
	//Sorts through permutations of allowed flavour combinations to see if there is a match, in which case true is returned.
	//3 known flavours, so the result of this check is either success or failure.
	std::vector<std::string> flavours{ flavour_0, flavour_1, flavour_2 };
	for (auto& test_flavours : allowed_flavour_combinations) {
		std::sort(test_flavours.begin(), test_flavours.end());
		do {
			if (flavours == test_flavours) {
				return true;
			}
		} while (std::next_permutation(test_flavours.begin(), test_flavours.end()));
	}
	return false;
}
std::vector<std::string> flavour_checker_2(const std::string& flavour_0, const std::string& flavour_1)
{
	//Sorts through permutations of allowed flavour combinations to see if there is a match.
	//2 known flavours, so there are multiple possibilities for the third particle,
	//a vector of possible flavours to assign is returned. A blank vector is returned if there is no match.
	std::vector<std::string> flavours{ flavour_0, flavour_1, "" };
	std::vector<std::string> fit_flavour{};
	for (auto& test_flavours : allowed_flavour_combinations) {
		std::sort(test_flavours.begin(), test_flavours.end());
		do {
			if ((flavours[0] == test_flavours[0]) && flavours[1] == test_flavours[1]) {
				fit_flavour.push_back(test_flavours[2]);
			}
		} while (std::next_permutation(test_flavours.begin(), test_flavours.end()));
	}
	return fit_flavour;
}


//Interaction public member functions:
interaction::interaction() : vertex()
{
	particles.push_back(std::make_unique<particle>());
	particles.push_back(std::make_unique<particle>());
	particles.push_back(std::make_unique<particle>());
}
interaction::interaction(vertex&& blank_vertex) : vertex(std::move(blank_vertex))
{
	particles.push_back(std::make_unique<particle>());
	particles.push_back(std::make_unique<particle>());
	particles.push_back(std::make_unique<particle>());
}
interaction::interaction(const interaction& source_interaction) : vertex(source_interaction.key)
{
	particles.push_back(std::make_unique<particle>(*source_interaction.particles[0]));
	particles.push_back(std::make_unique<particle>(*source_interaction.particles[1]));
	particles.push_back(std::make_unique<particle>(*source_interaction.particles[2]));
}

void interaction::assign_particle(const particle& source_particle)
{
	particles[0] = std::make_unique<particle>(source_particle);
}
void interaction::assign_particles(const std::vector<particle>& source_particles)
{
	particles[0] = std::make_unique<particle>(source_particles[0]);
	particles[1] = std::make_unique<particle>(source_particles[1]);
	particles[2] = std::make_unique<particle>(source_particles[2]);
	left->assign_particle(source_particles[1]);
	right->assign_particle(source_particles[2]);
}
particle interaction::get_particle_0() const
{
	return *particles[0];
}
void interaction::get_next_particles()
{
	particle particle_l{ left->get_particle_0() };
	particle particle_r{ right->get_particle_0() };
	particles[1] = std::make_unique<particle>(particle_l);
	particles[2] = std::make_unique<particle>(particle_r);
}
std::string interaction::info() const
{
	if (particles[0]->is_outgoing == true) {
		return particles[0]->info() + ">";
	}
	return particles[0]->info() + "<";
}

std::vector<std::vector<particle>> interaction::checking_procedure()
//Returns a vector of possible particle combinations that could be assigned to this interaction.
//If there are no allowed combinations, the returned vector is empty.
{
	std::vector<std::vector<particle>> fit_particles{};

	//Peek at the particles assigned to subsequent vertices:
	if (particles[1]->index == 0 || particles[2]->index == 0) {
		get_next_particles();
	}
	//For internal interactions, the direction of the initial particle must be switched (switched back after checking).
	if (key != 1) {
		particles[0]->switch_direction();
	}
	if (particles[1]->index == 0 || particles[2]->index == 0) {
		std::vector<std::string> fit_flavours{};
		size_t n{};

		if (particles[1]->index != 0) {
			n = 1;
		}
		else {
			n = 2;
		}
		fit_flavours = flavour_checker_2(particles[0]->flavour, particles[n]->flavour);
		for (auto& flavourless_particle : flavourless_particles) {
			for (auto& fit_flavour : fit_flavours) {
				particle fit_particle{ flavourless_particle };
				if (fit_particle.set_flavour(fit_flavour)) {
					if (n == 1) {
						if (checker_helper(std::vector<particle>{*particles[0], * particles[1], fit_particle})) {
							if (key != 1) {
								particles[0]->switch_direction();
							}
							fit_particles.push_back(std::vector<particle>{*particles[0], * particles[1], fit_particle});
						}
					}
					else {
						if (checker_helper(std::vector<particle>{*particles[0], fit_particle, * particles[2]}))
						{
							if (key != 1) {
								particles[0]->switch_direction();
							}
							fit_particles.push_back(std::vector<particle>{*particles[0], fit_particle, * particles[2]});
						}
					}
				}
			}
		}
	}
	else
	{
		if (flavour_checker_3(particles[0]->flavour, particles[1]->flavour, particles[2]->flavour)) {
			if (checker_helper(std::vector<particle>{*particles[0], * particles[1], * particles[2]})) {
				if (key != 1) {
					particles[0]->switch_direction();
				}
				fit_particles.push_back(std::vector<particle>{*particles[0], * particles[1], * particles[2]});
			}
		}
	}
	return fit_particles;
}
} //namespace fdiagram_project