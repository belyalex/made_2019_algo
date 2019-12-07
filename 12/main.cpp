/*
Ребро неориентированного графа называется мостом, если удаление этого ребра из графа 
увеличивает число компонент связности.

Дан неориентированный граф, требуется найти в нем все мосты.

Формат ввода
Первая строка входного файла содержит два целых числа n и m — количество вершин и ребер 
графа соответственно (1 ≤ n ≤ 20000, 1 ≤ m ≤ 200000).

Следующие m строк содержат описание ребер по одному на строке. Ребро номер i описывается 
двумя натуральными числами bi, ei — номерами концов ребра (1 ≤ bi, ei ≤ n).

Формат вывода
Первая строка выходного файла должна содержать одно натуральное число b — количество 
мостов в заданном графе. На следующей строке выведите b целых чисел — номера ребер, 
которые являются мостами, в возрастающем порядке. Ребра нумеруются с единицы в том 
порядке, в котором они заданы во входном файле.

Пример
Ввод
6 7
1 2
2 3
3 4
1 3
4 5
4 6
5 6

Вывод
1
3
*/
#include <fstream>
#include <vector>
#include <algorithm>

class Graph {
public:
    explicit Graph(size_t V);

    std::vector<size_t> FindAllBridges();

    void AddEdge(size_t n, size_t x, size_t y);

private:
    std::vector<std::vector<size_t>> vertexes; //для каждой вершины храним вектор смежных вершин
    std::vector<std::vector<size_t>> edges;    //для каждой вершины номера рёбер в том же порядке

    std::vector<size_t> in_time;  //времена входа в вершины
    std::vector<size_t> out_time; //времена выхода
    std::vector<bool> visited;    //признаки посещения вершин

    std::vector<size_t> bridges;  //сюда будем записывать номера рёбер - мостов
    size_t time = 0;              //счётчик времени

    void DFS(size_t v, size_t p = SIZE_MAX);
};

Graph::Graph(const size_t V) :
        vertexes(V),
        edges(V),
        visited(V, false),
        in_time(V, 0),
        out_time(V, 0) {}

std::vector<size_t> Graph::FindAllBridges() {
    for (size_t v = 0; v < vertexes.size(); v++) {
        if (!visited[v]) {
            DFS(v);
        }
    }
    std::sort(bridges.begin(), bridges.end());
    return bridges;
}

// n - номер ребра, x и y - какие вершины соединяет ребро
// номера рёбер начинаются с 1
// номера вершин с 0
void Graph::AddEdge(const size_t n, const size_t x, const size_t y) {
    vertexes[x].push_back(y);
    vertexes[y].push_back(x);
    edges[x].push_back(n);
    edges[y].push_back(n);
}

// Обход графа в глубину
// v - вершина в которую идём, p - номер ребра, по которому идём
void Graph::DFS(const size_t v, const size_t p) {
    visited[v] = true;
    in_time[v] = out_time[v] = time++;
    //цикл по всем смежным вершинам
    for (size_t i = 0; i < vertexes[v].size(); i++) {
        int w = vertexes[v][i];
        int e = edges[v][i];
        if (e == p) continue; //не рассматриваем то же ребро, по которому пришли в v
        if (visited[w]) {
            out_time[v] = std::min(out_time[v], in_time[w]);
        } else {
            DFS(w, e);
            out_time[v] = std::min(out_time[v], out_time[w]);
            if (out_time[w] > in_time[v]) {
                // Нашли мост, добавляем в вектор мостов номер текущего ребра
                bridges.push_back(e);
            }
        }
    }
}

int main() {
    std::ifstream in("bridges.in");
    std::ofstream out("bridges.out");
    size_t n = 0; // кол-во вершин
    size_t m = 0; // кол-во рёбер
    in >> n >> m;
    Graph graph(n);
    for (size_t i = 0; i < m; i++) {
        size_t x = 0, y = 0;
        in >> x >> y;
        graph.AddEdge(i + 1, x - 1, y - 1);
    }

    std::vector<size_t> bridges = graph.FindAllBridges();

    out << bridges.size() << std::endl;
    for (auto b:bridges) {
        out << b << " ";
    }

    return 0;
}
