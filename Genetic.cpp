#include <utility>
#include <vector>
#include <cstdlib>
#include <time.h>
#include <algorithm>
#include <chrono>
#include <random>


struct chrom
{
	std::vector <int> route;
	int dist = 0;
	chrom(std::vector <int> vec, std::vector <std::vector<int>> M) // при создании на основе пути считает его растояние
	{
		route = vec;
		dist += M[0][route[0]];
		for (int i = 0; i < route.size() - 1; i++)
			dist += M[route[i]][route[i + 1]];
		dist += M[route[route.size() - 1]][0];
	}
};

void mutate(chrom& ch, std::vector <std::vector<int>> M, int MutationPossibility)
{
	srand(time(0));
	int chance = rand() % 100 + 1;
	if (MutationPossibility > chance) // если выпало делать мутацию
	{
		int FirstIndex = rand() % ch.route.size(), SecondIndex = FirstIndex;
		while (SecondIndex == FirstIndex) SecondIndex = rand() % ch.route.size(); // выбираем 2 неравных индекса
		std::vector <int> temp = ch.route;
		int index = temp[FirstIndex];
		temp[FirstIndex] = temp[SecondIndex];
		temp[SecondIndex] = index; // меняем элементы с такими индексами местами
		ch = chrom(temp, M);
	}
}

bool cmp(chrom a, chrom b) { return a.dist < b.dist; }

void Hybridization(std::vector <chrom>& Population, std::vector <std::vector<int>> M, int MutationPossibility)
{
	srand(time(0));
	int n = Population.size();
	int FirstIndex = rand() % n, SecondIndex = FirstIndex;
	while (SecondIndex == FirstIndex) SecondIndex = rand() % n;
	chrom FirstParent = Population[FirstIndex], SecondParent = Population[SecondIndex]; // выбираем две хромосомы из популяции
	int border = rand() % (FirstParent.route.size() - 1) + 1; // вибираем место, относительно которого будет происходить обмен участками

	std::vector <int> FirstRoute; // делаем первого ребенка
	std::vector <bool> mask(FirstParent.route.size()+1, false);// маска чтобы следить за теми вершинами, которые мы уже использовали или еще не использовали
	for (int i = 0; i < border; i++) // до границы
	{
		FirstRoute.push_back(FirstParent.route[i]); // последовательно добавляем первомму ребенку гены первого родителя
		mask[FirstParent.route[i]] = true; // и помечаем, что они уже используются, в маске
	}
	for (int i = border; i < SecondParent.route.size(); i++) // дополняем первого ребенка генами из второго родителя, которые еще не использовались
	{
		if (!mask[SecondParent.route[i]])
		{
			FirstRoute.push_back(SecondParent.route[i]);
			mask[SecondParent.route[i]] = true;
		}
	}

	for (int i = border; i < FirstParent.route.size(); i++) // и заполняем оставшиеся теми, которые еще не брали, в порядке как они идут у первого родителя
	{
		if (!mask[FirstParent.route[i]])
		{
			FirstRoute.push_back(FirstParent.route[i]);
			mask[FirstParent.route[i]] = true;
		}
	}
	
	chrom FirstChild(FirstRoute, M); // делаем из пути хромосому
	mutate(FirstChild, M, MutationPossibility); // выполняем мутацию

	std::vector <int> SecondRoute; // все то же самое для второго ребенка, но в порядке родителей второй-первый-второй
	for (int i = 0; i < mask.size(); i++) mask[i] = false;
	for (int i = 0; i < border; i++)
	{
		SecondRoute.push_back(SecondParent.route[i]);
		mask[SecondParent.route[i]] = true;
	}
	
	for (int i = border; i < FirstParent.route.size(); i++)
	{
		if (!mask[FirstParent.route[i]])
		{
			SecondRoute.push_back(FirstParent.route[i]);
			mask[FirstParent.route[i]] = true;
		}
	}
	
	for (int i = border; i < SecondParent.route.size(); i++)
	{
		if (!mask[SecondParent.route[i]])
		{
			SecondRoute.push_back(SecondParent.route[i]);
			mask[SecondParent.route[i]] = true;
		}
	}

	chrom SecondChild(SecondRoute, M);
	mutate(SecondChild, M, MutationPossibility);

	Population.push_back(FirstChild);
	Population.push_back(SecondChild);
}

void sort(std::vector <chrom>& Population) { sort(Population.begin(), Population.end(), cmp); }

std::pair <std::vector<int>, int> genetic(std::vector <std::vector<int>> M)
{
	int Attempts = 5; // константа количества запусков алгоритма
	std::vector <chrom> reses;
	while (Attempts--)
	{
		int Iterations = M.size() * M.size() + M.size() * 100; // константа количества итераций
		int MutationPossibility = 33; // константа для вероятности мутаций
		int PopulationSize = M.size() + 4; // константа для размера популяции

		std::vector <chrom> Population;
		std::vector <int> Generic;
		for (int i = 1; i < M.size(); i++) Generic.push_back(i);
		//Population.push_back(chrom(Generic, M));
		for (int i = 0; i < PopulationSize; i++)
		{
			unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
			std::default_random_engine e(seed);
			std::shuffle(std::begin(Generic), std::end(Generic), e);
			Population.push_back(chrom(Generic, M));
		}
		// Подготовлена популяция
		while (Iterations--)
		{
			Hybridization(Population, M, MutationPossibility); // производим гибридизацию в популяции
			sort(Population); // сортируем по возрастанию
			Population.erase(Population.end() - 1); // удаляем двух наихудших
			Population.erase(Population.end() - 1);
		}
		reses.push_back(Population[0]); // к результатам добавляем лучшего из этой попытки
	}
	sort(reses);
	chrom Final = reses[0]; // выбираем лучшего из всех попыток
	std::pair <std::vector <int>, int> Res(Final.route, Final.dist);
	Res.first.insert(Res.first.begin(), 0);
	Res.first.push_back(0);
	return Res;
}