
#include "Particle.h"

#include <string>
#include <vector>
#include <memory>

namespace fdiagram_project {

//Particle public member functions:
particle::particle() : flavour{}, is_matter{}, is_outgoing{}, is_fermion{}, is_lepton{}, is_neutrino{}, is_quark{}, index{} {}
particle::particle(std::string flavour_in, bool is_matter_in, bool is_outgoing_in, bool is_fermion_in,
	bool is_lepton_in, bool is_neutrino_in, bool is_quark_in, size_t index_in)
	: flavour{ flavour_in }, is_matter{ is_matter_in }, is_outgoing{ is_outgoing_in }, is_fermion{ is_fermion_in },
	is_lepton{ is_lepton_in }, is_neutrino{ is_neutrino_in }, is_quark{ is_quark_in }, index{ index_in } {}
particle::particle(particle&& particle_in) noexcept(false)
{
	flavour = particle_in.flavour;
	is_matter = particle_in.is_matter;
	is_outgoing = particle_in.is_outgoing;
	is_fermion = particle_in.is_fermion;
	is_lepton = particle_in.is_lepton;
	is_neutrino = particle_in.is_neutrino;
	is_quark = particle_in.is_quark;
	index = particle_in.index;
	particle_in.flavour = "";
	particle_in.is_matter = false;
	particle_in.is_outgoing = false;
	particle_in.is_fermion = false;
	particle_in.is_lepton = false;
	particle_in.is_neutrino = false;
	particle_in.is_quark = false;
	particle_in.index = false;
}
particle::particle(const particle& particle_in)
{
	flavour = particle_in.flavour;
	is_matter = particle_in.is_matter;
	is_outgoing = particle_in.is_outgoing;
	is_fermion = particle_in.is_fermion;
	is_lepton = particle_in.is_lepton;
	is_neutrino = particle_in.is_neutrino;
	is_quark = particle_in.is_quark;
	index = particle_in.index;
}
particle& particle::operator=(particle&& particle_in) noexcept(false)
{
	//Check for self-assignment:
	if (&particle_in == this) return *this;
	std::swap(flavour, particle_in.flavour);
	std::swap(is_matter, particle_in.is_matter);
	std::swap(is_outgoing, particle_in.is_outgoing);
	std::swap(is_fermion, particle_in.is_fermion);
	std::swap(is_lepton, particle_in.is_lepton);
	std::swap(is_neutrino, particle_in.is_neutrino);
	std::swap(is_quark, particle_in.is_quark);
	std::swap(index, particle_in.index);
	return *this;
}
particle& particle::operator=(const particle& particle_in)
{
	//Checking for self-assignment:
	if (&particle_in == this) return *this;
	flavour = particle_in.flavour;
	is_matter = particle_in.is_matter;
	is_outgoing = particle_in.is_outgoing;
	is_fermion = particle_in.is_fermion;
	is_lepton = particle_in.is_lepton;
	is_neutrino = particle_in.is_neutrino;
	is_quark = particle_in.is_quark;
	index = particle_in.index;
	return *this;
}

void particle::switch_direction()
{
	is_outgoing = !is_outgoing;
	//Index is changed so that incoming/outgoing identical particles are recognised as distinct:
	index += 100;
}
bool particle::set_flavour(std::string flavour_in)
{
	//As well as setting flavour, this method checks that particle properties are consistent:
	flavour = flavour_in;
	if ((flavour == "e" || flavour == "m") || flavour == "t") {
		//(leptons)
		if (is_fermion != true) { return false; }
		if (is_lepton != true) { return false; }
		if (is_neutrino == true) { return false; }
		if (is_quark == true) { return false; }
		return true;
	}
	if (((flavour == "u" || flavour == "d") || flavour == "c") || flavour == "s") {
		//(quarks)
		if (is_fermion != true) { return false; }
		if (is_lepton == true) { return false; }
		if (is_neutrino == true) { return false; }
		if (is_quark != true) { return false; }
		return true;
	}
	if (flavour == "y" || flavour == "g") {
		//(bosons)
		//Not accounting for charged bosons (set is_matter = true by default).
		if (is_matter != true) { return false; }
		if (is_fermion == true) { return false; }
		if (is_lepton == true) { return false; }
		if (is_neutrino == true) { return false; }
		if (is_quark == true) { return false; }
		return true;
	}
	return false;
}
std::string particle::info() const
{
	//Called on diagram output.
	if (!is_matter) return flavour + "*";
	else return flavour;
}

//Particle non-member functions:
bool operator==(particle& lhs, particle& rhs)
{
	if (lhs.flavour != rhs.flavour) { return false; }
	else if (lhs.is_matter != rhs.is_matter) { return false; }
	else if (lhs.is_outgoing != rhs.is_outgoing) { return false; }
	else if (lhs.is_fermion != rhs.is_fermion) { return false; }
	else if (lhs.is_lepton != rhs.is_lepton) { return false; }
	else if (lhs.is_neutrino != rhs.is_neutrino) { return false; }
	else if (lhs.is_quark != rhs.is_quark) { return false; }
	else if (lhs.index != rhs.index) { return false; }
	return true;
}
bool operator!=(particle& lhs, particle& rhs)
{
	return !operator==(lhs, rhs);
}
particle instantiate_particle(std::string flavour_in)
{
	//Returns a copy of the generic particle corresponding to the flavour input.
	//If the flavour is invalid, a blank particle is returned.
	for (auto& generic_particle : generic_particles) {
		if (flavour_in == generic_particle.flavour) {
			particle return_particle{ generic_particle };
			return return_particle;
		}
	}
	particle blank_particle{};
	return blank_particle;
}
} //namespace fdiagram_project