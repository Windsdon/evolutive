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
	q.pop();

	while (!q.empty()) {
		killed.push_back(q.top().id);
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
	map<string, guy> popmap;

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

		popmap.insert(pair<string, guy>(g.id, g));

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

	int last = 0;
	for (int i = 0; i < 400; i += 10) {
		int c = 0;

		bool broken = false;
		for (int j = last; j < pop.size(); j++) {
			if (pop[j].score > i) {
				last++;
				broken = true;
				break;
			}

			last = j;
			c++;
		}

		broken = true;

		popOut << i << '\t' << c << '\n';
	}

//	for (int toursize = 2; toursize <= 64; toursize *= 2) {
//		vector<guy> newpop;
//		map<string, guy> mapunique;
//
//		cout << "Torneio de " << toursize << endl;
//
//		while (newpop.size() != indCount) {
//			vector<string> killed;
//			vector<string> chosen;
//			tournament(pop, 0, pop.size() - 1, toursize, chosen, killed);
//
//			newpop.push_back(popmap[chosen[0]]);
//			mapunique.insert(pair<string, guy>(chosen[0], popmap[chosen[0]]));
//		}
//
//		vector<guy> popunique;
//
//		for (map<string, guy>::iterator it = mapunique.begin();
//				it != mapunique.end(); ++it) {
//			popunique.push_back(it->second);
//		}
//
//		cout << "Total selecionado: " << newpop.size() << ", unicos: "
//				<< popunique.size() << endl;
//
//		sort(newpop.begin(), newpop.end());
//		sort(popunique.begin(), popunique.end());
//
//		stringstream k;
//		k << "tournament." << toursize << ".txt";
//
//		ofstream popOut(k.str());
//
//		popOut << "All:\n";
//
//		int total = 0;
//
//		int last = 0;
//		for (int i = 0; i <= 400; i += 10) {
//			int c = 0;
//
//			bool broken = false;
//			for (int j = last; j < newpop.size(); j++) {
//
//				if (newpop[j].score > i) {
//					last++;
//					broken = true;
//					break;
//				}
//
//				last = j;
//				c++;
//			}
//
//			if (!broken) {
//				last++;
//			}
//
//			total += c;
//
//			popOut << i << '\t' << c << '\n';
//		}
//
//		cout << "Output: " << total << endl;
//
//		popOut << "Unique:\n";
//
//		last = 0;
//		for (int i = 0; i < 400; i += 10) {
//			int c = 0;
//
//			bool broken = false;
//			for (int j = last; j < popunique.size(); j++) {
//				if (popunique[j].score > i) {
//					last++;
//					break;
//				}
//
//				last = j;
//				c++;
//			}
//			if (!broken) {
//				last++;
//			}
//
//			popOut << i << '\t' << c << '\n';
//		}
//
//		popOut.close();
//
//	}
//
//	return 0;

	map<string, RobotDescriptor*> newPop;

	vector<guy> newpop;
	map<string, guy> mapunique;
	while (newpop.size() != indCount) {
		vector<string> killed;
		vector<string> chosen;
		tournament(pop, 0, pop.size() - 1, 4, chosen, killed);

		newpop.push_back(popmap[chosen[0]]);
		mapunique.insert(pair<string, guy>(chosen[0], popmap[chosen[0]]));
	}

	vector<guy> popunique;

	for (map<string, guy>::iterator it = mapunique.begin();
			it != mapunique.end(); ++it) {
		popunique.push_back(it->second);
	}

	sort(newpop.begin(), newpop.end());
	sort(popunique.begin(), popunique.end());

	popOut << "All:\n";

	last = 0;
	for (int i = 0; i < 400; i += 10) {
		int c = 0;

		bool broken = false;
		for (int j = last; j < newpop.size(); j++) {
			if (newpop[j].score > i) {
				last++;
				broken = true;
				break;
			}

			last = j;
			c++;
		}

		if (!broken) {
			last++;
		}

		popOut << i << '\t' << c << '\n';
	}

	popOut << "Unique:\n";

	last = 0;
	for (int i = 0; i < 400; i += 10) {
		int c = 0;

		bool broken = false;
		for (int j = last; j < popunique.size(); j++) {
			if (popunique[j].score > i) {
				last++;
				broken = true;
				break;
			}

			last = j;
			c++;
		}

		if (!broken) {
			last++;
		}

		popOut << i << '\t' << c << '\n';
	}

	popOut << newpop.size() << " chosen, " << popunique.size() << " unique\n";

	for (int i = 0; i < pop.size(); i++) {
		popOut << pop[i].score << '\n';
	}

	popOut.close();

	for (int j = 0; j < popunique.size(); j++) {
		string id = popunique[j].id;
		cout << "choosing " << id << ", score: " << popunique[j].score << endl;
		newPop.insert(pair<string, RobotDescriptor*>(id, descs[id]));
	}

	cout << endl;

	ofstream output;
	output.open("generated.txt");

	int k = 0;
	while (newPop.size() < indCount) {
		string parentID = popunique[rand() % (popunique.size())].id;
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
			if (rand() % 100 <= 5) {
				b->angle += rand() % 5 - 2;
			}
			if (rand() % 100 <= 5) {
				b->angle = std::min(std::max(b->angle, 360.0), 0.0);
			}
			if (rand() % 100 <= 5) {
				b->distanceMax += (rand() % 20) / 100.0 - 0.1;
			}
			if (rand() % 100 <= 5) {
				b->distanceMin += (rand() % 20) / 100.0 - 0.1;
			}

			for (int j = 0; j < b->actions.size(); j++) {
				Action *a = b->actions[j];

				if (rand() % 100 <= 5) {
					a->duration += (rand() % 20) / 100.0 - 0.1;
				}
				if (rand() % 100 <= 5) {
					a->duration = std::max(0.0, a->duration);
				}

				if (rand() % 100 <= 5) {
					a->value += (rand() % 20) / 100.0 - 0.1;
				}
			}
		}

		stringstream generatedID;
		generatedID << setw(5) << setfill('0') << popNum << '.' << setw(5)
				<< setfill('0') << k;

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

	for (map<string, RobotDescriptor*>::iterator it = newPop.begin();
			it != newPop.end(); ++it) {
		output << it->first << '\n';
	}

	output.close();

	return 0;
}

