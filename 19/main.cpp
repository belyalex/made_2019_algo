/*
Задача 19. Поиск точек в прямоугольнике (5 баллов)
На вход подаются точки и прямоугольники. Точка задается двумя координатами (x, y).
Прямоугольник - четверкой чисел [left, bottom, right, top].
Точка (left, bottom) - принадлежит прямоугольнику, точка (right, top) - нет.
(left < right, bottom < top) Для каждого прямоугольника нужно вывести, сколько добавленных точек он содержит.
X  в диапазоне [-180, 180)
Y -  [-90, 90)
Количество точек <= 100000, Количество прямоугольников <= 1000
Для решения задачи необходимо реализовать алгоритм “Geohashing”.
Формат входного файла
Количество точек N. Далее N строк с координатами точек (x пробел y).
Далее количество запросов M. Прямоугольники в виде 4 координат.
Формат выходного файла
Количество точек в каждом прямоугольнике
stdin
3
1. 1.
1. 2.
2. 1.
3
0. 0. 1. 1.
1. 1. 2. 2.
1. 1. 3. 3.
stdout
0
1
3
 */

#include <iostream>
#include <vector>
#include <memory>
#include <queue>

struct Point {
    double lon = 0;
    double lat = 0;

    explicit Point(double lon = 0, double lat = 0) : lon(lon), lat(lat) {}
};

class LonComparator {
public:
    bool operator()(const Point &p1, const Point &p2) { return p1.lon > p2.lon; }
};

class LatComparator {
public:
    bool operator()(const Point &p1, const Point &p2) { return p1.lat > p2.lat; }
};

struct GeoHashNode {
    Point bottom_left;
    Point top_right;
    std::vector<Point> points;
    GeoHashNode *left = nullptr;
    GeoHashNode *right = nullptr;
    int count;

    explicit GeoHashNode(const Point &bottom_left, const Point &top_right, const int count) : bottom_left(bottom_left),
                                                                                              top_right(top_right),
                                                                                              count(count) {}

    ~GeoHashNode() {
        delete left;
        delete right;
    }
};

struct Bbox {
    double left = 0;
    double bottom = 0;
    double right = 0;
    double top = 0;

    bool Contains(const Point &point) const {
        return left <= point.lon && bottom <= point.lat &&
               point.lon < right && point.lat < top;
    }

    bool Contains(const GeoHashNode &node) const {
        return left <= node.bottom_left.lon && bottom <= node.bottom_left.lat &&
               node.top_right.lon < right && node.top_right.lat < top;
    }

    bool Intersects(const GeoHashNode &node) const {
        double l = std::max(left, node.bottom_left.lon);
        double t = std::min(top, node.top_right.lat);
        double r = std::min(right, node.top_right.lon);
        double b = std::max(bottom, node.bottom_left.lat);

        double w = r - l;
        double h = t - b;

        return ((w >= 0) && (h >= 0));
    }
};

class GeoHashTree {
public:
    explicit GeoHashTree(std::priority_queue<Point, std::deque<Point>, LonComparator> &points);

    ~GeoHashTree() { delete root; }

    int CountPointsInBbox(const Bbox &bbox);

private:

    GeoHashNode *root;

    static GeoHashNode *BuildTreeLon(
            std::priority_queue<Point, std::deque<Point>, LonComparator> &points, const Point &bottom_left,
            const Point &top_right);

    static GeoHashNode *BuildTreeLat(
            std::priority_queue<Point, std::deque<Point>, LatComparator> &points, const Point &bottom_left,
            const Point &top_right);

    static int CountPointsInBbox(
            const Bbox &bbox, const GeoHashNode *node, int depth);
};

GeoHashTree::GeoHashTree(std::priority_queue<Point, std::deque<Point>, LonComparator> &points) {
    Point bottom_left(-180, -90);
    Point top_right(180, 90);
    root = BuildTreeLon(points, bottom_left, top_right);
}

int GeoHashTree::CountPointsInBbox(const Bbox &bbox) {
    if (!root) return 0;
    return CountPointsInBbox(bbox, root, 0);
}

GeoHashNode *
GeoHashTree::BuildTreeLon(std::priority_queue<Point, std::deque<Point>, LonComparator> &points,
                          const Point &bottom_left,
                          const Point &top_right) {
    Point center1((bottom_left.lon + top_right.lon) / 2, bottom_left.lat);
    Point center2((bottom_left.lon + top_right.lon) / 2, top_right.lat);
    auto node = new GeoHashNode(bottom_left, top_right, points.size());
    if (points.size() > 5) {
        std::priority_queue<Point, std::deque<Point>, LatComparator> left_queue;
        LonComparator cmp;
        while (!points.empty()) {
            Point p = points.top();
            if (cmp(center1, p)) {
                points.pop();
                left_queue.push(p);
            } else {
                break;
            }
        }
        if (left_queue.size() > 1) {
            node->left = BuildTreeLat(left_queue, bottom_left, center2);
        }
        std::priority_queue<Point, std::deque<Point>, LatComparator> right_queue;
        while (!points.empty()) {
            Point p = points.top();
            points.pop();
            right_queue.push(p);
        }
        if (right_queue.size() > 1) {
            node->right = BuildTreeLat(right_queue, center1, top_right);
        }
    } else {
        while (!points.empty()) {
            node->points.push_back(points.top());
            points.pop();
        }
    }
    return node;
}

GeoHashNode *
GeoHashTree::BuildTreeLat(std::priority_queue<Point, std::deque<Point>, LatComparator> &points,
                          const Point &bottom_left,
                          const Point &top_right) {
    Point center1(bottom_left.lon , (bottom_left.lat + top_right.lat) / 2);
    Point center2(top_right.lon, (bottom_left.lat + top_right.lat) / 2);
    auto node = new GeoHashNode(bottom_left, top_right, points.size());
    if (points.size() > 5) {
        std::priority_queue<Point, std::deque<Point>, LonComparator> left_queue;
        LatComparator cmp;
        while (!points.empty()) {
            Point p = points.top();
            if (cmp(center1, p)) {
                points.pop();
                left_queue.push(p);
            } else {
                break;
            }
        }
        if (left_queue.size() > 1) {
            node->left = BuildTreeLon(left_queue, bottom_left, center2);
        }
        std::priority_queue<Point, std::deque<Point>, LonComparator> right_queue;
        while (!points.empty()) {
            Point p = points.top();
            points.pop();
            right_queue.push(p);
        }
        if (right_queue.size() > 1) {
            node->right = BuildTreeLon(right_queue, center1, top_right);
        }
    } else {
        while (!points.empty()) {
            node->points.push_back(points.top());
            points.pop();
        }
    }
    return node;
}

int GeoHashTree::CountPointsInBbox(const Bbox &bbox, const GeoHashNode *node, int depth) {
    if (bbox.Contains(*node)) {
        return node->count;
    }
    if (bbox.Intersects(*node)) {
        int count = 0;
        if (node->left != nullptr) {
            count += CountPointsInBbox(bbox, node->left, depth + 1);
        }
        if (node->right != nullptr) {
            count += CountPointsInBbox(bbox, node->right, depth + 1);
        }
        for (const Point &point:node->points) {
            if (bbox.Contains(point)) {
                count++;
            }
        }
        return count;
    }

    return 0;
}


int main() {
    int N;
    std::cin >> N;
    std::priority_queue<Point, std::deque<Point>, LonComparator> points;
    Point p;
    for (int i = 0; i < N; i++) {
        std::cin >> p.lon >> p.lat;
        points.push(p);
    }

    GeoHashTree tree(points);

    int M;
    std::cin >> M;
    Bbox bbox;
    for (int i = 0; i < M; i++) {
        std::cin >> bbox.left >> bbox.bottom >> bbox.right >> bbox.top;
        std::cout << tree.CountPointsInBbox(bbox) << std::endl;
    }


    return 0;
}
