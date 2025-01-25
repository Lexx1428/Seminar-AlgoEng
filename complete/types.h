#ifndef TYPES_H
#define TYPES_H

#include <iostream>
#include <vector>
using namespace std;

struct Item {
    int index;
    int profit;
    int weight;

    double profitToWeightRatio() const {
        return static_cast<double>(profit) / weight;
    }
};

struct KnapsackInstance {
    std::vector<Item> items;
    int capacity;
};


ostream& operator<<(ostream &os, const Item &item) {}


ostream& operator<<(ostream &os, const KnapsackInstance &Instance) {}

#endif