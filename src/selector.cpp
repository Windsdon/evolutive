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

// Tamanho do torneio
#define SIZE 2
// Porcentagem da população gerada por cross over
// Para fazer uma população só com mutação, coloque em 0
#define TAXA_CROSS 0.7

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

/*
 * Funcionamento:
 * organiza data nas ranges divs[i] <= k < divs[i+1]
 */
vector<int> generateHistogram(const vector<double> &divs,
		const vector<double> &data) {
	vector<int> r(divs.size(), 0);

	for (int i = 0; i < data.size(); i++) {
		int minr;
		for (minr = 0; minr < divs.size(); minr++) {
			if (data[i] < divs[minr]) {
				break;
			}
		}
		if (minr) {
			minr--;
			r[minr]++;
		}
	}

	return r;
}

double average(const vector<double> &v) {
	double sum = 0;
	for (int i = 0; i < v.size(); i++) {
		sum += v[i];
	}

	return sum / (v.size());
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

	map<string, RobotDescriptor*> newPop;

	vector<guy> newpop;
	map<string, guy> mapunique;
	while (newpop.size() != indCount) {
		vector<string> killed;
		vector<string> chosen;
		tournament(pop, 0, pop.size() - 1, SIZE, chosen, killed);

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

	vector<double> divs;
	vector<double> oldPopScores;
	vector<double> newPopScores;
	vector<double> newPopScoresUnique;

	for (int i = 0; i < 400; i += 10) {
		divs.push_back(i);
	}

	for (int i = 0; i < pop.size(); i++) {
		oldPopScores.push_back(pop[i].score);
	}

	for (int i = 0; i < newpop.size(); i++) {
		newPopScores.push_back(newpop[i].score);
	}

	for (int i = 0; i < popunique.size(); i++) {
		newPopScoresUnique.push_back(popunique[i].score);
	}

	vector<int> oldPopStats = generateHistogram(divs, oldPopScores);
	vector<int> newPopStats = generateHistogram(divs, newPopScores);
	vector<int> newPopStatsUnique = generateHistogram(divs, newPopScoresUnique);

	popOut << newpop.size() << " chosen, " << popunique.size() << " unique\n";
	popOut << "range\told\tnew\tunique\n";

	for (int i = 0; i < divs.size(); i++) {
		popOut << divs[i] << '\t' << oldPopStats[i] << '\t' << newPopStats[i]
				<< '\t' << newPopStatsUnique[i] << '\n';
	}

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

	if (argc > 1) {
		return 0;
	}

	ofstream output;
	output.open("generated.txt");
	
	int ammountCross = TAXA_CROSS * (indCount - newPop.size());

	for (int i = 0; i < ammountCross; i++) {
		string parent1 = popunique[rand() % (popunique.size())].id;
		string parent2 = popunique[rand() % (popunique.size())].id;
		RobotDescriptor *p1 = descs[parent1];
		RobotDescriptor *p2 = descs[parent2];

		double point = (rand() % 10) / 10.0;

		if (p1->behaviours.size() < 3 || p2->behaviours.size() < 3) {
			continue;
		}

		RobotDescriptor *ind = new RobotDescriptor();

		int cut1 = std::min(std::max(1, (int) (point * p1->behaviours.size())),
				(int) p1->behaviours.size() - 2);
		int cut2 = std::min(std::max(1, (int) (point * p2->behaviours.size())),
				(int) p2->behaviours.size() - 2);

		//@bug Referência ao descritor pai é mantida!
		for(int i = 0; i < cut1; i++){
			ind->addBehavior(p1->behaviours[i]);
		}
		for(int i = cut2; i < p2->behaviours.size(); i++){
			ind->addBehavior(p2->behaviours[i]);
		}
	}

	//Mutação!
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
				b->angle = std::max(std::min(b->angle, 360.0), 0.0);
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

