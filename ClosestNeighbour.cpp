#include <iostream>
#include <vector>
#include <utility>
#include <cstdlib>
#include <random>
#include <chrono>
#include <algorithm>
#include "limits.h"

std::pair <std::vector<int>, int> closest_neighbour(std::vector <std::vector<int>> M)
{
    std::vector <int> path = { 0 }, rest;
    for (int i = 1; i < M.size(); i++) rest.push_back(i);
    int min = INT_MAX, index_of_min, dist = 0;
    while (!rest.empty())
    {
        min = INT_MAX;
        if (rest.size() > 1)
        {
            for (int index = 0; index < rest.size(); index++)
            {
                if (M[path[path.size() - 1]][rest[index]] < min)
                {
                    min = M[path[path.size() - 1]][rest[index]]; // находим минимальное ребро
                    index_of_min = index; // запоминаем индекс вершины
                }
            }
        }
        else
        {
            min = M[path[path.size() - 1]][rest[0]];
            index_of_min = 0;
        }
        dist += min;
        path.push_back(rest[index_of_min]); // добавляем новую вершину в путь
        rest.erase(rest.begin() + index_of_min); // удаляем из оставшихся выбранную вершину
    }
    dist += M[path[path.size() - 1]][0];
    path.push_back(0);

    std::pair <std::vector<int>, int> p;
    p.first = path;
    p.second = dist;
    return p;
}