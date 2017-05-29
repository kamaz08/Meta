#include <iostream>
#include <ctime>
#include "Graph.h"

using namespace std;

int main(int c, char **v) {
	int n, temp;
	cin >> n;
	Graph *graph = new Graph(n);
	double x, y;
	for (int i = 0; i < n; i++) {
		cin >> temp >> x >> y;
		graph->AddEdge(temp-1, x, y);
	}
	graph->ComputeDistance();
	graph->ComputeFirstSolution();
	int * result = graph->GetFirstSolution();

	graph->SimulatedAnnealingRandom(100000000);
	result = graph->GetFirstSolution();
	cout << graph->GetWeight(result)<<endl;

	cerr << 1 << " ";
	for (int i = 0; i < n; i++) {
		cerr << result[i]+1<< " ";
	}
}
