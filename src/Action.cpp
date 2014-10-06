/*
 * Action.cpp
 *
 */

#include "Action.h"
#include <iostream>
#include <fstream>

using namespace std;


Action::Action(map<string, double> param) {
	type = (int) param["type"];
	value = param["val"];
	duration = param["duration"];
}

void Action::saveToFile(ofstream &file) const {
	file << "ACTION type=" << type << " val=" << value << " duration=" << duration << endl;
}


