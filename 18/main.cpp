/*
Задача 18. Построение выпуклой оболочки (5 баллов)
Дано множество точек на плоскости (x, y). Постройте выпуклую оболочку этого множества и вычислите ее периметр.
Вариант 1. С помощью алгоритма Грэхема.
Вариант 2. С помощью алгоритма Джарвиса.
Обратите внимание: три и более точки из множества могут лежать на одной прямой.
Формат входного файла
Количество точек, далее на каждой строке координаты точек x, y.
Общее количество точек не превосходит 100000.
Координаты точек - числа с плавающей точкой в диапазоне [-1000, 1000].
Формат выходного файла
Выведите периметр выпуклой оболочки.
Время: 100мс, память 3Mb.
stdin
3
1. 1.
2. 1.
0. 0.
stdout
4.65028154
*/

#include <iostream>
#include <vector>
#include <cfloat>
#include <cmath>
#include <algorithm>
#include <numeric>

const double eps = 1e-10;

struct Point {
    double x = 0;
    double y = 0;
};
//Эвклидово расстояние между точками
double distance(const Point &p1, const Point &p2) {
    return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}

double OrientTriangle(const Point &p1, const Point &p2, const Point &p3) {
    return p1.x * (p2.y - p3.y) + p2.x * (p3.y - p1.y) + p3.x * (p1.y - p2.y);
}
//Находится ли точка p между точками p1 и p2?
bool isInside(const Point &p1, const Point &p, const Point &p2) {
    return (p1.x <= p.x && p.x <= p2.x &&
            p1.y <= p.y && p.y <= p2.y);
}

int main() {
    int n;
    std::cin >> n;
    std::vector<Point> points(n);
    std::vector<int> MCH; //здесь будем хранить индексы точек, попавших в минимальную выпуклую оболочку
    Point p;
    double x_min = DBL_MAX;
    double y_min = DBL_MAX;
    int i_min = 0;
    for (int i = 0; i < n; i++) {
        std::cin >> p.x >> p.y;
        points[i] = p;
        // сразу ищем самую левую нижнюю точку
        if (p.x < x_min) {
            i_min = i;
            x_min = p.x;
        } else if (std::abs(p.x - x_min)<eps) {
            if (p.y < y_min) {
                i_min = i;
                y_min = p.y;
            }
        }
    }
    MCH.push_back(0);
    if (i_min != 0) {
        std::swap(points[0], points[i_min]);
    }

    int first = 0;
    int cur = 0;
    do {
        int next = (cur + 1) % points.size();
        for (int i = 0; i < points.size(); i++) {
            if (i == next) continue;
            double orient = OrientTriangle(points[cur], points[next], points[i]);
            if ((std::abs(orient)>eps) && (orient < 0)) {
                next = i;
            } else if (orient < eps) {
                //если точки лежат на одной прямой
                if (isInside(points[cur], points[next], points[i]))
                    next = i;
            }
        }
        cur = next;
        MCH.push_back(next);
    } while (cur != first);

    double perimetr = 0;
    for (size_t i = 0; i < MCH.size() - 1; i++) {
        perimetr += distance(points[MCH[i]], points[MCH[i + 1]]);
    }

    std::cout.precision(12);
    std::cout << perimetr;

    return 0;
}
