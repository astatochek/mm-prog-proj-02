#include <utility>
#include <vector>
#include <cmath>
#include <time.h>
#include <random>
#include <algorithm>



struct ant // ��������� ������� ��� ����, ����� ��� ������� �������
{
	std::vector <bool> TabooList; // ���������� ������� � �������� ������� �����
	std::vector <int> trace = {}; // ����
	int Location;
	int StartLocation;
	ant(int CurrentLocation, int n)
	{
		std::vector <bool> temp(n, false);
		TabooList = temp;
		Location = CurrentLocation;
		StartLocation = CurrentLocation;
		TabooList[CurrentLocation] = true;
		trace.push_back(Location);
	}

};

struct edge // ��������� ��� �����
{
	int dist;
	double PheromoneLevel;
	double Evaporation;
	double BonusPheromones;
	edge(double CurrentPheromoneLevel, double CurrentEvapoartion, int Mdist)
	{
		dist = Mdist;
		PheromoneLevel = CurrentPheromoneLevel;
		Evaporation = CurrentEvapoartion;
		BonusPheromones = 0;
	}
	void UpdatePheromones()
	{
		PheromoneLevel = PheromoneLevel * Evaporation + BonusPheromones;
		BonusPheromones = 0;
	}
};


void Refresh(std::vector <ant> &Colony, int n) // ����������� �������� �� ��������� �������
{
	std::vector <ant> temp;
	for (int i = 0; i < n; i++)
		temp.push_back(ant(i, n));
	Colony = temp;
}


void NextPosition(ant &Ant, int alpha, int beta, std::vector <std::vector <edge>> &M) // ����������, ���� ���� ������� ������
{
	int location = Ant.Location;
	int n = M.size();
	double sum = 0;
	for (int i = 0; i < n; i++)
	{
		if (!Ant.TabooList[i] && i != location)
			sum += pow(M[location][i].PheromoneLevel, alpha) * pow(1.0 / double(M[location][i].dist), beta);
	}
	std::vector <std::pair<int, double>> Vector;
	double addition = 0;
	for (int i = 0; i < n; i++) // ������� ������������ ��� ������� �� ��������� ����� ����������
	{
		if (!Ant.TabooList[i] && i != location)
		{
			std::pair <int, double> tmp;
			tmp.first = i;
			tmp.second = pow(M[location][i].PheromoneLevel, alpha) * pow(1.0 / double(M[location][i].dist), beta) / sum + addition;
			addition += pow(M[location][i].PheromoneLevel, alpha) * pow(1.0 / double(M[location][i].dist), beta) / sum;
			Vector.push_back(tmp);
		}
	}
	srand(time(0));
	double chance = double(rand() % 1000) / 1000.0;
	for (int i = 0; i < Vector.size(); i++) // ����� ������ �� �����
		if (chance <= Vector[i].second)
		{
			Ant.TabooList[Vector[i].first] = true;
			M[location][Vector[i].first].BonusPheromones += 1.0 / double(M[location][Vector[i].first].dist);
			Ant.Location = Vector[i].first;
			Ant.trace.push_back(Vector[i].first);
			break;
		}

}


bool Cmp(std::pair <std::vector <int>, int> a, std::pair <std::vector <int>, int> b) { return a.second > b.second; }

std::pair <std::vector<int>, int> ant_colony(std::vector <std::vector<int>> Mprev)
{
	std::vector <std::vector <edge>> M;
	std::vector <edge> temp;
	int n = Mprev.size();
	
	double CurrentPheromoneLevel = 0.2; // ���������, ������������ ��������� ������� ��������� �� ������ �� �����
	double CurrentEvaporation = 0.6; // ���������, ������������ ������� ��������� ��������� ��� ������� ����� � ����� �������� �����
	int alpha = 2; // �������������� ������ ���������
	int beta = 1; // �������������� ����� ����
	// ����� � ���� ������������ � ������� ���������� ����������� ��� ������ ���������� ����� �� ���� �������
	std::vector <std::pair <std::vector <int>, int>> reses;
	for (int i = 0; i < n; i++) // ���������� ���������� ������� ���������
	{
		temp = {};
		for (int j = 0; j < n; j++)
			temp.push_back(edge(CurrentPheromoneLevel, CurrentEvaporation, Mprev[i][j]));
		M.push_back(temp);
	}
	std::vector <ant> Colony;
	for (int i = 0; i < n; i++)
		Colony.push_back(ant(i, n));

	int Iterations = n * n + 100; // ���������� �������� �����
	while (Iterations--)
	{
		Refresh(Colony, n);
		for (auto& Ant : Colony) // ��� ������� �������
		{
			for (int j = 0; j < n - 1; j++) // ����� ��������� �������
			{
				NextPosition(Ant, alpha, beta, M);
			}
			Ant.TabooList[Ant.StartLocation] = false;
			NextPosition(Ant, alpha, beta, M); // ���������� �������� ������� �������
			std::vector <int> route = Ant.trace, final_route;
			int start;
			route.pop_back();
			for (int i = 0; i < route.size(); i++) // �������������� ���� � ����, ��� �� ���������� � ����
			{
				if (route[i] == 0)
				{
					start = i;
					break;
				}
			}
			final_route.push_back(0);
			for (int i = (start + 1) % route.size(); i != start; i = (i + 1) % route.size())
				final_route.push_back(route[i]);
			final_route.push_back(0);
			bool flag = true;
			for (auto& res : reses)
			{
				if (res.first == final_route)
				{
					flag = false;
					res.second++;
					break;
				}
			}
			if (flag)
				reses.push_back(std::pair <std::vector<int>, int>(final_route, 1));
		}
		for (int i = 0; i < n; i++)
			for (int j = 0; j < n; j++)
				M[i][j].UpdatePheromones();
	}

	sort(reses.begin(), reses.end(), Cmp); // ���������� ������� ����������� �� ��������
	std::pair <std::vector <int>, int> res = reses[0]; // ����� ������� ����
	res.second = 0;
	for (int i = 0; i < res.first.size() - 1; i++)
		res.second += M[res.first[i]][res.first[i + 1]].dist;

	return(res);
}