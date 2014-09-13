/*
 * Robot.h
 *
 * Interface para um indivíduo
 *
 */

#pragma once

#include <player-3.0/libplayerc++/playerc++.h>
#include "RobotDescriptor.h"
#include <chrono>

using namespace PlayerCc;

class Robot {
	public:
		Robot(Position2dProxy* pp, LaserProxy* lp, RobotDescriptor* desc) :
				pp(pp), lp(lp), desc(desc), activeBehaviour(nullptr), activeAction(
						nullptr), actionPerformed(false) {
			lastBehaviourUpdate = chrono::high_resolution_clock::now();
		}

		void update();

		void scanNextBehaviour();

		void runAction();

		bool actionPerformed;

		Position2dProxy *pp;
		LaserProxy *lp;
		RobotDescriptor *desc;
		Behaviour *activeBehaviour;
		Action *activeAction;

		chrono::high_resolution_clock::time_point lastBehaviourUpdate;

};
