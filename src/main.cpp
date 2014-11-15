/*
 * main.cpp
 *
 */

#include "Robot.h"
#include "Behaviour.h"
#include "Action.h"
#include <player-3.0/libplayerc++/playerc++.h>
#include <iostream>
#include <cstdlib>
#include <exception>
#include <sstream>
#include <algorithm>

#define _GXX_EXPERIMENTAL_CXX0X__
#include <chrono>

using namespace PlayerCc;
using namespace std;

void printBehaviour(ostream& left, const Behaviour& b) {
	left << "Behaviour " << &b << " ";
	b.print(left);
}

void printDescriptor(ostream& left, const RobotDescriptor &rd) {
	left << "Descriptor " << &rd << ". Enumerating behaviours:" << endl;
	for (unsigned int i = 0; i < rd.behaviours.size(); i++) {
		printBehaviour(left, *(rd.behaviours[i]));
	}
}

int main(int argc, char **argv) {
	PlayerClient *robot = nullptr;

	int at = 0;

	stringstream portConverter(argv[2]);
	int simPort;
	portConverter >> simPort;

	while (robot == nullptr) {
		try {
			robot = new PlayerClient("localhost", simPort);
		} catch (PlayerError &ex) {
			robot = nullptr;
			sleep(1);
		}

		if(at >= 20){
			return 0;
		}

		at++;
	}

	Position2dProxy pp(robot);
	LaserProxy lp(robot);

	ifstream targetData("target.txt");

	double rtx = 0;
	double rty = 0;
	double timeLimit = 60;
	
	targetData >> rtx >> rty;
	targetData.close();

	cout << "Target: " << rtx << ", " << rty << endl;

	RobotDescriptor descriptor;

	Robot r(&pp, &lp, &descriptor);

	stringstream fileName;
	fileName << "robots/desc" << argv[1];

	ifstream read;
	read.open("stats.txt");

	stringstream fileLockName;
	fileLockName << "lock." << argv[1];


	ifstream fileLockCheck(fileLockName.str());

	if(fileLockCheck.good()){
		fileLockCheck.close();
		return 0;
	}

	fileLockCheck.close();

	bool simulated = false;

	do {
		string id;
		double score;
		int stall;
		double duration;

		read >> id >> score >> stall >> duration;

		if (!id.length()) {
			break;
		}

		stringstream fileName2;
		fileName2 << "robots/desc" << id;

		if (fileName == fileName2) {
			simulated = true;
			break;
		}

	} while (!read.eof());

	if(simulated){
		return 0;
	}

	cout << "Creating LOCK" << endl;
	
	ofstream fileLock(fileLockName.str());
	fileLock << endl;
	fileLock.close();

	cout << "READING FROM FILE: " << fileName.str() << endl;

	ifstream inputFile;
	inputFile.open(fileName.str());
	descriptor.loadFromFile(inputFile, &r);
	inputFile.close();

	//printDescriptor(cout, descriptor);

	cout << "*** DESCRIPTOR READ. STARTING SIMULATION. ***" << endl;

	auto begin = chrono::high_resolution_clock::now();
	auto lastSample = chrono::high_resolution_clock::now();

	double points = 0;
	bool stall = false;
	vector<double> pl;
	double sampleRate = 1;

	while (true) {
		robot->Read();
		r.update();

		if (pp.GetStall()) {
			stall = true;
			break;
		}

		auto now = chrono::high_resolution_clock::now();
		double elapsed = (chrono::duration_cast<chrono::duration<double> >(
				now - begin)).count();

		if ((chrono::duration_cast<chrono::duration<double> >(now - lastSample)).count()
				>= sampleRate) {
			lastSample = now;
			double dist = pow(pp.GetXPos() - rtx, 2)
					+ pow(pp.GetYPos() - rty, 2);

			pl.push_back(dist);
		}

		if (elapsed > timeLimit) {
			break;
		}
	}

	sort(pl.begin(), pl.end());

	int samples = 0;

	for (unsigned int i = 0; (i < 10 && i < pl.size()); i++) {
		points += pl[i];
		samples++;
	}

	points /= samples;

	double duration = (chrono::duration_cast<chrono::duration<double> >(
			chrono::high_resolution_clock::now() - begin)).count();

	cout << "Acabou o tempo! Pontos: " << points << endl;
	cout << "Bateu: " << stall << endl;
	cout << "Duration: " << duration << endl;

	ofstream stats;
	stats.open("stats.txt", ios::app);
	stats << argv[1] << " " << points << " " << (stall ? "1" : "0") << " " << duration
			<< endl;
	stats.close();

	remove(fileLockName.str().c_str());

	return 0;
}

