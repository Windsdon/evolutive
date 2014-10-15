/*
 * RobotDescriptor.cpp
 */

#include "RobotDescriptor.h"
#include <string>

#include <sstream>
#include <utility>

#include <stack>
#include <map>

void RobotDescriptor::saveToFile(ofstream &file) const {
	unsigned int numBehaviours = behaviours.size();
	file << "DESCRIPTOR" << endl;
	file << "LIST " << numBehaviours << endl;

	for (unsigned int i = 0; i < numBehaviours; i++) {
		behaviours[i]->saveToFile(file);
	}
}

std::vector<std::string> explode(std::string const & s, char delim) {
	std::vector<std::string> result;
	std::istringstream iss(s);

	for (std::string token; std::getline(iss, token, delim);) {
		result.push_back(std::move(token));
	}

	return result;
}

void makeParam(vector<string> chunks, map<string, double> &params) {
	for (unsigned int i = 1; i < chunks.size(); i++) {
		vector<string> paramExplode = explode(chunks[i], '=');

		istringstream numberiss(paramExplode[1]);
		double n;
		numberiss >> n;

		params.insert(pair<string, double>(paramExplode[0], n));

		//cout << "Read param " << paramExplode[0] << ", value: " << n << endl;
	}
}

void RobotDescriptor::loadFromFile(ifstream &file, Robot* robot) {
	string line;
	getline(file, line);
	getline(file, line);

	//cout << "Read line " << line << endl;

	vector<string> chunks1 = explode(line, ' ');
	istringstream numberiss(chunks1[1]);
	int clist;
	numberiss >> clist;

	//cout << "Reading " << clist << " behaviours" << endl;

	for (int i = 0; i < clist; i++) {
		getline(file, line);
		//cout << "Read line " << line << endl;
		vector<string> chunks2 = explode(line, ' ');
		map<string, double> params;
		makeParam(chunks2, params);
		Behaviour *b = new BehaviourOnObstacleDistance(robot, params);

		//cout << "Made behaviour: ";
		//b->print(cout);

		getline(file, line);
		//cout << "Read line " << line << endl;
		vector<string> chunks3 = explode(line, ' ');
		istringstream numberiss2(chunks3[1]);
		int clist2;
		numberiss2 >> clist2;
		//cout << "Reading " << clist2 << " actions" << endl;
		for (int j = 0; j < clist2; j++) {
			getline(file, line);
			vector<string> chunks4 = explode(line, ' ');
			map<string, double> params2;
			makeParam(chunks4, params2);
			Action *a = new Action(params2);
			//cout << "made action: ";
			//a->print(cout);
			//cout << endl;
			b->addAction(a);
		}

		//cout << "DONE READING ACTIONS" << endl;

		addBehavior(b);

		//cout << "ADDED BEHAVIOUR" << endl;
	}
}

