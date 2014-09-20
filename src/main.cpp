/*
 * main.cpp
 *
 */

#include "Robot.h"
#include "Behaviour.h"
#include "Action.h"
#include <player-3.0/libplayerc++/playerc++.h>
#include <iostream>
#include <cstdlib>
#include <exception>
#include <sstream>

#define _GXX_EXPERIMENTAL_CXX0X__
#include <chrono>

using namespace PlayerCc;
using namespace std;

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
	PlayerClient *robot = nullptr;

	while(robot == nullptr){
		try{
			robot = new PlayerClient("localhost", 6665);
		}catch(PlayerError &ex){
			robot = nullptr;
			sleep(1);
		}
	}

	Position2dProxy pp(robot);
	LaserProxy lp(robot);

	double rtx = 6;
	double rty = 6;
	double timeLimit = 60;

	RobotDescriptor descriptor;

	Robot r(&pp, &lp, &descriptor);

	/*ifstream inputFile;
	inputFile.open("desc");
	descriptor.loadFromFile(inputFile);
	inputFile.close();*/

	srand(time(0));

	// criar comportamentos
	int rb = rand() % 5 + 2;
	for (int i = 0; i < rb; i++) {
		double minDist = (rand() % 100) / 100.0;
		Behaviour *b = new BehaviourOnObstacleDistance(&r, rand() % 360,
				minDist, minDist + (rand() % 400) / 100.0);

		int ra = rand() % 10 + 1;
		for (int i = 0; i < ra; i++) {
			Action *act;
			if (rand() % 2) {
				act = new Action(ACTION_LINEAR_VEL, (rand() % 100) / 100.0,
						(rand() % 100) / 100.0);
			} else {
				act = new Action(ACTION_ANGULAR_VEL, (rand() % 200) / 100.0 - 1,
						(rand() % 100) / 100.0);
			}
			b->addAction(act);
		}

		descriptor.addBehavior(b);
	}

	BehaviourOnObstacleDistance far(&r, 180, 0, 100);
	far.addAction(new Action(ACTION_LINEAR_VEL, 1, 0.1));

	// senão tem uma grande chance dele ficar parado
	descriptor.addBehavior(&far);

	printDescriptor(cout, descriptor);


	stringstream fileName;
	fileName << "robots/desc" << time(NULL);

	ofstream outputFile;
	outputFile.open(fileName.str());
	descriptor.saveToFile(outputFile);
	outputFile.close();


	auto begin = chrono::high_resolution_clock::now();

	double points = -1;
	bool stall = false;

	while (true) {
		robot->Read();
		r.update();

		if(pp.GetStall()){
			stall = true;
			break;
		}

		auto now = chrono::high_resolution_clock::now();
		double elapsed = (chrono::duration_cast < chrono::duration<double>
				> (now - begin)).count();

		double dist = pow(pp.GetXPos() - rtx, 2) + pow(pp.GetYPos() - rty, 2);

		if(points < 0 || dist < points) {
			points = dist;
		}

		if (elapsed > timeLimit) {
			break;
		}
	}

	cout << "Acabou o tempo! Pontos: " << points << endl;
	cout << "Bateu: " << stall << endl;

	return 0;
}

