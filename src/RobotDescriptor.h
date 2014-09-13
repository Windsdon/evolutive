/*
 * Descreve completamente o comportamento de um individuo
 * */
#pragma once

#include <fstream>
#include "Behaviour.h"

using namespace std;

class RobotDescriptor {
	public:
		/*
		 * Inicialização a partir de arquivo
		 */
		//RobotDescriptor(ifstream &file);
		/*
		 * Inicialização vazia
		 */
		RobotDescriptor() {
		}

		void addBehavior(Behaviour *b) {
			behaviours.push_back(b);
		}

		vector<Behaviour*> behaviours;

};
