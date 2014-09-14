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

#define _GXX_EXPERIMENTAL_CXX0X__
#include <chrono>

using namespace PlayerCc;
using namespace std;

int main(int argc, char **argv) {
	PlayerClient robot("localhost", 6665);
	Position2dProxy pp(&robot);
	LaserProxy lp(&robot);

	RobotDescriptor descriptor;

	Robot r(&pp, &lp, &descriptor);

	srand(time(0));

	// criar comportamentos
	int rb = rand()%5 + 2;
	for(int i = 0; i < rb; i++){
		double minDist = (rand() % 100)/100.0;
		Behaviour *b = new BehaviourOnObstacleDistance(&r, rand()%360, minDist, minDist + (rand() % 400)/100.0);

		int ra = rand() % 10 + 1;
		for(int i = 0; i < ra; i++){
			Action *act;
			if(rand()%2){
				act = new Action(ACTION_LINEAR_VEL, (rand()%200)/100.0 - 1, (rand()%100)/100.0);
			}else{
				act = new Action(ACTION_ANGLUAR_VEL, (rand()%200)/100.0 - 1, (rand()%100)/100.0);
			}
			b->addAction(act);
		}

		descriptor.addBehavior(b);
	}

	BehaviourOnObstacleDistance far(&r, 180, 0, 100);
	far.addAction(new Action(ACTION_LINEAR_VEL, 1, 0.1));

	// senão tem uma grande chance dele ficar parado
	descriptor.addBehavior(&far);

	while (true) {
		robot.Read();
		r.update();
	}
}

