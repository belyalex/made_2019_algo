//
// Created by alex on 26.12.2019.
//

#include <cassert>
#include <iostream>
#include "DSU.h"

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