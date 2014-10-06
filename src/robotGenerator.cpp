/*
 * robotGenerator.cpp
 *
 *  Created on: Oct 6, 2014
 *      Author: suporte
 */

#include "Robot.h"
#include "Behaviour.h"
#include "Action.h"
#include <player-3.0/libplayerc++/playerc++.h>
#include <iostream>
#include <cstdlib>
#include <exception>
#include <sstream>
#include <algorithm>

void printBehaviour(ostream& left, const Behaviour& b) {
	left << "Behaviour " << &b << " ";
	b.print(left);
}

void printDescriptor(ostream& left, const RobotDescriptor &rd) {
	left << "Descriptor " << &rd << ". Enumerating behaviours:" << endl;
	for (unsigned int i = 0; i < rd.behaviours.size(); i++) {
		printBehaviour(left, *(rd.behaviours[i]));
	}
}

int main(int argc, char **argv) {
	srand(time(0));

	if (argc <= 1) {
		cout << "FALTAM ARGUMENTOS!" << endl;
		return 1;
	}

	istringstream numberiss(argv[1]);
	int numDesc;
	numberiss >> numDesc;

	int numStart;
	if (argc > 2) {
		istringstream numberiss2(argv[2]);
		numberiss2 >> numStart;
	} else {
		numStart = rand();
	}

	ofstream output;
	output.open("generated.txt");

	for (int k = 0; k < numDesc; k++) {

		RobotDescriptor descriptor;

		// criar comportamentos
		int rb = rand() % 5 + 2;
		for (int i = 0; i < rb; i++) {
			double minDist = (rand() % 100) / 100.0;
			Behaviour *b = new BehaviourOnObstacleDistance(nullptr,
					rand() % 360, minDist, minDist + (rand() % 400) / 100.0);

			int ra = rand() % 10 + 1;
			for (int i = 0; i < ra; i++) {
				Action *act;
				if (rand() % 2) {
					act = new Action(ACTION_LINEAR_VEL, (rand() % 100) / 100.0,
							(rand() % 100) / 100.0);
				} else {
					act = new Action(ACTION_ANGULAR_VEL,
							(rand() % 200) / 100.0 - 1, (rand() % 100) / 100.0);
				}
				b->addAction(act);
			}

			descriptor.addBehavior(b);
		}

		BehaviourOnObstacleDistance far(nullptr, 180, 0, 100);
		far.addAction(new Action(ACTION_LINEAR_VEL, 1, 0.1));

		// senão tem uma grande chance dele ficar parado
		descriptor.addBehavior(&far);

		printDescriptor(cout, descriptor);

		stringstream fileName;
		fileName << "robots/desc" << (numStart + k);

		ofstream outfile;
		outfile.open(fileName.str());
		descriptor.saveToFile(outfile);
		outfile.close();

		output << (numStart + k) << endl;
	}

	output.close();
}

