/*
 * RobotDescriptor.cpp
 */

#include "RobotDescriptor.h"
#include <string>

#include <regex>

void RobotDescriptor::saveToFile(ofstream &file) const {
	unsigned int numBehaviours = behaviours.size();
	file << "DESCRIPTOR" << endl;
	file << "LIST " << numBehaviours << endl;

	for (unsigned int i = 0; i < numBehaviours; i++) {
		behaviours[i]->saveToFile(file);
	}
}

void RobotDescriptor::loadFromFile(ifstream &file) {
	string line;

	regex regList("LIST ([0-9]+)");
	smatch matches;

	do {
		getline(file, line);

	} while (!file.eof());
}

