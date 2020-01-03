//
// Created by alex on 26.12.2019.
//

#include <random>
#include <cfloat>
#include <algorithm>
#include <iostream>
#include "Graph.h"
#include "DSU.h"

Graph::Graph(const size_t n) :
        vertexes(n),
        vertex_edges(n) {
    GenerateRandomGraph(n);
}

void Graph::GenerateRandomGraph(size_t n) {
    /*
     Не забываем инициализировать генератор случайных чисел. А то была одна история...
     Начало лета 1991 года. Нас в школе после 10-го класса послали на практику. Разбили класс на группы по 4-5 человек,
     школа договорилась с какими-то конторами, куда нас могли принять. Мы попали в организацию
     рядом со школой, где был КОМПЬЮТЕР ДВК-3. Здоровенный гроб, на котором из развлечений был только бейсик.
     Нашу группу запускали в комнату с этим монстром и оставляли часа на 2.
     Обычно мы там коротали время за игрой в карты. Но в один из дней произошла эта поучительная история.
     Один из моих друзей предложил развлечься, сыграв на ДВК на деньги в написанную им самим программу типа
     спортлото то ли 6 из 49, то ли 5 из 36. (Он вообще любил устраивать всякие споры и игры на деньги :))
     Ставка была 15 копеек. Выигрыш за угадывание всех чисел был обозначен суммой 25 ещё тех, советских рублей.
     Второй мой друг согласился. Сделал первую ставку, проиграл. В следующий заход он выиграл 25 рублей:)
     С тех пор я стараюсь не забывать делать инициализацию генератора случайных чисел.
     */
    time_t t;
    static std::default_random_engine generator(time(&t)); //static, чтобы генератор 1 раз инициализировался
    std::uniform_real_distribution<double> distribution(0, 1);
    for (size_t i = 0; i < n; i++) {
        //Генерируем координаты вершин при помощи преобразования Бокса-Мюллера
        double r = distribution(generator);
        double fi = distribution(generator);

        double x = cos(2 * M_PI * fi) * sqrt(-2 * log(r));
        double y = sin(2 * M_PI * fi) * sqrt(-2 * log(r));
        //std::cout << x << ' ' << y << std::endl;
        coords.push_back(Point{x, y});
    }
    for (size_t i = 0; i < n; i++) {
        for (size_t j = i + 1; j < n; j++) {
            vertexes[i].push_back(j);
            vertexes[j].push_back(i);

            double w = CalcDistance(coords[i], coords[j]);

            edges.push_back(std::make_tuple(i, j, w));

            vertex_edges[i].push_back(std::make_pair(w, false));
            vertex_edges[j].push_back(std::make_pair(w, false));
        }
    }
}

double Graph::CalcDistance(const Point &p1, const Point &p2) {
    return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}

double Graph::GetMinimalBruteForceMarshrut() {
    std::vector<bool> visited(vertexes.size(), false);

    double min_weight = DBL_MAX;
    return CalcBruteForceMarshrut(0, 0, 1, visited, min_weight);
}

double Graph::GetMinimalSpanningTreeMarshrut() {
    DSU dsu(vertexes.size());
    std::sort(edges.begin(), edges.end(),
              [](const std::tuple<size_t, size_t, double> &t1, const std::tuple<size_t, size_t, double> &t2) {
                  return std::get<2>(t1) < std::get<2>(t2);
              });
    for (auto &edge : edges) {
        size_t b = std::get<0>(edge);
        size_t e = std::get<1>(edge);

        size_t db = dsu.find(b);
        size_t de = dsu.find(e);
        if (db != de) {
            dsu.merge(db, de);

            for (size_t i = 0; i < vertexes[b].size(); i++) {
                if (vertexes[b][i] == e) {
                    vertex_edges[b][i].second = true;
                }
            }
            for (size_t i = 0; i < vertexes[e].size(); i++) {
                if (vertexes[e][i] == b) {
                    vertex_edges[e][i].second = true;
                }
            }
        }
    }

    std::vector<bool> visited(vertexes.size(), false);
    double weight= CalcMinimalSpanningTreeMarshrut(0, visited);

    return weight;
}

double Graph::CalcBruteForceMarshrut(size_t v, double weight, size_t depth, std::vector<bool> &visited, double &min_weight) {
    visited[v] = true;

    if (depth == vertexes.size()) {
        weight = weight + vertex_edges[v][0].first;
        if (weight < min_weight) {
            min_weight = weight;
        }
    } else {
        for (size_t i = 0; i < vertexes[v].size(); i++) {
            if (!visited[vertexes[v][i]]) {
                CalcBruteForceMarshrut(vertexes[v][i], weight + vertex_edges[v][i].first,
                                       depth + 1, visited, min_weight);
            }
        }
    }

    visited[v] = false;

    return min_weight;
}

double Graph::CalcMinimalSpanningTreeMarshrut(size_t v, std::vector<bool>& visited) {
    visited[v]=true;

    for (size_t i=0;i<vertexes[v].size();i++) {
        if (!visited[vertexes[v][i]] && vertex_edges[v][i].second) {
            //Пытаемся идти по минимальному остовному дереву
            return CalcMinimalSpanningTreeMarshrut(vertexes[v][i], visited) + vertex_edges[v][i].first;
        }
    }

    for (size_t i=0;i<vertexes[v].size();i++) {
        if (!visited[vertexes[v][i]]) {
            //Если по минимальному остовному дереву пройти нельзя - идём просто в непосещённую вершину
            return CalcMinimalSpanningTreeMarshrut(vertexes[v][i], visited) + vertex_edges[v][i].first;
        }
    }

    //Посетили все вершины - надо вернуться в начальную
    return vertex_edges[v][0].first;
}
