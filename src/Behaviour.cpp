/*
 * Behaviour.cpp
 *
 */

#include "Behaviour.h"
#include "Robot.h"

void Behaviour::saveActionsToFile(ofstream &file) const {
	unsigned int numActions = actions.size();
	file << "LIST " << numActions << endl;
	for(unsigned int i = 0; i < numActions; i++) {
		actions[i]->saveToFile(file);
	}
}

BehaviourOnObstacleDistance::BehaviourOnObstacleDistance(Robot* r, map<string, double> param): Behaviour(r){
	angle = (int) param["angle"];
	distanceMin = param["min"];
	distanceMax = param["max"];
}

bool BehaviourOnObstacleDistance::checkConditions() {
	if (!robot || !(robot->lp)) {
		cerr << "Robot ou laser não definido" << endl;
		return false;
	}

	cout << "Count: " << robot->lp->GetCount() << endl;

	if (!robot->lp->GetCount()) {
		return 0;
	}

	double lr = robot->lp->GetRange(angle);
	cout << "Distance at " << angle << ": " << lr << endl;

	if (lr <= distanceMax && lr >= distanceMin) {
		return true;
	}

	return false;
}

void BehaviourOnObstacleDistance::print(ostream& os) const {
	os << "[BehaviourOnObstacleDistance] (min = " << distanceMin << ", max = "
			<< distanceMax << ", angle = " << angle << "). Actions: " << endl;
	for (unsigned int i = 0; i < actions.size(); i++) {
		os << "* ";
		actions[i]->print(os);
		os << endl;
	}
}

void BehaviourOnObstacleDistance::saveToFile(ofstream &file) const {
	file << "BEHAVIOUR id=" << getID() << " min=" << distanceMin << " max="
			<< distanceMax << " angle=" << angle << endl;

	saveActionsToFile(file);
}
