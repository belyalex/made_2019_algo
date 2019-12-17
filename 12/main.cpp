#include <fstream>
#include <vector>
#include <stack>
#include <algorithm>
#include <tuple>

class Graph {
public:
    explicit Graph(size_t V);

    std::vector<size_t> FindAllBridges();

    void AddEdge(size_t n, size_t x, size_t y);
private:
    std::vector<std::vector<size_t>> vertexes; //для каждой вершины храним вектор смежных вершин
    std::vector<std::vector<size_t>> edges;    //для каждой вершины номера рёбер в том же порядке

    void DFS(size_t v, std::vector<bool>& visited, std::vector<size_t>& bridges,
             std::vector<size_t>& in_time, std::vector<size_t>& out_time);
};

Graph::Graph(const size_t V) :
        vertexes(V),
        edges(V) {}

std::vector<size_t> Graph::FindAllBridges() {
    std::vector<size_t> bridges; //сюда будем записывать номера рёбер - мостов
    std::vector<bool> visited(vertexes.size(),false); //признаки посещения вершин
    std::vector<size_t> in_time(vertexes.size(),0);   //времена входа в вершины
    std::vector<size_t> out_time(vertexes.size(),0);  //времена выхода

    for (size_t v = 0; v < vertexes.size(); v++) {
        if (!visited[v]) {
            DFS(v, visited, bridges, in_time, out_time);
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

// Нерекурсивный обход графа в глубину
void Graph::DFS(const size_t v1, std::vector<bool>& visited,std::vector<size_t>& bridges,
                std::vector<size_t>& in_time, std::vector<size_t>& out_time) {
    size_t time = 0; //счётчик времени
    std::stack<std::tuple<size_t, size_t, size_t>> stack; // в стеке храним вершину, номер смежной вершины и номер ребра
    stack.push(std::make_tuple(v1,SIZE_MAX,0));
    while (!stack.empty()) {
next:
        std::tuple<size_t,size_t,size_t> tuple=stack.top();

        size_t v=std::get<0>(tuple);
        size_t n=std::get<1>(tuple);
        size_t p=std::get<2>(tuple);
        if (!visited[v]) {
            in_time[v] = out_time[v] = time++;
            visited[v] = true;
        }
        for (size_t i = n + 1; i < vertexes[v].size(); i++) {
            size_t w = vertexes[v][i];
            size_t e = edges[v][i];
            if (e == p) continue; //не рассматриваем то же ребро, по которому пришли в v
            if (visited[w]) {
                out_time[v] = std::min(out_time[v], in_time[w]);
            } else {
                std::get<1>(stack.top())=i;

                stack.push(std::make_tuple(w,SIZE_MAX,e));
                
                goto next;
            }
        }
        stack.pop();
        if (!stack.empty()) {
            size_t w = v;
            v = std::get<0>(stack.top());
            out_time[v] = std::min(out_time[v], out_time[w]);
            if (out_time[w] > in_time[v]) {
                // Нашли мост, добавляем в вектор мостов номер текущего ребра
                bridges.push_back(p);
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
