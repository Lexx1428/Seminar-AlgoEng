#include <vector>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <chrono>

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


int findWmax(const vector<Item>& items) {
    int wmax = 0;
    for (const auto& item : items) {
        if (item.weight > wmax) {
            wmax = item.weight;
        }
    }
    return wmax;
}

int findPmax(const vector<Item>& items) {
    int pmax = 0;
    for (const auto& item : items) {
        if (item.profit > pmax) {
            pmax = item.profit;
        }
    }
    return pmax;
}

vector<Item> generateDataset(const int &numItems, const int &maxWeight, const int &maxValue) {
    vector<Item> items;
    items.reserve(numItems);
    srand(time(0));    //seed for randomness
    for (int i = 0; i < numItems; ++i) {
        int index = i;
        int weight = rand() % maxWeight + 1;
        int value = rand() % maxValue + 1;
        items.push_back({value, weight});
    }
    return items;
}

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
        cout << item << endl;
    }

    cout << "Medium items: " << endl;
    for (const auto& item : medium) {
        cout << item << endl;
    }

    cout << "bad items: " << endl;
    for (const auto& item : bad) {
        cout << item << endl;
    }

    int newCapacity = 0;
    int weightTracker = 0;
    for (const auto& item : sortedItems) {
        if (weightTracker + item.weight > original.capacity) {
            break;
        }
        weightTracker += item.weight;
        if (item.profitToWeightRatio() < 2*rho && item.profitToWeightRatio() > rho/2) {
            newCapacity += item.weight;
        }
    }

    KnapsackInstance balancedInstance = {medium, newCapacity};
    //KnapsackInstance2: bad items in ranges [0, 10wmax] with profits capped to 10pmax
    //KnapsackInstance3: 

    return balancedInstance;
}

void measureRuntime(KnapsackInstance input) {
    auto start = chrono::high_resolution_clock::now();
    reduceToBalanced(input);
    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double> elapsed = end-start;
    cout << "Input size = " << input.items.size() << endl << "time taken: " << elapsed.count() << " seconds" << endl;

}

int main() {

    KnapsackInstance test6 = { {{8,1}, {7,7}, {5,6}, {6,9}, {3,5}, {1,7}}, 25};
    KnapsackInstance test10 = { {{8,1}, {7,7}, {5,6}, {6,9}, {3,5}, {1,7}, {4,5}, {6,7}, {10,9}, {1,1}}, 25};
  
    KnapsackInstance test1000;
    test1000.capacity = 100000;
    test1000.items = generateDataset(1000, 10000, 10000);

    KnapsackInstance test10000;
    test10000.capacity = 1000000;
    test10000.items = generateDataset(10000, 1000, 1000);

    KnapsackInstance test100000;
    test100000.capacity = 10000000;
    test100000.items = generateDataset(100000, 10000, 10000);



    //cout << test1000 << endl;

    /*
    int pmax = findPmax(test.items);
    int wmax = findWmax(test.items);
    cout << "Wmax = " << wmax << endl;
    cout << "Pmax = " << pmax << endl;
    cout << endl;
    

    KnapsackInstance reduced = reduceToBalanced(test);

    cout << endl;
    cout << "Reduced instance: " << endl;

    cout << reduced << endl;

    */

    measureRuntime(test100000);
    return 0;
}