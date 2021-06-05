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
	chrom(std::vector <int> vec, std::vector <std::vector<int>> M) // ��� �������� �� ������ ���� ������� ��� ���������
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
	if (MutationPossibility > chance) // ���� ������ ������ �������
	{
		int FirstIndex = rand() % ch.route.size(), SecondIndex = FirstIndex;
		while (SecondIndex == FirstIndex) SecondIndex = rand() % ch.route.size(); // �������� 2 �������� �������
		std::vector <int> temp = ch.route;
		int index = temp[FirstIndex];
		temp[FirstIndex] = temp[SecondIndex];
		temp[SecondIndex] = index; // ������ �������� � ������ ��������� �������
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
	chrom FirstParent = Population[FirstIndex], SecondParent = Population[SecondIndex]; // �������� ��� ��������� �� ���������
	int border = rand() % (FirstParent.route.size() - 1) + 1; // �������� �����, ������������ �������� ����� ����������� ����� ���������

	std::vector <int> FirstRoute; // ������ ������� �������
	std::vector <bool> mask(FirstParent.route.size()+1, false);// ����� ����� ������� �� ���� ���������, ������� �� ��� ������������ ��� ��� �� ������������
	for (int i = 0; i < border; i++) // �� �������
	{
		FirstRoute.push_back(FirstParent.route[i]); // ��������������� ��������� �������� ������� ���� ������� ��������
		mask[FirstParent.route[i]] = true; // � ��������, ��� ��� ��� ������������, � �����
	}
	for (int i = border; i < SecondParent.route.size(); i++) // ��������� ������� ������� ������ �� ������� ��������, ������� ��� �� ��������������
	{
		if (!mask[SecondParent.route[i]])
		{
			FirstRoute.push_back(SecondParent.route[i]);
			mask[SecondParent.route[i]] = true;
		}
	}

	for (int i = border; i < FirstParent.route.size(); i++) // � ��������� ���������� ����, ������� ��� �� �����, � ������� ��� ��� ���� � ������� ��������
	{
		if (!mask[FirstParent.route[i]])
		{
			FirstRoute.push_back(FirstParent.route[i]);
			mask[FirstParent.route[i]] = true;
		}
	}
	
	chrom FirstChild(FirstRoute, M); // ������ �� ���� ���������
	mutate(FirstChild, M, MutationPossibility); // ��������� �������

	std::vector <int> SecondRoute; // ��� �� �� ����� ��� ������� �������, �� � ������� ��������� ������-������-������
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
	int Attempts = 5; // ��������� ���������� �������� ���������
	std::vector <chrom> reses;
	while (Attempts--)
	{
		int Iterations = M.size() * M.size() + M.size() * 100; // ��������� ���������� ��������
		int MutationPossibility = 33; // ��������� ��� ����������� �������
		int PopulationSize = M.size() + 4; // ��������� ��� ������� ���������

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
		// ������������ ���������
		while (Iterations--)
		{
			Hybridization(Population, M, MutationPossibility); // ���������� ������������ � ���������
			sort(Population); // ��������� �� �����������
			Population.erase(Population.end() - 1); // ������� ���� ���������
			Population.erase(Population.end() - 1);
		}
		reses.push_back(Population[0]); // � ����������� ��������� ������� �� ���� �������
	}
	sort(reses);
	chrom Final = reses[0]; // �������� ������� �� ���� �������
	std::pair <std::vector <int>, int> Res(Final.route, Final.dist);
	Res.first.insert(Res.first.begin(), 0);
	Res.first.push_back(0);
	return Res;
}