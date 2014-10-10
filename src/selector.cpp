/*
 * selector.cpp
 *
 *  Created on: Oct 9, 2014
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
#include <iomanip>
#include <queue>

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

struct guy {
	public:
		string id;
		double score;

		bool operator<(const guy& other) const {
			return other.score > score;
		}
};

class CompareScore {
	public:
		bool operator()(guy& a, guy& b) {
			return a.score > b.score;
		}
};

int tournament(vector<guy> &pop, int start, int end, int n,
		vector<string> &chosen, vector<string> &killed) {
	priority_queue<guy, vector<guy>, CompareScore> q;

	map<string, guy> m;

	int realN = std::min(end - start + 1, n);

	while (m.size() < realN) {
		guy g = pop[start + rand() % (end - start + 1)];
		m.insert(pair<string, guy>(g.id, g));
	}

	for (map<string, guy>::iterator it = m.begin(); it != m.end(); ++it) {
		q.push(it->second);
	}

	int r = q.size();

	if (!r) {
		return 0;
	}

	chosen.push_back(q.top().id);
	cout << "Winner: " << q.top().id << ", score " << q.top().score << endl;
	q.pop();

	while (!q.empty()) {
		killed.push_back(q.top().id);
		cout << "Lost: " << q.top().id << ", score " << q.top().score << endl;
		q.pop();
	}

	return r;
}

int main(int argc, char **argv) {
	srand(time(0));

	int popNum;
	int indCount;

	fstream simdata;
	simdata.open("simdata.txt");
	simdata >> popNum >> indCount;
	simdata.close();

	cout << "População: " << popNum << endl;
	cout << "Numero de individuos: " << indCount << endl;

	stringstream popOutName;

	popOutName << "popstat." << setw(5) << setfill('0') << popNum << ".txt";

	simdata.open("simdata.txt");
	simdata << (popNum + 1) << ' ' << indCount;
	simdata.close();

	vector<guy> pop;

	ifstream read;
	read.open("stats.txt");

	Robot r(nullptr, nullptr, nullptr);

	map<string, RobotDescriptor*> descs;

	do {
		string id;
		double score;
		int stall;
		double duration;

		read >> id >> score >> stall >> duration;

		if (!id.length()) {
			break;
		}

		guy g;
		g.id = id;
		g.score = score;

		pop.push_back(g);

		RobotDescriptor *descriptor = new RobotDescriptor();

		stringstream fileName;
		fileName << "robots/desc" << id;

		cout << "READING FILE " << fileName.str() << endl;
		ifstream inputFile;
		inputFile.open(fileName.str());
		descriptor->loadFromFile(inputFile, &r);
		inputFile.close();

		descs.insert(pair<string, RobotDescriptor*>(id, descriptor));
	} while (!read.eof());

	sort(pop.begin(), pop.end());

	ofstream popOut;
	popOut.open(popOutName.str());

	for (int i = 0; i < pop.size(); i++) {
		popOut << pop[i].score << '\n';
	}

	int last = 0;
	for (int i = 0; i < 400; i += 10) {
		int c = 0;

		for (int j = last; j < pop.size(); j++) {
			if (pop[j].score > i) {
				break;
			}

			last = j;
			c++;
		}

		popOut << i << '\t' << c << '\n';
	}

	popOut.close();

	map<string, RobotDescriptor*> newPop;

	int numDIV = 20;
	double divAmmount = pop.size() / ((double) numDIV);

	vector<string> killed;
	vector<string> chosen;
	for (int i = 0; i < numDIV; i++) {
		int start = divAmmount * i;
		int end = start + divAmmount - 1;

		start = std::min(start, (int) pop.size() - 1);
		end = std::min(end, (int) pop.size() - 1);

		cout << "Tournament from " << start << " to " << end << endl;

		tournament(pop, start, end, 4, chosen, killed);
	}

	cout << killed.size() << " killed, " << chosen.size() << " chosen" << endl;

	for (int j = 0; j < chosen.size(); j++) {
		string id = chosen[j];

		for (vector<guy>::iterator it = pop.begin(); it != pop.end(); ++it) {
			if (it->id == id) {
				cout << "choosing " << id << ", score: " << it->score << endl;
				newPop.insert(pair<string, RobotDescriptor*>(id, descs[id]));
				pop.erase(it);
				break;
			}
		}
	}

	for (int j = 0; j < killed.size(); j++) {
		string id = killed[j];

		for (vector<guy>::iterator it = pop.begin(); it != pop.end(); ++it) {
			if (it->id == id) {
				pop.erase(it);
				break;
			}
		}
	}

	cout << endl;

	ofstream output;
	output.open("generated.txt");

	int k = 0;
	while (newPop.size() < indCount) {
		string parentID = chosen[rand() % (chosen.size())];
		RobotDescriptor *parent = descs[parentID];
		RobotDescriptor *ind = new RobotDescriptor();

		stringstream fileName;
		fileName << "robots/desc" << parentID;
		ifstream inParent;
		inParent.open(fileName.str());
		ind->loadFromFile(inParent, nullptr);
		inParent.close();

		for (int i = 0; i < ind->behaviours.size(); i++) {
			BehaviourOnObstacleDistance *b =
					static_cast<BehaviourOnObstacleDistance*>(ind->behaviours[i]);
			b->angle += rand()%5 - 2;
			b->angle = std::min(std::max(b->angle, 360.0), 0.0);
			b->distanceMax += (rand()%20)/100.0 - 0.1;
			b->distanceMin += (rand()%20)/100.0 - 0.1;

			for(int j = 0; j < b->actions.size(); j++){
				Action *a = b->actions[j];

				a->duration += (rand()%20)/100.0 - 0.1;
				a->duration = std::max(0.0, a->duration);

				a->value += (rand()%20)/100.0 - 0.1;
			}
		}

		stringstream generatedID;
		generatedID << setw(5) << setfill('0') << popNum << '.' << setw(5) << setfill('0') << k;

		cout << "New ID: " << generatedID.str() << endl;

		stringstream generatedFile;
		generatedFile << "robots/desc" << generatedID.str();

		cout << "New File: " << generatedFile.str() << endl;

		ofstream saveFile;
		saveFile.open(generatedFile.str());
		ind->saveToFile(saveFile);
		saveFile.close();

		newPop.insert(pair<string, RobotDescriptor*>(generatedID.str(), ind));

		k++;

	}

	for(map<string, RobotDescriptor*>::iterator it = newPop.begin(); it != newPop.end(); ++it){
		output << it->first << '\n';
	}

	output.close();

	return 0;
}

