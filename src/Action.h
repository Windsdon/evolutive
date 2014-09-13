/*
 * Behaviour.h
 *
 * Uma ação para o robô executar
 *
 */

#pragma once

enum ActionTypes {
	ACTION_ANGULAR_ACC,     // Define aceleração angular
	ACTION_ANGLUAR_VEL,     // Define velocidade angular
	ACTION_ANGULAR_DELTA,   // Movimento angular relativo
	ACTION_LINEAR_ACC,      // Define aceleração linear
	ACTION_LINEAR_VEL,      // Define velocidade linear
	ACTION_LINEAR_VEL_REL   // Velocidade linear relativa
};

class Action {
	public:
		Action(int type, double value, double duration) :
				type(type), value(value), duration(duration) {

		}

		int type;
		double value;
		double duration;

};
