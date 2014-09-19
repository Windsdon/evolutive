/*
 * Behaviour.h
 *
 * Uma ação para o robô executar
 *
 */

#pragma once

#include <iostream>
using namespace std;

enum ActionTypes {
	ACTION_ANGULAR_ACC,     // Define aceleração angular
	ACTION_ANGULAR_VEL,     // Define velocidade angular
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

		void print(ostream &os) const {
			os << "[Action] (type = ";
			switch (type) {
				case ACTION_ANGULAR_ACC:
					os << "ACTION_ANGLUAR_ACC";
					break;
				case ACTION_ANGULAR_VEL:
					os << "ACTION_ANGULAR_VEL";
					break;
				case ACTION_ANGULAR_DELTA:
					os << "ACTION_ANGULAR_DELTA";
					break;
				case ACTION_LINEAR_ACC:
					os << "ACTION_LINEAR_ACC";
					break;
				case ACTION_LINEAR_VEL:
					os << "ACTION_LINEAR_VEL";
					break;
				case ACTION_LINEAR_VEL_REL:
					os << "ACTION_LINEAR_VEL_REL";
					break;
				default:
					os << "!!UNDEFINED!!";
			}
			os << ", value = " << value << ", duration = " << duration << ")";
		}

};
