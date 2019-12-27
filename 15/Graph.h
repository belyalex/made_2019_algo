//
// Created by alex on 26.12.2019.
//

#ifndef TASK15_GRAPH_H
#define TASK15_GRAPH_H

#include <vector>
#include <tuple>

struct Point {
    double x;
    double y;
};

class Graph {
public:
    explicit Graph(size_t n);

    double GetMinimalBruteForceMarshrut();
    double GetMinimalSpanningTreeMarshrut();
private:
    //для каждой вершины храним вектор смежных вершин
    std::vector<std::vector<size_t>> vertexes;
    //для каждой вершины храним веса рёбер и признак, принадлежит ли ребро мин. остовному дереву.
    std::vector<std::vector<std::pair<double,bool>>> vertex_edges;
    //вектор всех рёбер графа. для каждого ребра храним номера
    //вершин, вес и признак, принадлежит ли ребро мин. остовному дереву.
    std::vector<std::tuple<size_t,size_t,double>> edges;
    //координаты вершин
    std::vector<Point> coords;

    void GenerateRandomGraph(size_t n);

    double CalcBruteForceMarshrut(size_t v, double weight, size_t depth, std::vector<bool>& visited, double& min_weight);

    double CalcMinimalSpanningTreeMarshrut(size_t v, std::vector<bool>& visited);

    static double CalcDistance(const Point &p1,const Point &p2);
};


#endif //TASK15_GRAPH_H
