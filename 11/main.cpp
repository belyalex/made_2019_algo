/*
11_2. Количество различных путей
Дан невзвешенный неориентированный граф. В графе может быть несколько кратчайших путей между какими-то вершинами.
Найдите количество различных кратчайших путей между заданными вершинами. Требуемая сложность O(V+E).
Ввод: v: кол-во вершин (макс. 50000), n: кол-во ребер (макс. 200000), n пар реберных вершин, пара вершин v, w для запроса.
Вывод: количество кратчайших путей от v к w.

in
4
5
0 1
0 2
1 2
1 3
2 3

0 3

out
2



*/

#include <iostream>
#include <vector>
#include <queue>

class Graph {
public:
    explicit Graph(size_t V);

    void AddEdge(size_t x, size_t y);

    size_t GetMinPathCount(size_t v, size_t w) const;

private:
    std::vector<std::vector<size_t>> graph;  //для каждой вершины графа храним вектор смежных вершин
};

Graph::Graph(const size_t V) :
        graph(V) {}

void Graph::AddEdge(const size_t x, const size_t y) {
    graph[x].push_back(y);
    graph[y].push_back(x);
}

//Поиск числа кратчайших путей из вершины v в w
size_t Graph::GetMinPathCount(const size_t v, const size_t w) const {
    //Для посещаемых вершин будем хранить длину минимального пути от начальной вершины.
    std::vector<size_t> min_d(graph.size(), SIZE_MAX);
    //Здесь для посещаяемых вершин будем хранить количество минимальных путей от начальной вершины.
    std::vector<size_t> count(graph.size(),0);
    //Здесь когда-нибудь будет длина кратчайшего пути из v в w.
    //А пока просто самое большое число, влезающее в size_t.
    size_t distance = SIZE_MAX;
    //В очереди будем хранить пару вершин: в какаую вершину и из какой идём на текущем шаге.
    std::queue<std::pair<size_t, size_t>> queue;
    //Положим начальную вершину в очередь.
    queue.push(std::make_pair(v, v));
    //В начальную вершину из начальной можно попасть ровно одним способом :)
    count[v] = 1;
    while (!queue.empty()) {
        //Читаем из очереди очередную пару вершин.
        auto p = queue.front();
        queue.pop();
        if (min_d[p.first] == SIZE_MAX) {
            //Первый раз попали в вершину p.first.
            //Длина минимального пути до неё будет равна длине минимального пути до предка + 1.
            //Для начальной вершины v тоже получим правильное расстояние: SIZE_MAX + 1 переполнится до 0.
            min_d[p.first] = min_d[p.second] + 1;
            //Количество путей пока равно количеству путей до предка
            count[p.first] = count[p.second];
            if (p.first == w) {
                //Попали в искомую вершину, значит стала известна длина минимального пути в искомую вершину.
                distance = min_d[p.first];
            }
            if (min_d[p.first] < distance) {
                //Дальше продолжать обход имеет смысл, если только длина пути до текущей вершины меньше
                //длины минимального пути от v до w. Если мы пока не знаем длину минимального пути от v до w,
                //то в distance у нас лежит очень большое число, и данное условие сработает.
                for (auto x:graph[p.first]) {
                    //Положим в очередь все пары со смежными вершинами, а в качестве предка - текущая вершина.
                    queue.push(std::make_pair(x, p.first));
                }
            }
        } else if (min_d[p.first] == min_d[p.second] + 1) {
            //Повторно пришли в некоторую вершину и длина пути при этом такая же.
            //Прибавим к счётчику путей в текущую вершину счётчик числа путей из предка.
            //Более короткий путь у нас получиться не может, так как мы обходим граф в ширину, а более длинные пути
            //нам не интересны.
            count[p.first] += count[p.second];
        }
    }
    //Вернём счётчик числа путей для конечной вершины.
    return count[w];
}

int main() {
    size_t V = 0; //кол-во вершин
    size_t N = 0; //кол-во рёбер

    std::cin >> V >> N;
    Graph graph(V);
    for (size_t i = 0; i < N; i++) {
        size_t x = 0, y = 0;
        std::cin >> x >> y;
        graph.AddEdge(x, y);
    }
    size_t v = 0, w = 0; //вершины между которыми надо найти количество кратчайших путей
    std::cin >> v >> w;

    size_t count = graph.GetMinPathCount(v, w);

    std::cout << count << std::endl;

    return 0;
}
