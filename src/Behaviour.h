/*
 * Behaviour.h
 *
 * Descreve uma sequência de comandos que são executados
 * quando uma condição é atendida
 *
 */

#pragma once

#include <vector>
#include "Action.h"

using namespace std;

class Robot;

class Behaviour {
	public:

		Behaviour(Robot* robot) :
				currentAction(0), robot(robot) {
		}

		/*
		 * Verifica se esse comportamento deve ser ativado
		 */
		virtual bool checkConditions() = 0;

		/*
		 * Próxima ação a ser executada
		 * Retorna null se tiver chegado ao fim
		 */
		Action *getNextAction() {
			if (currentAction >= actions.size()) {
				return 0;
			} else {
				return actions[currentAction++];
			}
		}

		/*
		 * Reiniciar para a primeira ação
		 */
		void reset() {
			currentAction = 0;
		}

		void addAction(Action* action) {
			actions.push_back(action);
		}

		virtual ~Behaviour() {
		}

		Robot* getRobot() {
			return robot;
		}

		vector<Action*> actions;
		int currentAction;
		Robot *robot;
};

class BehaviourOnObstacleDistance: public Behaviour {
	public:
		BehaviourOnObstacleDistance(Robot* robot, double angle,
				double distanceMin, double distanceMax) :
				Behaviour(robot), angle(angle), distanceMin(distanceMin), distanceMax(
						distanceMax) {

		}

		virtual bool checkConditions();

	private:
		double angle, distanceMin, distanceMax;
};
