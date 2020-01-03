//
// Created by alex on 26.12.2019.
//

#ifndef TASK15_DSU_H
#define TASK15_DSU_H

#include <vector>

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

#endif //TASK15_DSU_H
