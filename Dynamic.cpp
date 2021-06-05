#include <utility>
#include <vector>
#include "limits.h"



std::pair <std::vector<int>, int> dynamic(std::vector <std::vector<int>> M)
{
	int n = M.size();
	std::vector <std::vector <std::pair<std::vector<int>, int>>> map((1 << n), std::vector<std::pair<std::vector<int>, int>>(n, std::pair<std::vector<int>, int>({ 0 }, INT_MAX))); // создаем массив масок 2^n * n
	map[0][0].second = 0; // первый элемент нулевой
	map[0][0].first = { 0 };
	for (int mask = 0; mask < (1 << n); mask++)
	{

		for (int i = 0; i < n; i++)
		{
			if (map[mask][i].second == INT_MAX) // такой рассматривать не стоит, из него никуда не выгодно идти
				continue;
			for (int j = 0; j < n; j++) // для каждой вершины
				if (!(mask & (1 << j))) // если мы в ней не были
				{
					if (map[mask ^ (1 << j)][j].second > map[mask][i].second + M[i][j]) // и в нее идти выгодней чем не идти
					{
						map[mask ^ (1 << j)][j].first = map[mask][i].first;
						map[mask ^ (1 << j)][j].first.push_back(j);
						map[mask ^ (1 << j)][j].second = map[mask][i].second + M[i][j]; // идем в нее из данного пути
					}
				}
		}
	}
	std::pair <std::vector <int>, int> res = map[(1 << n) - 1][0];
	return res;
}