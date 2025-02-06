#include "types.h"
#include <iostream>

using namespace std;

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