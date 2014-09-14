/*
 * Behaviour.cpp
 *
 */

#include "Behaviour.h"
#include "Robot.h"

bool BehaviourOnObstacleDistance::checkConditions() {
	if(!robot || !(robot->lp)){
		cerr << "Robot ou laser não definido" << endl;
		return false;
	}

	cout << "Count: " << robot->lp->GetCount() << endl;

	if(!robot->lp->GetCount()){
		return 0;
	}

	double lr = robot->lp->GetRange(angle);
	cout << "Distance at " << angle << ": " << lr << endl;


	if(lr <= distanceMax && lr >= distanceMin){
		return true;
	}

	return false;
}
