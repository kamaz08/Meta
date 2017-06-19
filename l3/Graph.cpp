#include <ctime>
#include <iostream>
#include <cstdlib>
#include <cfloat>
#include <list>
#include <cmath>
#include <algorithm>
#include "Graph.h"


//Graph
Graph::Graph(int n, int numberOfParent) {
	srand((unsigned int)time(NULL));
	NumberVertex = n;
	NumberOfParent = numberOfParent;
	VertexTab = new Vertex[n];
	EdgeTab = new double*[n - 1];
	NumberOfStep = (int) log2(numberOfParent);

	for (int i = 1; i < n; i++) {
		EdgeTab[i - 1] = new double[n - i];
	}

	Parent = new int*[NumberOfParent];
	for (int i = 0; i < NumberOfParent; i++) {
		Parent[i] = new int[n];
	}
}

void Graph::ComputeFirstSolution() {
	bool* Visited = new bool[NumberVertex];
	Visited[0] = true;
	for (int i = 1; i < NumberVertex; i++) {
		Visited[i] = false;
	}
	int ptr = 0;
	for (int i = 0; i < NumberVertex - 1; i++) {
		double minimum = DBL_MAX;
		int minId = -1;
		for (int j = 0; j < NumberVertex; j++) {
			if (!Visited[j] && ptr != j) {
				double temp = GetDistance(ptr, j);
				if (temp < minimum) {
					minimum = temp;
					minId = j;
				}
			}
		}
		Parent[0][i] = minId;
		Visited[minId] = true;
		ptr = minId;
	}
	Parent[0][NumberVertex - 1] = 0;
}

void Graph::ComputeDistance() {
	for (int i = 0; i < NumberVertex - 1; i++) {
		for (int j = 0; j < NumberVertex - i - 1; j++) {
			double temp = sqrt(pow(VertexTab[i].X - VertexTab[j + i + 1].X, 2.0)) + sqrt(pow(VertexTab[i].Y - VertexTab[j + i + 1].Y, 2.0));
			EdgeTab[i][j] = temp;
		}
	}
}

double Graph::GetWeight(int * tab) {
	double result = GetDistance(0, tab[0]);
	for (int i = 1; i < NumberVertex; i++) {
		result += GetDistance(tab[i - 1], tab[i]);
	}
	return result;
}


void Graph::ComputeRandomParent() {
	for (int i = 0; i < NumberVertex; i++) {
		for (int j = 1; j < NumberOfParent; j++) {
			int id0, id1;
			id0 = rand() % (NumberVertex - 1);
			while (id0 == (id1 = rand() % (NumberVertex - 1)));
			Swap(Parent[j], id0, id1);
		}
	}
}

void Graph::GenerateParent() {
	for (int i = 0; i < NumberVertex; i++) {
		for (int j = 1; j < NumberOfParent; j++) {
			Parent[j][i] = Parent[0][i];
		}
	}
}


void Graph::MutableParent(int id, int step) {
	int numberMutable = NumberVertex /  (NumberOfStep - step) + 1;

	for (int i = 0; i < numberMutable; i++) {
		int id0, id1;
		id0 = rand() % (NumberVertex - 1);
		while (id0 == (id1 = rand() % (NumberVertex - 1)));
		double actual = GetCurrentWeight(Parent[id], id0, id1);
		double changed = GetChangedWeight(Parent[id], id0, id1);
		if (actual > changed) {
			Swap(Parent[id], id0, id1);
		}
	}
}

void Graph::GeneticAlgorithm() {
	int numberAcutalParent = NumberOfParent;
	for (int i = 0; i < NumberOfStep; i++) {
		for (int j = 0; j < numberAcutalParent; j++) {
			MutableParent(j, i);
		}
		for (int j = 0; j < numberAcutalParent / 2; j++) {
			SelectBetter(j);
		}
		numberAcutalParent /= 2;
	}
}

void Graph::SelectBetter(int id) {
	int pid0 = id * 2, pid1 = id * 2 + 1;
	int id0 = rand() % (NumberVertex - 2) + 1;
	int* child0 = new int[NumberVertex];
	int* child1 = new int[NumberVertex];
	std::list<int> l0, l1;
	child0[NumberVertex - 1] = 0;
	child1[NumberVertex - 1] = 0;


	for (int i = 0; i < id0; i++) {
		l0.push_front(Parent[pid0][i]);
		l1.push_front(Parent[pid1][i]);
		child0[i] = Parent[pid0][i];
		child1[i] = Parent[pid1][i];
	}
	int count0 = 0, count1 = 0;
	for (int i = id0; i < NumberVertex - 1; i++) {
		while (std::find(l0.begin(), l0.end(), Parent[pid1][(i + count0) % (NumberVertex-1)]) != l0.end()) {
			count0++;
		}
		child0[i] = Parent[pid1][(i + count0) % (NumberVertex-1)];

		while (std::find(l1.begin(), l1.end(), Parent[pid0][(i + count1) % (NumberVertex-1)]) != l1.end()) {
			count1++;
		}
		child1[i] = Parent[pid0][(i + count1) % (NumberVertex-1)];
	}
	int* tab[4] = {Parent[pid0], Parent[pid1], child0, child1};
	int tabWaight[4] = {GetWeight(Parent[pid0]),GetWeight(Parent[pid1]),GetWeight(child0),GetWeight(child1)};

	int min = 0;

	for (int i = 1; i < 4; i++) {
		if (tabWaight[min] > tabWaight[i]) {
			delete tab[min];
			min = i;
		}
		else {
			delete tab[i];
		}
	}
	Parent[id] = tab[min];
}
