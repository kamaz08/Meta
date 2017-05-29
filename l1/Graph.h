#pragma once

struct Vertex {
	double X;
	double Y;
};


struct Tabu {
	short Id;
	Tabu* next;
};

class TabuList {
private:
	short MaxSize;
	short Size;
	short WaitTime;
	Tabu* First;
	Tabu* Last;
	short* WaitingList;

	void NextStep();
	void ClearZero();
	void AddTabu(int id);

public:
	TabuList(short maxSize, short waitngTime, int n);
	void UpdateTabu(short id0, short id1);
	bool IsInTabu(short id) {
		return WaitingList[id] != 0;
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
	void TabuSearchRandom(int testCount, int randomCount);
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
	void TabuSearchAll(int testCount);
};