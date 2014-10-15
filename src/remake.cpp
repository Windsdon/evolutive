/*
 * remake.cpp
 *
 *  Created on: Oct 10, 2014
 *      Author: suporte
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include <vector>

using namespace std;

int main(int argc, char **argv) {
	vector<string> generated;
	set<string> simulated;
	vector<string> regenerate;

	ifstream in;


	in.open("stats.txt");

	while (!in.eof()) {
		string line;
		getline(in, line);

		stringstream ss(line);
		string id;
		ss >> id;

		if(id.size() <= 1){
			continue;
		}

		simulated.insert(id);
	}

	in.close();

	in.open("generated.txt");

	while (!in.eof()) {
		string id;
		in >> id;

		if(id.size() <= 1){
			continue;
		}

		if(simulated.find(id) == simulated.end()){
			regenerate.push_back(id);
		}
	}

	in.close();

	if(argc == 1){
		for(int i = 0; i < regenerate.size(); i++){
			cout << regenerate[i] << endl;
		}
	}

	if(regenerate.size()){
		return 1;
	}else{
		return 0;
	}

}

