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

		RobotDescriptor(ifstream &file);

		void loadFromFile(ifstream &file);

		/*
		 * Inicialização vazia
		 */
		RobotDescriptor() {
		}

		void saveToFile(ofstream &file) const;

		void addBehavior(Behaviour *b) {
			behaviours.push_back(b);
		}

		vector<Behaviour*> behaviours;

};
