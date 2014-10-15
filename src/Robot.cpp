/*
 * Robot.cpp
 *
 */

#include "Robot.h"

void Robot::update() {
	auto now = chrono::high_resolution_clock::now();
	double elapsed = (chrono::duration_cast < chrono::duration<double> > (now - lastBehaviourUpdate)).count();

	if(activeAction){
		if(activeAction->duration < elapsed){
			//cout << "Tempo de ação esgotado!" << endl;
			if(activeBehaviour){
				activeAction = activeBehaviour->getNextAction();
			} else {
				activeAction = nullptr;
			}

			actionPerformed = false;
		}
	} else {
		if(activeBehaviour){
			activeAction = activeBehaviour->getNextAction();
			actionPerformed = false;
		}
	}

	if(!activeAction || !activeBehaviour){
		//cout << "Começando o scan de comportamento" << endl;
		scanNextBehaviour();
	}

	runAction();
}

void Robot::scanNextBehaviour() {
	for(unsigned int i = 0; i < desc->behaviours.size(); i++){
		if(desc->behaviours[i]->checkConditions()){
			activeBehaviour = desc->behaviours[i];
			activeBehaviour->reset();

			//cout << "Ativando comportamento " << activeBehaviour << endl;
			break;
		}
	}
}

void Robot::runAction() {
	if(activeAction && !actionPerformed){
		lastBehaviourUpdate = chrono::high_resolution_clock::now();

		switch (activeAction->type) {
			case ACTION_LINEAR_VEL:
				pp->SetSpeed(activeAction->value, 0);
				break;
			case ACTION_ANGULAR_VEL:
				pp->SetSpeed(0, activeAction->value);
				break;
			default:
				break;
		}
	}

	actionPerformed = true;
}
