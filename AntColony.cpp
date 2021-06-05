#include <utility>
#include <vector>
#include <cmath>
#include <time.h>
#include <random>
#include <algorithm>



struct ant // структура муравей для того, чтобы все удобнее хранить
{
	std::vector <bool> TabooList; // пройденные вершины в качестве булевой маски
	std::vector <int> trace = {}; // путь
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

struct edge // структура для ребра
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


void Refresh(std::vector <ant> &Colony, int n) // расставляет муравьев на стартовые позиции
{
	std::vector <ant> temp;
	for (int i = 0; i < n; i++)
		temp.push_back(ant(i, n));
	Colony = temp;
}


void NextPosition(ant &Ant, int alpha, int beta, std::vector <std::vector <edge>> &M) // определяет, куда идти муравью дальше
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
	for (int i = 0; i < n; i++) // рассчет вероятностей для каждого из возможных ребер кандидатов
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
	for (int i = 0; i < Vector.size(); i++) // выбор одного из ребер
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
	
	double CurrentPheromoneLevel = 0.2; // константа, определяющая начальный уровень феромонов на каждом из ребер
	double CurrentEvaporation = 0.6; // константа, определяющая процент испарения феромонов для каждого ребра в конце итерации цикла
	int alpha = 2; // приоритетность уровня феромонов
	int beta = 1; // приоритетность длины пути
	// альфа и бера используются в формуле вычисления вероятности для выбора следующего ребра на пути муравья
	std::vector <std::pair <std::vector <int>, int>> reses;
	for (int i = 0; i < n; i++) // заполнение изнчальной матрицы смежности
	{
		temp = {};
		for (int j = 0; j < n; j++)
			temp.push_back(edge(CurrentPheromoneLevel, CurrentEvaporation, Mprev[i][j]));
		M.push_back(temp);
	}
	std::vector <ant> Colony;
	for (int i = 0; i < n; i++)
		Colony.push_back(ant(i, n));

	int Iterations = n * n + 100; // количество итераций цикла
	while (Iterations--)
	{
		Refresh(Colony, n);
		for (auto& Ant : Colony) // для каждого муравья
		{
			for (int j = 0; j < n - 1; j++) // выбор следующей вершины
			{
				NextPosition(Ant, alpha, beta, M);
			}
			Ant.TabooList[Ant.StartLocation] = false;
			NextPosition(Ant, alpha, beta, M); // завершение маршрута данного муравья
			std::vector <int> route = Ant.trace, final_route;
			int start;
			route.pop_back();
			for (int i = 0; i < route.size(); i++) // преобразование пути к виду, где он начинается с нуля
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

	sort(reses.begin(), reses.end(), Cmp); // сортировка массива результатов по убыванию
	std::pair <std::vector <int>, int> res = reses[0]; // выбор лучшего пути
	res.second = 0;
	for (int i = 0; i < res.first.size() - 1; i++)
		res.second += M[res.first[i]][res.first[i + 1]].dist;

	return(res);
}