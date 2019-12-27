/*
Задача 15 (5 баллов)
«Приближенное решение метрической неориентированной задачи коммивояжера». Найдите приближенное решение метрической
неориентированной задачи коммивояжера в полном графе (на плоскости) с помощью минимального остовного дерева.
Оцените качество приближения на случайном наборе точек, нормально распределенном на плоскости с дисперсией 1. Нормально
распределенный набор точек получайте с помощью преобразования Бокса-Мюллера.
При фиксированном N, количестве вершин графа, несколько раз запустите оценку качества приближения. Вычислите среднее
значение и среднеквадратичное отклонение качества приближения для данного N.
Запустите данный эксперимент для всех N в некотором диапазоне, например, [2, 10].
Автоматизируйте запуск экспериментов.
В решении требуется разумно разделить код на файлы. Каждому классу - свой заголовочный файл и файл с реализацией.
*/

#include <iostream>
#include <numeric>
#include <cmath>
#include <cfloat>
#include <omp.h>
#include "Graph.h"

int main(int argc, char* argv[]) {
    if (argc!=4) {
        std::cout<<std::string("Usage: ")+argv[0]+ " <graph min size> <graph max size> <experiments quantity>";
        return 0;
    }

    for (size_t n=atoi(argv[1]);n<=atoi(argv[2]);n++) {
        //Коэффициенты качества приближения. Теоретически должен быть в пределах от 1 до 2.
        //Чем ближе к 1, тем лучше.
        std::vector<double> k(atoi(argv[3]));
        //Эксперименты будем запускать параллельно
#pragma omp parallel for
        for (size_t i = 0; i < atoi(argv[3]); i++) {
            Graph g(n);

            double minimal_marshrut = g.GetMinimalBruteForceMarshrut();
            double approximate_marshrut = g.GetMinimalSpanningTreeMarshrut();

            k[i]=(approximate_marshrut / minimal_marshrut);
        }


        double M = std::accumulate(k.begin(), k.end(), 0.0) / k.size();
        double D = 0;
        double Min = DBL_MAX;
        double Max = 0;
        for (auto x : k) {
            D += (M - x) * (M - x);
            Min = std::min(Min, x);
            Max = std::max(Max, x);
        }
        D /= k.size();
        double stddev = sqrt(D);

        std::cout<<"Размер графа:"<<n<<" Количество экспериментов:"<<atoi(argv[3])<<std::endl;
        std::cout << "Лучшее качество приближения:" << Min << std::endl;
        std::cout << "Худшее качество приближения:" << Max << std::endl;
        std::cout << "Мат. ожидание качества приближения:" << M << std::endl;
        std::cout << "Среднеквадратичное отклонение:" << stddev << std::endl<< std::endl;
    }
    return 0;
}