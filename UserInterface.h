#pragma once

#ifndef USERINTERFACE_H
#define USERINTERFACE_H

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

bool user_interface_helper();
void user_interface();
} //namespace fdiagram_project

#endif