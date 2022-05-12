
#include "UserInterface.h"

#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <sstream>

#include "Particle.h"
#include "Vertex.h"
#include "Interaction.h"
#include "Endpoint.h"
#include "Fdiagram.h"
#include "Fdiagrams.h"

namespace fdiagram_project {

bool user_interface_helper()
{
	//Handles input of initial/final state particles by the user and outputs corresponding feynman diagrams.
	//Returns false for failed input, true for succesful excecution.

	std::cout << "Input the initial/final state particles for a diagram you would like to find. \n \n"
		<< "Please use the notation defined above, and separate particles using commas. \n"
		<< "For example, if I were to input the initial/final state particles for the example \n"
		<< "diagram above, I would input:   m<, m*<, e>, e*>, y>,  \n"
		<< std::endl;

	std::string input_str{};
	std::vector<particle> input_particles{};

	std::getline(std::cin, input_str);

	//Remove spaces and check number of particles:
	input_str.erase(remove(input_str.begin(), input_str.end(), ' '), input_str.end());
	size_t num_input_particles = std::count(input_str.begin(), input_str.end(), '<')
		+ std::count(input_str.begin(), input_str.end(), '>');
	if (num_input_particles == 0) return false;

	//Read in particles:
	std::stringstream input_stream{ input_str };
	for (size_t i{}; i < num_input_particles; i++) {

		std::string particle_str{};
		particle input_particle{};
		bool is_outgoing{ true };
		bool is_matter{ true };

		std::getline(input_stream, particle_str, ',');

		//Check if particle incoming/outgoing:
		if (std::count(particle_str.begin(), particle_str.end(), '<') == 1) {
			is_outgoing = false;
			particle_str.erase(remove(particle_str.begin(), particle_str.end(), '<'), particle_str.end());
		}
		else if (std::count(particle_str.begin(), particle_str.end(), '>') == 1) {
			is_outgoing = true;
			particle_str.erase(remove(particle_str.begin(), particle_str.end(), '>'), particle_str.end());
		}
		else {
			std::cout << "Cannot read from input, check format. \n" << std::endl;
			return false;
		}
		//Check if particle matter/antimatter:
		if (std::count(particle_str.begin(), particle_str.end(), '*') == 1) {
			is_matter = false;
			particle_str.erase(remove(particle_str.begin(), particle_str.end(), '*'), particle_str.end());
		}
		//Check particle flavour:
		input_particle = instantiate_particle(particle_str);
		if (input_particle.index == 0) {
			std::cout << "Cannot read from input, check format. \n" << std::endl;
			return false;
		}
		input_particle.is_outgoing = is_outgoing;
		input_particle.is_matter = is_matter;
		input_particles.push_back(input_particle);
	}
	if (input_particles.size() < 3) {
		std::cout << "AT LEAST 3 incoming/outgoing particles are required for a valid feynman diagram. \n" << std::endl;
		return false;
	}
	if (input_particles[0].is_outgoing)
	{
		std::cout << "The first particle entered MUST be an incoming particle. \n" << std::endl;
		return false;
	}

	fdiagrams output_diagrams{ input_particles.size() - 2, input_particles };
	output_diagrams.print();
	if (output_diagrams.size() == 0) {
		std::cout << "No valid feynman diagrams for that input could be constructed. \n" << std::endl;
	}
	else {
		std::cout << output_diagrams.size() << " diagrams constructed." << std::endl;
	}
	return true;
}
void user_interface()
{
	//Prints a banner on program startup, and calls "user_interface_helper" as many times as the user chooses.
	std::cout << "\n \n \t \t \t \t FEYNMAN DIAGRAM CONSTRUCTOR" << std::endl;
	std::cout << "\t \t \t \t \t Sam Stevens \n \n" << std::endl;
	std::cout << "This program serves as proof of concept for a more fully-fledged feynman diagram constructor. \n"
		<< "Due to time constraints, there are many aspects of feynman diagrams that are not implemented in this version. \n"
		<< "\t --> There is no accounting for diagrams containing LOOPS. \n"
		<< "\t --> Weak interactions are not included, thus neutrinos are also missing. \n \t --> The Higgs boson is not included. \n"
		<< "\t --> The third quark generation is not included. \n \t --> Gluon 4-vertices are not included. \n"
		<< "\t --> Energy/Momentum and spin considerations are not accounted for. \n"
		<< "\t --> There is no accounting for diagrams containing non-interacting particles. \n"
		<< std::endl;
	std::cout << "A guide to interpreting the output of this program. \n"
		<< "\t --> Particle flavours are represented by letters, an antiparticle is denoted by an asterisk *. \n"
		<< "\t --> The symbols < and > represent incoming and outgoing particles, respectively. \n \n"
		<< "e: electron  m : muon  u : up quark  d : down quark  c : charm quark  s : strange quark  y : photon  g : gluon. \n \n"
		<< "\t For example, the output: \n " << std::endl;
	std::cout
		<< "                            |___m<                                  \n"
		<< "                                |___m*<                             \n"
		<< "                                |___y>                             \n"
		<< "                                    |___e>                          \n"
		<< "                                    |___e*>                        \n"
		<< "                                        |___e*>                    \n"
		<< "                                        |___y>                     \n"
		<< std::endl;
	std::cout << " \n \t Would be interpreted as: \n \n"
		<< "                             \\  m             /                   \n"
		<< "                              \\              / e                   \n"
		<< "                               \\            /                     \n"
		<< "                                \\    y     /                      \n"
		<< "                                 ~~~~~~~~~~                        \n"
		<< "                                /          \\                      \n"
		<< "                               /            \\ e*                   \n"
		<< "                              /              \\        y            \n"
		<< "                             / m*             \\~~~~~~~~~~~~        \n"
		<< "                                               \\                    \n"
		<< "                                                \\                  \n"
		<< "                                                 \\                  \n"
		<< "                                                  \\                 \n"
		<< "                                                   \\ e*             \n"
		<< std::endl;

	bool restart{ true };
	while (restart) {
		if (user_interface_helper()) {
			std::cout << "Would you like to run the program again?" << std::endl;
		}
		else {
			std::cout << "There was a problem with your input, would you like to try again?" << std::endl;
		}
		std::cout << "Input y for yes and n for no:" << std::endl;
		bool successful_input{ false };
		while (!successful_input) {
			std::string restart_input{};
			std::getline(std::cin, restart_input);
			std::cout << std::endl;

			//Remove spaces and verify input:
			restart_input.erase(remove(restart_input.begin(), restart_input.end(), ' '), restart_input.end());
			if (restart_input == "y" || restart_input == "Y") {
				restart = true;
				successful_input = true;
			}
			else if (restart_input == "n" || restart_input == "N") {
				std::cout << "Bye!" << std::endl;
				restart = false;
				successful_input = true;
			}
			else {
				std::cout << "You must input either y for yes or n for no: " << std::endl;
				std::cin.clear();
				successful_input = false;
			}
		}
	}
}
} //namespace fdiagram_project
