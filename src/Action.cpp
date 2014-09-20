/*
 * Action.cpp
 *
 */

#include "Action.h"
#include <iostream>
#include <fstream>

using namespace std;


void Action::saveToFile(ofstream &file) const {
	file << "ACTION type=" << type << " val=" << value << " duration=" << duration << endl;
}


