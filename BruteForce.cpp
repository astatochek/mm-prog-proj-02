#include <iostream>
#include <vector>
#include <utility>
#include <cstdlib>
#include <random>
#include <chrono>
#include <algorithm>
#include "limits.h"

bool cmp(std::pair <std::vector<int>, int> a, std::pair <std::vector<int>, int> b)
{
    return a.second < b.second;
}


void brute_force_help(std::vector<int> path, std::vector<int> rest, int dist, std::vector <std::pair <std::vector<int>, int>>& RES, std::vector <std::vector<int>> M) // рекурсивное нахождение всех путей
{
    std::vector <int> path_next; // следующий путь для рекурсии
    std::vector <int> rest_next; // следующие неиспользованные вершины для рекурсии
    int dist_next;
    for (int i = 0; i < rest.size(); i++) // для каждой вершины из оставшихся выберем ее и добавим к текущему пути
    {
        path_next = path;
        path_next.push_back(rest[i]);
        rest_next = rest;
        rest_next.erase(rest_next.begin() + i);
        dist_next = dist;
        dist_next += M[path_next[path_next.size() - 2]][path_next[path_next.size() - 1]];

        if (!rest_next.empty()) brute_force_help(path_next, rest_next, dist_next, RES, M);
        else
        {
            std::pair <std::vector<int>, int> pair_;
            path_next.push_back(0);
            pair_.first = path_next;
            dist_next += M[path_next[path_next.size() - 2]][path_next[path_next.size() - 1]];
            pair_.second = dist_next;
            RES.push_back(pair_); // получившийся путь добавляем к массиву всех путей
        }
    }
}

std::pair <std::vector<int>, int> brute_force(std::vector <std::vector<int>> M)
{
    std::vector <std::pair <std::vector<int>, int>> RES = {};
    std::vector <int> h;
    for (int i = 1; i < M.size(); i++) h.push_back(i);
    brute_force_help({ 0 }, h, 0, RES, M);
    sort(RES.begin(), RES.end(), cmp); // находим минимальный среди всех путей
    return RES[0];
}