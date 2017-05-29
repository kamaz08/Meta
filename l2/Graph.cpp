#include <ctime>
#include <cmath>
#include <cstdlib>
#include <cfloat>
#include "Graph.h"

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



void Graph::SimulatedAnnealingRandom(int testCount) {
	SimulatedAnnealing* annealing = new SimulatedAnnealing(GetWeight(FirstSolution), testCount, NumberVertex);
	srand((unsigned int)time(NULL));
	for (int i = 0; i < testCount; i++) {
		double diff = DBL_MAX;

		int t1, t0 = rand() % (NumberVertex - 1);
		while (t0 == (t1 = rand() % (NumberVertex - 1)));
		
		double actual = GetCurrentWeight(FirstSolution, t0, t1);
		double changed = GetChangedWeight(FirstSolution, t0, t1);

		if (changed < actual) {
			//std::cout << 1;
			Swap(FirstSolution, t0, t1);
		}else{
			double ran = (double)rand() / (double)RAND_MAX;
			double propa = annealing->GetPropablity(actual, changed);
			if (ran != 0 && ran < propa) {
				if (actual - changed != 0) {
					//std::cout << 0;
					//std::cout << actual << " " << changed << " " << ran << " " << propa<<std::endl;
				}

				Swap(FirstSolution, t0, t1);
			}
		}
		annealing->NextIter();
	}
}
