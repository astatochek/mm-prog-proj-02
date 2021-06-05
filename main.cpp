#include <iostream>
#include <vector>
#include <cmath>
#include <utility>
#include <chrono>
#include <string>
#include <cstdio>
#include <ctime>
#include <algorithm>
#include "limits.h"


#include "SimulatedAnnealing.h"
#include "BranchAndBound.h"
#include "ClosestNeighbour.h"
#include "BranchAndBound.h"
#include "BruteForce.h"
#include "Dynamic.h"
#include "Genetic.h"
#include "AntColony.h"
//#include "SFML\Graphics.hpp"


//using namespace sf;

std::string printable(std::vector <int> route)
{
    std::string res = "";
    for (int i = 0; i < route.size(); i++)
    {
        res += std::to_string(route[i]+1);
        if (i != route.size() - 1)
            res += " -> ";
    }
    return res;
}


std::vector <std::vector <int>> MakeM(int dist, int n)
{
    // dist - расстояние между соседними вершинами правильного n-угольника
    // n - количество вершин правильного n-угольника

    double radius = dist / (2 * sin(3.14159 / n));
    std::vector <double> angles(n);
    for (int i = 0; i < n; i++)
        angles[i] = i * 2 * 3.14159 / n;
    std::vector <std::pair <double, double>> points(n);
    for (int i = 0; i < n; i++)
        points[i] = std::pair <double, double>(radius - radius * cos(angles[i]), radius - radius * sin(angles[i]));
    std::vector <std::vector <int>> M(n, std::vector <int>(n));
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (i == j)
            {
                M[i][j] = INT_MAX;
                continue;
            }
            double ijdist = sqrt(pow(points[i].first - points[j].first, 2) + pow(points[i].second - points[j].second, 2)); // расстояние между двумя точками
            M[i][j] = (ijdist > (double)(int)ijdist + 0.5) ? ((int)ijdist + 1) : ((int)ijdist); // типа округление, потому что в массиве должны быть int-ы
        }

    }

    return M;     
}

struct Input
{
    std::string name;
    std::vector <int> route;
    int dist;
    double time;
    //Color color;
    Input(std::string name_, std::vector <int> route_, int dist_, double time_)
    {
        name = name_;
        route = route_;
        dist = dist_;
        time = time_;
    }
};

bool DistCmp(Input A, Input B) { return A.dist < B.dist; }
bool TimeCmp(Input A, Input B) { return A.time < B.time; }

int main() {

    int ExampleNumber;
    std::cin >> ExampleNumber;


    std::vector <std::vector <int>> M;
    // Example 1 - ломаем жадный
    if (ExampleNumber == 1)
    {
        M = {
        {INT_MAX, 1, 2, 2},
        {2, INT_MAX, 1, 2},
        {2, 2, INT_MAX, 1},
        {100000, 2, 2, INT_MAX} };
    }

    // Ezxample 2 - ломаем перебор
    if (ExampleNumber == 2)
    {
        M = MakeM(5, 10);
    }

    // Ezxample 3 - ломаем динамику
    if (ExampleNumber == 3)
    {
        M = MakeM(5, 19);
    }

    // Ezxample 4
    if (ExampleNumber == 4)
    {
        M = MakeM(20, 40);
    }
    


    // Вывод графа
    std::cout << "The graph is:\n";

    for (int i = 0; i < M.size(); i++)
    {
        for (int j = 0; j < M[i].size(); j++) std::cout << M[i][j] << " ";
        std::cout << "\n";
    }
    std::cout << "\n";

    
    std::vector <Input> Inputs;
    std::clock_t start;
    double duration;
    // Вывод результатов разных методов решения

    if (ExampleNumber == 1 || ExampleNumber == 2)
    {
        start = std::clock();
        std::pair <std::vector <int>, int> brute_force_res = brute_force(M);
        duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
        Inputs.push_back(Input("Brute Force", brute_force_res.first, brute_force_res.second, duration));
    }


    start = std::clock();
    std::pair <std::vector <int>, int> closest_neighbour_res = closest_neighbour(M);
    duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
    Inputs.push_back(Input("Closest Neighbour", closest_neighbour_res.first, closest_neighbour_res.second, duration));


    start = std::clock();
    std::pair <std::vector <int>, int> simulated_annealing_res = simulated_annealing(M);
    duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
    Inputs.push_back(Input("Simulated Annealing", simulated_annealing_res.first, simulated_annealing_res.second, duration));


    start = std::clock();
    std::pair <std::vector <int>, int> branch_and_bound_res = branch_and_bound(M);
    duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
    Inputs.push_back(Input("Branch and Bound", branch_and_bound_res.first, branch_and_bound_res.second, duration));

    start = std::clock();
    std::pair <std::vector <int>, int> genetic_res = genetic(M);
    duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
    Inputs.push_back(Input("Genetic Algorithm", genetic_res.first, genetic_res.second, duration));
    start = std::clock();

    std::pair <std::vector <int>, int> ant_colony_res = ant_colony(M);
    duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
    Inputs.push_back(Input("Ant Colony Algorithm", ant_colony_res.first, ant_colony_res.second, duration));
    std::cout << "\n";

    


    if (ExampleNumber == 1 || ExampleNumber == 2 || ExampleNumber == 3)
    {

        start = std::clock();
        std::pair <std::vector <int>, int> dynamic_res = dynamic(M);
        duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
        Inputs.push_back(Input("Dynamic Algorithm", dynamic_res.first, dynamic_res.second, duration));
    }
    
    for (auto Input : Inputs)
    {
        std::string tmp = printable(Input.route);
        std::cout << Input.name << ":\nRoute:" << tmp << "\nDistance:" << Input.dist << "\nTime:" << Input.time << "\n\n";
    }



    //SFML Part
    /*
    // Dist Diagram
    sort(Inputs.begin(), Inputs.end(), DistCmp);
    int MaxDist = Inputs[Inputs.size() - 1].dist;
    MaxDist = MaxDist - MaxDist % 10 + 10;
    double Coefficient = double(MaxDist) / 400.0;
    // высота прямоугольника равна dist/Coefficient

    RenderWindow window(VideoMode(1700, 1000), "Window");
    int wwidth = window.getSize().x;
    int wheight = window.getSize().y;


    int ox = 150, oy = 50;
   
    VertexArray grid(Lines);
    Vertex dot;
    for (int i = 0; i < 36; i++) {
        dot.color = { 100,100,100 };
        dot.position = Vector2f(ox + i*40,oy);
        grid.append(dot);
        dot.position = Vector2f(ox + i * 40, 400 + oy);
        grid.append(dot);
    }
    window.draw(grid);

    for (int i = 0; i < 11; i++) {
        dot.color = { 100,100,100 };
        dot.position = Vector2f(ox, oy + i*40);
        grid.append(dot);
        dot.position = Vector2f(1400 + ox, oy + i*40);
        grid.append(dot);
    }
    window.draw(grid);

    
    Text text;
    Font font;
    font.loadFromFile("IBMPlexSans-Medium.ttf");
    text.setFont(font);
    text.setCharacterSize(12);

    RectangleShape Rect;
    int hyte;
    std::vector <Color> colorvector = { {102,255,255}, {102,255,178 }, {102,255,102 }, {178,255,102 }, {255,255,102 }, {255,178,102 }, { 255,102,102 } };
    for (int i = 0; i < 7; i++) {
        hyte = Inputs[i].dist / Coefficient;
        Rect.setFillColor(colorvector[i]);
        Rect.setPosition(ox + 80 * (i + 1) + 120*i, 400 + oy-hyte);
        Rect.setSize(Vector2f(120, hyte));
        window.draw(Rect);
        Inputs[i].color = colorvector[i];
        text.setString(Inputs[i].name + "\nDistance: " + std::to_string(Inputs[i].dist));
        text.setFillColor(Inputs[i].color);
        text.setPosition(ox + 80 * (i + 1) + 120 * i, 400 + oy + 5);
        window.draw(text);
    }

    RectangleShape axis;
    axis.setFillColor(Color::White);
    axis.setPosition(ox, oy);
    axis.setSize(Vector2f(3, 400));
    window.draw(axis);
    axis.setPosition(ox, 400 + oy - 1);
    axis.setSize(Vector2f(1400, 3));
    window.draw(axis);

    
    

    sort(Inputs.begin(), Inputs.end(), TimeCmp);
    double MaxTime = Inputs[Inputs.size() - 1].time;
    Coefficient = double(MaxTime) / 400.0;
    

    

    ox = 150;
    oy = 550;
    VertexArray grid2(Lines);
    Vertex dot2;
    for (int i = 0; i < 36; i++) {
        dot2.color = { 100,100,100 };
        dot2.position = Vector2f(ox + i * 40, oy);
        grid2.append(dot2);
        dot2.position = Vector2f(ox + i * 40, 400 + oy);
        grid2.append(dot2);
    }
    window.draw(grid2);

    for (int i = 0; i < 11; i++) {
        dot2.color = { 100,100,100 };
        dot2.position = Vector2f(ox, oy + i * 40);
        grid2.append(dot2);
        dot2.position = Vector2f(1400 + ox, oy + i * 40);
        grid2.append(dot2);
    }
    window.draw(grid2);

    RectangleShape Rect2;
    int hyte2;
    for (int i = 0; i < 7; i++) {
        hyte2 = Inputs[i].time / Coefficient;
        Rect2.setFillColor(Inputs[i].color);
        Rect2.setPosition(ox + 80 * (i + 1) + 120 * i, 400 + oy - hyte2);
        Rect2.setSize(Vector2f(120, hyte2));
        window.draw(Rect2);
        text.setString(Inputs[i].name + "\nTime: " + std::to_string(Inputs[i].time));
        text.setFillColor(Inputs[i].color);
        text.setPosition(ox + 80 * (i + 1) + 120 * i, 400 + oy + 5);
        window.draw(text);
    }

    RectangleShape axis2;
    axis2.setFillColor(Color::White);
    axis2.setPosition(ox, oy);
    axis2.setSize(Vector2f(3, 400));
    window.draw(axis2);
    axis2.setPosition(ox, 400 + oy - 1);
    axis2.setSize(Vector2f(1400, 3));
    window.draw(axis2);

    window.display();
    system("pause");
    */
    return 0;
}
