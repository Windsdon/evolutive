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

	/*
	 BehaviourOnObstacleDistance bClose(&r, 180, 0, 1.5);

	 for (int i = 0; i < 10; i++) {
	 bClose.addAction(
	 new Action(rand() % 2 ? ACTION_LINEAR_VEL : ACTION_ANGLUAR_VEL,
	 -1 + (rand() % 200) / 100.0, rand() % 2));
	 }

	 BehaviourOnObstacleDistance bFar(&r, 180, 1.5, 100);

	 for (int i = 0; i < 10; i++) {
	 bFar.addAction(
	 new Action(rand() % 2 ? ACTION_LINEAR_VEL : ACTION_ANGLUAR_VEL,
	 -1 + (rand() % 200) / 100.0, rand() % 2));
	 }

	 */

	BehaviourOnObstacleDistance bClose(&r, 180, 0, 1);
	BehaviourOnObstacleDistance bCloseL(&r, 270, 0, 0.7);
	BehaviourOnObstacleDistance bCloseR(&r, 90, 0, 0.7);
	BehaviourOnObstacleDistance bCloseL2(&r, 360, 0, 0.3);
	BehaviourOnObstacleDistance bCloseR2(&r, 0, 0, 0.3);
	BehaviourOnObstacleDistance bFar(&r, 180, 1, 100);

	bClose.addAction(new Action(ACTION_ANGLUAR_VEL, 0.5, 0.1));
	bCloseL.addAction(new Action(ACTION_ANGLUAR_VEL, -0.5, 0.2));
	bCloseR.addAction(new Action(ACTION_ANGLUAR_VEL, 0.5, 0.2));
	bCloseL2.addAction(new Action(ACTION_ANGLUAR_VEL, -0.5, 0.2));
	bCloseR2.addAction(new Action(ACTION_ANGLUAR_VEL, 0.5, 0.2));
	bFar.addAction(new Action(ACTION_LINEAR_VEL, 1, 0.1));

	descriptor.addBehavior(&bClose);
	descriptor.addBehavior(&bCloseL);
	descriptor.addBehavior(&bCloseR);
	descriptor.addBehavior(&bCloseL2);
	descriptor.addBehavior(&bCloseR2);
	descriptor.addBehavior(&bFar);

	while (true) {
		robot.Read();
		r.update();
	}
}

