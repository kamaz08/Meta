struct Vertex {
	double X;
	double Y;
};


class Graph {
private:
	int NumberVertex;
	Vertex * VertexTab;
	double ** EdgeTab;
	//int * FirstSolution;
	int ** Parent;
	int NumberOfParent;
	int NumberOfStep;

public:
	Graph(int n, int numberOfParent=512);

	void AddEdge(int id, double x, double y) {
		VertexTab[id].X = x;
		VertexTab[id].Y = y;
	}

	void ComputeFirstSolution();
	void ComputeDistance();
	int* GetSolution() {
		return Parent[0];
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
	void ComputeRandomParent();
	
	void MutableParent(int id, int step);

	void Test() {
		std::cout << "taak";
		for (int i = 0; i < NumberOfParent; i++) {
			GetWeight(Parent[i]);
		}
		std::cout << "koniec";
	}


	void SimulatedAnnealingRandom(int testCount);

	void GeneticAlgorithm();

	void SelectBetter(int p);

	void GenerateParent();
};
