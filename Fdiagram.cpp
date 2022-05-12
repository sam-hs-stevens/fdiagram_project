
#include "Fdiagram.h"

#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <iostream>

#include "Particle.h"
#include "Vertex.h"
#include "Interaction.h"
#include "Endpoint.h"

namespace fdiagram_project {

//Fdiagram private member functions:
void fdiagram::construct_primitive_diagram(const std::unique_ptr<vertex>& vertice)
{
	vertice->left = std::make_unique<vertex>();
	vertice->right = std::make_unique<vertex>();
}
void fdiagram::destroy_diagram(std::unique_ptr<vertex>& vertice)
{
	if (vertice != nullptr) {
		destroy_diagram(vertice->left);
		destroy_diagram(vertice->right);
		vertice.reset();
	}
}

std::pair<size_t, size_t> fdiagram::key_assigner(std::unique_ptr<vertex>& source_vertice, std::pair<size_t, size_t> keycount)
{
	//Recursively traverses the diagram, replaces base class vertices with derived classes and assigns keys.
	//Endpoints have keys 1, 2, 3..., interactions have keys 100, 101, 102... .
	if (source_vertice->left == nullptr && source_vertice->right == nullptr) {
		source_vertice = std::make_unique<endpoint>(std::move(*source_vertice));
		source_vertice->key = keycount.first++;
	}
	else {
		source_vertice = std::make_unique<interaction>(std::move(*source_vertice));
		if (std::get<0>(keycount) == 1) {
			//(called for the root vertex)
			source_vertice->key = keycount.first++;
		}
		else {
			source_vertice->key = keycount.second++;
		}
		keycount = key_assigner(source_vertice->left, keycount);
		keycount = key_assigner(source_vertice->right, keycount);
	}
	return keycount;
}
std::vector<particle>::iterator fdiagram::particle_assigner(std::unique_ptr<vertex>& source_vertice,
	std::vector<particle>::iterator particles_it)
{
	//Recursively traverses the diagram, assigns initial/final state particles to the endpoints.
	if (source_vertice->key < 100 && source_vertice->key != 1) {
		source_vertice->assign_particle(*particles_it);
		particles_it++;
	}
	else {
		if (source_vertice->key == 1) {
			//(called for the root vertex)
			source_vertice->assign_particle(*particles_it);
			particles_it++;
		}
		particles_it = particle_assigner(source_vertice->left, particles_it);
		particles_it = particle_assigner(source_vertice->right, particles_it);
	}
	return particles_it;
}
std::vector<std::unique_ptr<fdiagram>> fdiagram::interaction_assigner(std::unique_ptr<vertex>& source_vertice)
{
	//Returns a vector of diagrams with one more interaction assigned.
	//If no suitable interaction could be found, an empty vector is returned.

	std::vector<std::unique_ptr<fdiagram>> diagram_copy_vector{};
	//Only interaction vertices need to be checked:
	if (source_vertice->key == 1 || source_vertice->key >= 100) {

		std::vector<std::vector<particle>> check_output{};
		check_output = source_vertice->checking_procedure();

		if (check_output.size() == 1) {
			//There is only one possible interaction, continue to traverse the diagram.
			source_vertice->assign_particles(check_output[0]);
		}
		else if (check_output.size() == 0) {
			//There is no possible interaction, return empty vector.
			return diagram_copy_vector;
		}
		else {
			for (auto& particles : check_output) {
				//There are multiple possible interactions.
				source_vertice->assign_particles(particles);
				std::unique_ptr<fdiagram> diagram_copy{ std::make_unique<fdiagram>(source_vertice) };
				diagram_copy_vector.push_back(std::move(diagram_copy));
			}
			return diagram_copy_vector;
		}
		//Traverse the diagram to the left:
		std::vector<std::unique_ptr<fdiagram>> subdiagrams{};
		subdiagrams = interaction_assigner(source_vertice->left);
		if (subdiagrams.size() > 1) {
			//Append the copied subdiagrams to this one:
			for (auto& diagram : subdiagrams) {
				std::unique_ptr<fdiagram> diagram_copy = std::make_unique<fdiagram>(source_vertice);
				diagram_copy->append_replace(true, diagram);
				diagram_copy_vector.push_back(std::move(diagram_copy));
			}
			return diagram_copy_vector;
		}if (subdiagrams.size() == 0) {
			//There is no possible interaction, return empty vector.
			return diagram_copy_vector;
		}
		subdiagrams.clear();
		//Traverse the diagram to the right:
		subdiagrams = interaction_assigner(source_vertice->right);
		if (subdiagrams.size() > 1) {
			//Append the copied subdiagrams to this one:
			for (auto& diagram : subdiagrams) {
				std::unique_ptr<fdiagram> diagram_copy = std::make_unique<fdiagram>(source_vertice);
				diagram_copy->append_replace(false, diagram);
				diagram_copy_vector.push_back(std::move(diagram_copy));
			}
			return diagram_copy_vector;
		}if (subdiagrams.size() == 0) {
			//There is no possible interaction, return empty vector.
			return diagram_copy_vector;
		}
	}
	//All interactions are assigned, return self:
	diagram_copy_vector.clear();
	std::unique_ptr<fdiagram> diagram_copy{ std::make_unique<fdiagram>(source_vertice) };
	diagram_copy->make_complete();
	diagram_copy_vector.push_back(std::move(diagram_copy));
	return diagram_copy_vector;
}

std::unique_ptr<vertex> fdiagram::deep_copy(const std::unique_ptr<vertex>& source_vertice) {
	//Deep copies the diagram, creating a new unique pointer.
	//Calls clone method to avoid slicing.
	if (source_vertice == nullptr) return nullptr;
	std::unique_ptr<vertex> dest_vertice{ source_vertice->clone() };
	dest_vertice->left = deep_copy(source_vertice->left);
	dest_vertice->right = deep_copy(source_vertice->right);
	return dest_vertice;
}
void fdiagram::print(const std::string& prefix, const std::unique_ptr<vertex>& vertice, bool is_left) const
{
	//Recursively prints the structure of a diagram to the console.
	if (vertice != nullptr) {
		std::cout << prefix;
		std::cout << (is_left ? "|___" : "|___") << vertice->info() << std::endl;
		print(prefix + (is_left ? "|    " : "     "), vertice->left, true);
		print(prefix + (is_left ? "|    " : "     "), vertice->right, false);
	}
}

//Fdiagram public member functions:
fdiagram::fdiagram(const size_t& order_in) : order{ order_in }, is_complete{ false }, root{ std::make_unique<vertex>() } {
	construct_primitive_diagram(root);
}
fdiagram::fdiagram(const fdiagram& source_diagram) : root{ deep_copy(source_diagram.root) }, order{ source_diagram.order },
is_complete{ source_diagram.is_complete } {}

void fdiagram::assign_keys()
{
	//Initial endpoint keycount 1 and initial interaction keycount 100.
	std::pair<size_t, size_t> keycount{ 1, 100 };
	key_assigner(root, keycount);
}
void fdiagram::assign_endpoint_particles(std::vector<particle> particles)
{
	particle_assigner(root, particles.begin());
}
std::vector<std::unique_ptr<fdiagram>> fdiagram::assign_interactions(std::unique_ptr<fdiagram>& source_diagram)
{
	//Repeatedly calls interaction_assigner in order to return a vector of completed diagrams.
	//If no completed diagrams can be found, an empty vector is returned.
	std::vector<std::unique_ptr<fdiagram>> complete_diagrams{};
	std::vector<std::unique_ptr<fdiagram>> partial_diagrams{};

	if (source_diagram->get_is_complete()) {
		complete_diagrams.push_back(std::move(source_diagram));
		return complete_diagrams;
	}
	partial_diagrams = interaction_assigner(source_diagram->root);
	if (partial_diagrams.size() != 0) {
		for (auto& partial_diagram : partial_diagrams) {

			std::vector<std::unique_ptr<fdiagram>> sub_complete_diagrams{};
			sub_complete_diagrams = assign_interactions(partial_diagram);
			for (auto& complete_diagram : sub_complete_diagrams) {
				complete_diagrams.push_back(std::move(complete_diagram));
			}
		}
	}
	return complete_diagrams;
}

void fdiagram::append(const bool is_left, const std::unique_ptr<fdiagram>& sub_fdiagram)
{
	//Appends a subdiagram to the left or right branch of the root.
	if (sub_fdiagram != nullptr) {
		if ((is_left ? root->left : root->right) != nullptr) {
			order += sub_fdiagram->order;
			sub_fdiagram->order = 0;
			(is_left ? root->left : root->right) = std::move(sub_fdiagram->root);
		}
		else {
			std::cout << "Fdiagram not appendable." << std::endl;
		}
	}
}
void fdiagram::append_replace(const bool is_left, const std::unique_ptr<fdiagram>& sub_fdiagram)
{
	//Replaces the left or right branch of the root.
	if (sub_fdiagram != nullptr) {
		(is_left ? root->left : root->right) = std::move(sub_fdiagram->root);
	}
}
} //namespace fdiagram_project
