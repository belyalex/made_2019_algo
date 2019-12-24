/*
Дан неориентированный связный граф. Требуется найти вес минимального остовного дерева в этом графе.
Вариант 2. С помощью алгоритма Крускала.

Ваш номер варианта прописан в ведомости.
Формат ввода
Первая строка содержит два натуральных числа n и m — количество вершин и ребер графа соответственно
(1 ≤ n ≤ 20000, 0 ≤ m ≤ 100000).
Следующие m строк содержат описание ребер по одному на строке.
Ребро номер i описывается тремя натуральными числами bi, ei и wi — номера концов ребра и его вес соответственно
 (1 ≤ bi, ei ≤ n, 0 ≤ wi ≤ 100000).

Формат вывода
Выведите единственное целое число - вес минимального остовного дерева.

Пример 1
Ввод
4 4
1 2 1
2 3 2
3 4 5
4 1 4

Вывод
7

Пример 2
Ввод
5 10
4 3 3046
4 5 90110
5 1 57786
3 2 28280
4 3 18010
4 5 61367
4 1 18811
4 2 69898
3 5 72518
3 1 85838

Вывод
107923
 */

#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <cassert>

class DSU {
public:
    explicit DSU(size_t size);

    int find(int element);

    void merge(int left, int right);

    void print() const;

private:
    std::vector<int> parent;
    std::vector<int> rank;
};

DSU::DSU(size_t size) :
        parent(size, -1),
        rank(size, 1) {}

int DSU::find(int element) {
    if (parent[element] == -1) {
        return element;
    }
    return parent[element] = find(parent[element]);
}

void DSU::merge(int left, int right) {
    const int deputat_left = find(left);
    const int deputat_right = find(right);
    assert(deputat_left != deputat_right);
    if (rank[deputat_left] == rank[deputat_right]) {
        parent[deputat_right] = deputat_left;
        ++rank[deputat_left];
    } else if (rank[deputat_left] > rank[deputat_right]) {
        parent[deputat_right] = deputat_left;
    } else {
        parent[deputat_left] = deputat_right;
    }
}

void DSU::print() const {
    for (const auto &item : parent) {
        std::cout << item << "\t";
    }
    std::cout << std::endl;

}

class Graph {
public:
    explicit Graph(size_t n, size_t m);

    void AddEdge(size_t b, size_t e, size_t w);

    size_t MinimalSpanningTreeWeight();

private:
    std::vector<std::vector<size_t>> vertexes;
    std::vector<std::tuple<size_t, size_t, size_t>> edges; //каждое ребро - тройка: 2 вершины и вес 
};

Graph::Graph(const size_t n, const size_t m) :
        vertexes(n),
        edges(m) {}

void Graph::AddEdge(const size_t b, const size_t e, const size_t w) {
    vertexes[b].push_back(e);
    vertexes[e].push_back(b);

    edges.push_back(std::make_tuple(b, e, w));
}

size_t Graph::MinimalSpanningTreeWeight() {
    DSU dsu(vertexes.size());
    std::sort(edges.begin(), edges.end(),
              [](const std::tuple<size_t, size_t, size_t> &t1, const std::tuple<size_t, size_t, size_t> &t2) {
                  return std::get<2>(t1) < std::get<2>(t2);
              });

    size_t Weight = 0;
    for (const auto &edge : edges) {
        size_t b = std::get<0>(edge);
        size_t e = std::get<1>(edge);

        size_t db = dsu.find(b);
        size_t de = dsu.find(e);
        if (db != de) {
            dsu.merge(db, de);
            Weight += std::get<2>(edge);
        }
    }

    return Weight;
}

int main() {
    size_t n = 0, m = 0;
    std::cin >> n >> m;
    Graph graph(n, m);
    for (size_t i = 0; i < m; i++) {
        size_t b = 0, e = 0, w = 0;
        std::cin >> b >> e >> w;
        graph.AddEdge(b - 1, e - 1, w);
    }

    size_t Weight = graph.MinimalSpanningTreeWeight();

    std::cout << Weight;

    return 0;
}
