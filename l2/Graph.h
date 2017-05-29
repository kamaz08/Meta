#pragma once
#include <cmath>
struct Vertex {
	double X;
	double Y;
};

class SimulatedAnnealing {
private:
	int NumberIteration;
	int Precentage;
	double T;
	const double e;
public:
	SimulatedAnnealing(double averageFirst, int numberTest, int numberVertex) : T(averageFirst/numberVertex), NumberIteration(1), e(2.71828182846), Precentage(numberTest / 100.0) {}
	double GetPropablity(double oldS, double newS) {
		return pow(e, -(newS - oldS) / T );
	}
	void NextIter() {
		NumberIteration+=1;
		if (NumberIteration % Precentage == 0 ) {
			T *=0.87;
		}
	}
};



class Graph {
private:
	int NumberVertex;
	Vertex * VertexTab;
	double ** EdgeTab;
	int * FirstSolution;

public:
	Graph(int n);

	void AddEdge(int id, double x, double y) {
		VertexTab[id].X = x;
		VertexTab[id].Y = y;
	}

	void ComputeFirstSolution();
	void ComputeDistance();
	int* GetFirstSolution() {
		return FirstSolution;
	}
	double GetDistance(int id0, int id1) {
		if (id0 > id1) {
			int temp = id0;
			id0 = id1;
			id1 = temp;
		}
		return EdgeTab[id0][id1 - id0 - 1];
	}
	double GetWeight(int * tab);
	int GetLeft(int id) {
		return  id == 0 ? NumberVertex - 1 : id - 1;
	}
	int GetRight(int id) {
		return id == NumberVertex - 1 ? 0 : id + 1;
	}
	double GetCurrentWeight(int *tab, int pos0, int pos1) {
		return GetDistance(tab[pos0], tab[GetLeft(pos0)])
			+ GetDistance(tab[GetRight(pos0)], tab[pos0])
			+ GetDistance(tab[pos1], tab[GetLeft(pos1)])
			+ GetDistance(tab[GetRight(pos1)], tab[pos1]);
	}
	double GetChangedWeight(int *tab, int pos0, int pos1) {
		Swap(tab, pos0, pos1);
		double result = GetCurrentWeight(tab, pos0, pos1);
		Swap(tab, pos0, pos1);
		return result;
	}
	void Swap(int *tab, int pos0, int pos1) {
		int temp = tab[pos0];
		tab[pos0] = tab[pos1];
		tab[pos1] = temp;
	}
	void SimulatedAnnealingRandom(int testCount);
};
