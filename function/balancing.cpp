#include <vector>
#include <algorithm>
#include <cmath>
#include <iostream>
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

KnapsackInstance reduceToBalanced(const KnapsackInstance& original) {
    vector<Item> sortedItems = original.items;
    sort(sortedItems.begin(), sortedItems.end(), [](const Item &a, const Item &b){
        return a.profitToWeightRatio() > b.profitToWeightRatio();
    });


    //find rho (p) from the prefix solution
    //prefix solution: items {1,..,j} for maximum j such that w1+...+wj <= t (original capacity)
    //rho = profit-to-weight ratio of least profitable (last) item in prefix solution
    int currentWeight = 0;
    int j = 0;
    double rho = 0.0;

    for (; j < sortedItems.size(); ++j) {
        if (currentWeight + sortedItems[j].weight > original.capacity) {
            break;
        }
        currentWeight += sortedItems[j].weight;
    }

    if (j > 0 && j <= sortedItems.size()) {
        rho = sortedItems[j-1].profitToWeightRatio();
    }

    //partition items into good, medium and bad
    vector<Item> good;
    vector<Item> medium;
    vector<Item> bad;

    for (const auto& item : sortedItems) {
        double ratio = item.profitToWeightRatio();
        if (ratio > 2*rho) {
            good.push_back(item);
        }
        else if (ratio < rho/2) {
            bad.push_back(item);
        }
        else { medium.push_back(item);}
    } 

    cout << "good items: " << endl;
    for (const auto& item : good) {
        cout << "profit: " << item.profit << " weight: " << item.weight << " ratio: " << item.profitToWeightRatio() << endl;
    }

    cout << "Medium items: " << endl;
    for (const auto& item : medium) {
        cout << "profit: " << item.profit << " weight: " << item.weight << " ratio: " << item.profitToWeightRatio() << endl;
    }

    cout << "bad items: " << endl;
    for (const auto& item : bad) {
        cout << "profit: " << item.profit << " weight: " << item.weight << " ratio: " << item.profitToWeightRatio() << endl;
    }

    int newCapacity = 0;
    for (const auto& item: medium) {
        newCapacity += item.weight;
    }

    KnapsackInstance balancedInstance = {medium, newCapacity};
    return balancedInstance;
}

int main() {

    KnapsackInstance test = {
        {{10,20}, {20,30}, {30,60}, {50,90}},
        50
    };
    cout << "original instance: " << endl;
    for (const auto& item : test.items) {
        cout << "profit: " << item.profit << " weight: " << item.weight << " ratio: " << item.profitToWeightRatio() << endl;
    }
    cout << "Original capacity: " << test.capacity << endl;

    KnapsackInstance reduced = reduceToBalanced(test);

    cout << "reduced instance: " << endl;
    for (const auto& item : reduced.items) {
        cout << "profit: " << item.profit << " weight: " << item.weight << " ratio: " << item.profitToWeightRatio() << endl;
    }
    cout << "reduced capacity: " << reduced.capacity << endl;



    return 0;
}