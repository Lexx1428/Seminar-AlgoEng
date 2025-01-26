#include "types.h"

using namespace std;

struct Item {
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

ostream& operator<<(ostream &os, const Item &item) {
    os << "Profit: " << item.profit << ", Weight: " << item.weight << " Ratio: " << item.profitToWeightRatio();
    return os;
}

ostream& operator<<(ostream &os, const KnapsackInstance &Instance) {
    os << "Capacity: " << Instance.capacity << endl;
    for (const auto& item : Instance.items) {
        os << item << endl;
    }
    return os;
}