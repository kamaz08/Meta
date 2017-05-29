#include <ctime>
#include <cmath>
#include <cstdlib>
#include <cfloat>
#include "Graph.h"

//Tabu
void TabuList::NextStep() {
	Tabu* ptr = First;
	while (ptr != NULL) {
		--WaitingList[ptr->Id];
		ptr = ptr->next;
	}
	ClearZero();
}

void TabuList::ClearZero() {
	Tabu* ptr = First;
	while (ptr != NULL && WaitingList[ptr->Id] == 0) {
		First = ptr->next;
		Size--;
		delete ptr;
		ptr = First;
	}
}

void TabuList::AddTabu(int id) {
	Tabu* ptr = new Tabu();
	ptr->Id = id;
	ptr->next = NULL;
	if (Size == 0) {
		First = ptr;
	}
	else {
		Last->next = ptr;
	}
	Last = ptr;
	Size++;
	WaitingList[id] = WaitTime;
}

TabuList::TabuList(short maxSize, short waitngTime, int n) {
	MaxSize = maxSize;
	Size = 0;
	WaitTime = waitngTime;
	First = NULL;
	Last = NULL;
	WaitingList = new short[n];
	for (int i = 0; i < n; i++) {
		WaitingList[i] = 0;
	}
}

void TabuList::UpdateTabu(short id0, short id1) {
	NextStep();
	for (Size; Size > MaxSize - 2; Size--) {
		Tabu* ptr = First;
		First = ptr->next;
		WaitingList[ptr->Id] = 0;
		delete ptr;
	}
	AddTabu(id0);
	AddTabu(id1);
}

//Graph
Graph::Graph(int n) {
	NumberVertex = n;
	VertexTab = new Vertex[n];
	EdgeTab = new double*[n - 1];
	FirstSolution = new int[n];
	for (int i = 1; i < n; i++) {
		EdgeTab[i - 1] = new double[n - i];
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
		FirstSolution[i] = minId;
		Visited[minId] = true;
		ptr = minId;
	}
	FirstSolution[NumberVertex - 1] = 0;
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

void Graph::TabuSearchAll(int testCount) {
	TabuList * tabuList = new TabuList(NumberVertex / 3, testCount / 3 + 1, NumberVertex);
	int * currentTab = new int[NumberVertex];
	for (int i = 0; i < NumberVertex; i++) {
		currentTab[i] = FirstSolution[i];
	}
	for (int i = 0; i < testCount; i++) {
		double diff = DBL_MAX;
		short id0 = 0, id1 = 0;
		for (int j = 0; j < NumberVertex - 2; j++) {
			for (int k = j+1; k < NumberVertex - 1; k++) {
				if (tabuList->IsInTabu(j)) break;
				if (tabuList->IsInTabu(k)) continue;
				double actual = GetCurrentWeight(currentTab, j, k);
				double changed = GetChangedWeight(currentTab, j, k);
				if (changed < actual) {
					double diffTemp = actual - changed;
					if (diffTemp < diff) {
						diff = diffTemp;
						id0 = j, id1 = k;
					}
				}
			}
		}
		if (!(id0 == id1)) {
			Swap(currentTab, id0, id1);
			tabuList->UpdateTabu(id0, id1);
		}
		else {
			delete[] FirstSolution;
			FirstSolution = currentTab;
			return;
		}
	}
}
void Graph::TabuSearchRandom(int testCount, int randomCount) {
	TabuList * tabuList = new TabuList(NumberVertex / 3, testCount / 3 + 1, NumberVertex);
	int * currentTab = new int[NumberVertex];
	for (int i = 0; i < NumberVertex; i++) {
		currentTab[i] = FirstSolution[i];
	}

	srand((unsigned int)time(NULL));
	for (int i = 0; i < testCount; i++) {
		double diff = DBL_MAX;
		short id0 = 0, id1 = 0;
		for (int j = 0; j < randomCount; j++) {
			int t1, t0 = rand() % (NumberVertex - 1);
			while (t0 == (t1 = rand() % (NumberVertex - 1)));
			if (tabuList->IsInTabu(t0) || tabuList->IsInTabu(t1)) continue;
			double actual = GetCurrentWeight(currentTab, t0, t1);
			double changed = GetChangedWeight(currentTab, t0, t1);
			
			if (changed < actual) {
				double diffTemp = actual - changed;
				if (diffTemp < diff) {
					diff = diffTemp;
					id0 = t0, id1 = t1;
				}
			}
		}
		if (!(id0 == id1)) {
			Swap(currentTab, id0, id1);
			tabuList->UpdateTabu(id0, id1);
		}
	}
	delete[] FirstSolution;
	FirstSolution = currentTab;
}
