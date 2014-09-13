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

	BehaviourOnObstacleDistance b(&r, 0, 0, 0);

	srand(time(0));

	for (int i = 0; i < 10 + rand() % 30; i++) {
		b.addAction(
				new Action(rand() % 2 ? ACTION_LINEAR_VEL : ACTION_ANGLUAR_VEL,
						(rand() % 100) / 100.0, rand() % 2));
	}

	descriptor.addBehavior(&b);

	while (true) {
		robot.Read();
		r.update();
	}
}

